#include "Defs.h"

#include <stdlib.h>

#include "ItemData.h"
#include "Mission.h"
#include "Map.h"
#include "Monsters.h"
#include "Outpost.h"
#include "RPG.h"
#include "Stats.h"
#include "Utils.h"

MissionInfo Missions[MAX_DIFFICULTIES][MAX_MISSIONS];

str const MissionDifficulties[MAX_DIFFICULTIES] =
{
    "\CdVery Easy",
    "\CdEasy",
    "\CjNormal",
    "\CaHard",
    "\CaVery Hard",
    "\CaNightmare",
    "\CgInsane",
    "\CgImpossible",
    "\CgSuicide",
};

str const MissionTypes[MT_MAX] =
{
    "\CdCollection",
    "\CaKill",
    "\CaKill Auras",
    "\CaKill Reinforcements",
    "\CgAssassination",
    "\CkFind Secrets",
    "\CnFind Items",
    "\CtAchieve Combo",
};

bool RPGMap MissionInit;

NamedScript void PopulateMissions()
{
    // Wait for the Rewards List to finish processing
    while (!RewardsInit) Delay(1);

    for (int i = 0; i < MAX_DIFFICULTIES; i++)
    {
        auto MissionInfo MissionsSorted[MAX_MISSIONS];

        // Create new missions
        for (int j = 0; j < MAX_MISSIONS; j++)
            MissionsSorted[j] = CreateMission(i);

        // Sort the missions by type
        qsort(MissionsSorted, MAX_MISSIONS, sizeof(MissionInfo), MissionTypeSort);

        // Add the sorted missions into the missions array
        for (int j = 0; j < MAX_MISSIONS; j++)
            Missions[i][j] = MissionsSorted[j];

        Delay(1);
    }

    MissionInit = true;
}

NamedScript void InitMission()
{
    // Small delay before initializing the script
    Delay(4);

    // If you aren't on a mission or you're in the Outpost, return
    if (!Player.Mission.Active || CurrentLevel->UACBase) return;

    // Kill Reinforcements Mission
    if (Player.Mission.Type == MT_REINFORCEMENTS)
        MonsterTransport(CalculateAverageDifficulty(), 16 - Player.Mission.Difficulty, 1024);

    // Assassination Mission
    if (Player.Mission.Type == MT_ASSASSINATION)
    {
        // Need to wait until all monsters have been initialized
        Delay(35);

        if (MonsterID <= 1) return; // No monsters, no assassination.

        // Find valid monster to replace
        for (int i = 1; i < MonsterID; i++)
        {
            if (!Monsters[i].Init) // Skip removed monsters
                continue;

            // Valid monster
            str ActorToCheck = GetMissionMonsterActor(Player.Mission.Monster->Actor);

            if (DebugLog)
                Log("\CdDEBUG: \C-Checking: %S - Looking For: %S", Monsters[i].Actor, ActorToCheck);

            // Check for valid monster
            if (StartsWith(Monsters[i].Actor, ActorToCheck, true))
                // Store valid monster ID into array
                zsDynArrayUtils("PotentialTargets", 1, i, -1);

            // Prevent script termination
            if (!(i % 1000)) Delay(1);
        }

        // Replace target monster
        if (zsDynArrayUtils("PotentialTargets", 2, NULL, -1)) // Proceed only if array is populated
        {
            // Randomly choose a valid monster to replace
            int Chosen = zsDynArrayUtils("PotentialTargets", 3, NULL, -1);

            int LevelMod = 1 + ((Player.Mission.Difficulty + 1) * 0.125);
            LevelMod = (int)(LevelMod * RandomFixed(1.00, 1.25));
            Monsters[Chosen].LevelAdd *= LevelMod;
            Monsters[Chosen].Target = PlayerNumber() + 1;

            // Shadow Aura
            for (int i = 0; i < AURA_MAX; i++)
                Monsters[Chosen].AuraAdd[i] = true;

            Monsters[Chosen].NeedReinit = true;

            // EVIL LAUGH OF WARNING
            if (Monsters[Chosen].Threat >= 10)
                LocalAmbientSound("mission/gottarget2", 127);
            else
                LocalAmbientSound("mission/gottarget1", 127);

            if (DebugLog)
                Log("\CdDEBUG: \C-Mission Target Chosen: \Ca%d", Chosen);

            // Cleanup
            zsDynArrayUtils("PotentialTargets", 99, NULL, -1);
        }
    }
}

NamedScript void MissionDeathCheck(int Killer, MonsterStatsPtr Stats)
{
    if ((GetCVar("drpg_mission_monsterkillscount") ? Killer >= -1 : Killer > -1) && !CurrentLevel->UACBase && !GetActorProperty(0, APROP_Friendly))
    {
        // Kill Mission
        for (int i = 0; i < MAX_PLAYERS; i++)
        {
            // Player is not in-game
            if (!PlayerInGame(i)) continue;

            // Perform actor -> needed actor match checking
            if (Players(i).Mission.Active && Players(i).Mission.Type == MT_KILL)
            {
                // Determine actor name
                str Actor = GetActorClass(0);
                str NeededActor = GetMissionMonsterActor(Players(i).Mission.Monster->Actor);
                // Check to see if we match
                if (StrCmp(Actor, NeededActor) == 0)
                    Players(i).Mission.Current++;
            }
        }

        // Kill Auras Mission
        if (Stats->HasAura)
        {
            for (int i = 0; i < MAX_PLAYERS; i++)
            {
                // Player is not in-game
                if (!PlayerInGame(i)) continue;

                if (Players(i).Mission.Active && Players(i).Mission.Type == MT_KILLAURAS)
                    Players(i).Mission.Current++;
            }
        }

        // Kill Reinforcements Mission
        if (Stats->Reinforcement)
        {
            for (int i = 0; i < MAX_PLAYERS; i++)
            {
                // Player is not in-game
                if (!PlayerInGame(i)) continue;

                if (Players(i).Mission.Active && Players(i).Mission.Type == MT_REINFORCEMENTS)
                    Players(i).Mission.Current++;
            }
        }

        // Assassination Mission
        if (Stats->Target)
        {
            int Target = Stats->Target - 1;
            if (PlayerInGame(Target) && (Players(Target).Mission.Active && Players(Target).Mission.Type == MT_ASSASSINATION))
                Players(Target).Mission.Current++;
        }
    }
}

MissionInfo CreateMission(int Difficulty)
{
    int Type = Random(0, MT_MAX - 1);
    int NumPlayers = 0;
    long int RewardXP = 0;
    long int RewardRank = 0;
    int RewardCredits = 0;
    int RewardModules = 0;
    ItemInfoPtr RewardItem;
    MissionInfo Mission;

    if (CompatMode == COMPAT_DRLA && Difficulty == MAX_DIFFICULTIES - 1 && Random(0, 100) < 15)
    {
        RewardItem = GetRewardItem(Difficulty + 1);
    }
    else
    {
        RewardItem = GetRewardItem(Difficulty);
    }

    // Calculate the rewards based on all Player's average Level and Rank
    // XPTable[Player.Level + 1] / (3 + MAX_DIFFICULTIES - Difficulty) + Random(0, XPTable[Player.Level + 1] / GameSkill());
    for (int i = 0; i < MAX_PLAYERS; i++)
    {
        long int XPNext;
        long int RankNext;

        fixed RewardModulesFactor = 100.0 / (100.0 + (((fixed)Players(i).Strength + (fixed)Players(i).Defense + (fixed)Players(i).Vitality + (fixed)Players(i).Energy + (fixed)Players(i).Regeneration + (fixed)Players(i).Agility + (fixed)Players(i).Capacity + (fixed)Players(i).Luck) / 16.0));
        if (RewardModulesFactor < 0.0)
            RewardModulesFactor = 1.0;

        if (!PlayerInGame(i)) continue;

        // Prevent overflows due to trying to check for a null table index
        if (Players(i).Level >= MAX_LEVEL)
            XPNext = XPTable[MAX_LEVEL - 1];
        else
            XPNext = XPTable[Players(i).Level];
        if (Players(i).RankLevel >= MAX_RANK)
            RankNext = RankTable[MAX_RANK - 1];
        else
            RankNext = RankTable[Players(i).RankLevel];

        RewardXP += ((XPNext * ((Difficulty + 1) * (1200 - (Players(i).Level * 10))) / (10000 + (Players(i).Level * 500))) + 50) / 50 * 50;
        RewardRank += ((RankNext * ((Difficulty + 1) * (300 - (Players(i).RankLevel * 10))) / (10000 + (Players(i).RankLevel * 1000))) + 250) / 250 * 250;
        RewardCredits += ((Random(4 * (Difficulty + 1) * (Players(i).Level + 1), 12 * (Difficulty + 1) * (Players(i).Level + 1) * 2)) + 50) / 50 * 50;
        RewardModules += ((int)((RandomFixed(4.0 * ((fixed)Difficulty + 1.0) * ((fixed)Players(i).Level + 1.0), 4.0 * ((fixed)Difficulty + 1.0) * ((fixed)Players(i).Level + 1.0) * 2.0)) * (fixed)RewardModulesFactor) + 25) / 25 * 25;

        NumPlayers++;
    }

    // Average the rewards
    RewardXP /= NumPlayers;
    RewardRank /= NumPlayers;
    RewardCredits /= NumPlayers;
    RewardModules /= NumPlayers;

    // Assign info to the struct
    Mission.Active = false;
    Mission.Type = Type;
    Mission.Difficulty = Difficulty;
    Mission.RewardXP = RewardXP;
    Mission.RewardRank = RewardRank;
    Mission.RewardCredits = RewardCredits;
    Mission.RewardModules = RewardModules;
    Mission.RewardItem = RewardItem;

    // Collection Mission
    if (Type == MT_COLLECT)
    {
        // Generate Item and Amount
        int ItemIndex = Random(0, MAX_LOOT - 1);
        int Amount = 20 + (Difficulty * 20);
        ItemInfoPtr ItemPtr = &ItemData[7][ItemIndex];

        // Pass data to the Mission struct
        Mission.Item = ItemPtr;
        Mission.Amount = Amount;
    }

    // Kill Mission
    if (Type == MT_KILL)
        GetTargetMonster(&Mission);

    // Kill Auras and Kill Reinforcements Missions
    if (Type == MT_KILLAURAS || Type == MT_REINFORCEMENTS)
        Mission.Amount = 4.0 + ((15.0 * (19.0 - RandomFixed(4.0 + ((fixed)Difficulty / 2.0), 10.0 + ((fixed)Difficulty / 2.0))) / 19.0) * (1.0 + (fixed)Difficulty / (4.0 + (RandomFixed(4.0 + ((fixed)Difficulty / 2.0), 10.0 + ((fixed)Difficulty / 2.0)) + 1.0) / 3.8))) * (1.0 + (fixed)AveragePlayerLevel() / (75.0 - (fixed)Difficulty * 5.0));

    // Assassination Mission
    if (Type == MT_ASSASSINATION)
        GetTargetMonster(&Mission);

    // Find Secrets Mission
    if (Type == MT_SECRETS)
        Mission.Amount = 3 + (Difficulty * 3);

    // Find Items Mission
    if (Type == MT_ITEMS)
        Mission.Amount = 30 + (Difficulty * 30);

    // Combo Mission
    if (Type == MT_COMBO)
        Mission.Amount = 20 + (Difficulty * 20);

    return Mission;
}

void CreateMissionAt(int Difficulty, int Index)
{
    // Create new mission
    Missions[Difficulty][Index] = CreateMission(Difficulty);

    // Resort the missions in this category
    auto MissionInfo MissionsSorted[MAX_MISSIONS];
    for (int i = 0; i < MAX_MISSIONS; i++)
        MissionsSorted[i] = Missions[Difficulty][i];
    qsort(MissionsSorted, MAX_MISSIONS, sizeof(MissionInfo), MissionTypeSort);
    for (int i = 0; i < MAX_MISSIONS; i++)
        Missions[Difficulty][i] = MissionsSorted[i];
}

void CheckMission()
{
    bool Complete = false;

    // If you currently have no mission, return
    if (!Player.Mission.Active) return;

    // Collection Mission
    if (Player.Mission.Type == MT_COLLECT)
    {
        Player.Mission.Current = CheckInventory(Player.Mission.Item->Actor);

        // Complete
        if (Player.Mission.Current >= Player.Mission.Amount)
        {
            TakeInventory(Player.Mission.Item->Actor, Player.Mission.Amount);
            Complete = true;
        }
    }

    // Generic Checking
    if ((Player.Mission.Type == MT_KILL || Player.Mission.Type == MT_KILLAURAS ||
            Player.Mission.Type == MT_REINFORCEMENTS || Player.Mission.Type == MT_SECRETS ||
            Player.Mission.Type == MT_ITEMS) && !CurrentLevel->UACBase)
        if (Player.Mission.Current >= Player.Mission.Amount)
            Complete = true;

    // Assassination Mission
    if (Player.Mission.Type == MT_ASSASSINATION && !CurrentLevel->UACBase)
        if (Player.Mission.Current >= 1)
            Complete = true;

    // Combo Mission
    if (Player.Mission.Type == MT_COMBO && !CurrentLevel->UACBase)
    {
        if (Player.ComboTimer == COMBO_STOP)
            if (Player.Combo > Player.Mission.Current)
                Player.Mission.Current = Player.Combo;

        // Complete
        if (Player.Mission.Current >= Player.Mission.Amount)
            Complete = true;
    }

    // Mission Complete!
    if (Complete)
    {
        str ActorToSpawn = Player.Mission.RewardItem->Actor;

        // Compatibility Handling - DoomRL Arsenal
        // Set actors
        if (CompatMode == COMPAT_DRLA)
        {
            str ItemName = Player.Mission.RewardItem->Name;
            int ItemCategory = Player.Mission.RewardItem->Category;

            // For weapons
            if (ItemCategory == 0)
            {
                if (StrMid(ItemName, StrLen(ItemName) - 9, 6) == "Common")
                    ActorToSpawn = StrParam("%SPickup", ActorToSpawn);
                if (StrMid(ItemName, StrLen(ItemName) - 9, 6) == "Exotic")
                    ActorToSpawn = StrParam("%SPickup", ActorToSpawn);
                if (StrMid(ItemName, StrLen(ItemName) - 11, 8) == "Superior")
                    ActorToSpawn = StrParam("%SPickup", ActorToSpawn);
                if (StrMid(ItemName, StrLen(ItemName) - 9, 6) == "Unique")
                    ActorToSpawn = StrParam("%SWorldSpawnPickup", ActorToSpawn);
                if (StrMid(ItemName, StrLen(ItemName) - 10, 7) == "Demonic")
                    ActorToSpawn = StrParam("%SWorldSpawnPickup", ActorToSpawn);
                if (StrMid(ItemName, StrLen(ItemName) - 12, 9) == "Legendary")
                    ActorToSpawn = StrParam("%SWorldSpawnPickup", ActorToSpawn);
            }

            // For armors and boots
            if (ItemCategory == 3 || ItemCategory == 9)
            {
                if (StrMid(ItemName, StrLen(ItemName) - 12, 9) == "Assembled")
                    if ((ItemCategory == 3 && (ActorToSpawn == "RLCerberusArmorPickup" || ActorToSpawn == "RLCyberNanoGreenArmorPickup" || ActorToSpawn == "RLCyberNanoBlueArmorPickup" || ActorToSpawn == "RLCyberNanoRedArmorPickup")) || (ItemCategory == 9 && ActorToSpawn == "RLCerberusBootsPickup"))
                        ActorToSpawn = StrParam("%SWorldSpawnPickup", StrLeft(ActorToSpawn, StrLen(ActorToSpawn) - 6));
                if (StrMid(ItemName, StrLen(ItemName) - 9, 6) == "Unique")
                    ActorToSpawn = StrParam("%SWorldSpawnPickup", StrLeft(ActorToSpawn, StrLen(ActorToSpawn) - 6));
                if (StrMid(ItemName, StrLen(ItemName) - 10, 7) == "Demonic")
                    ActorToSpawn = StrParam("%SWorldSpawnPickup", StrLeft(ActorToSpawn, StrLen(ActorToSpawn) - 6));
                if (StrMid(ItemName, StrLen(ItemName) - 12, 9) == "Legendary")
                    ActorToSpawn = StrParam("%SWorldSpawnPickup", StrLeft(ActorToSpawn, StrLen(ActorToSpawn) - 6));
            }
        }

        // Message
        ActivatorSound("mission/complete", 127);
        SetFont("BIGFONT");
        SetHudSize(GetActivatorCVar("drpg_hud_width"), GetActivatorCVar("drpg_hud_height"), false);
        if (GetActivatorCVar("drpg_notifications_detailed"))
            HudMessage("Mission Complete!\n\n\Cj+%ld XP\n\Ck+%ld Rank\n\Cf+%d Credits\n\Cd+%d Modules\n\n\CiItem: \Cj%S",
                       Player.Mission.RewardXP, Player.Mission.RewardRank, Player.Mission.RewardCredits, Player.Mission.RewardModules, Player.Mission.RewardItem->Name);
        else
            HudMessage("Mission Complete!");
        EndHudMessage(HUDMSG_FADEOUT, MISSION_ID, "Green", GetActivatorCVar("drpg_mission_complete_x") + 0.4, GetActivatorCVar("drpg_mission_complete_y"), 3.0, 2.0);

        // Reward - XP/Rank
        Player.XP += Player.Mission.RewardXP;
        Player.Rank += Player.Mission.RewardRank;

        // Credits
        GiveInventory("DRPGCredits", Player.Mission.RewardCredits);

        // Modules
        GiveInventory("DRPGModule", Player.Mission.RewardModules);

        // Spawn Item and try to pick it up
        SpawnForced(ActorToSpawn, GetActorX(0), GetActorY(0), GetActorZ(0), 0, 0);
        SetActorVelocity(Player.TID, 0.01, 0.01, 0, true, false);

        // Clear the Mission
        ClearMission();
    }
}

void ClearMission()
{
    Player.Mission.Active = false;
    Player.Mission.Difficulty = 0;

    Player.Mission.RewardXP = 0;
    Player.Mission.RewardRank = 0;
    Player.Mission.RewardCredits = 0;
    Player.Mission.RewardItem = NULL;

    Player.Mission.Item = NULL;
    Player.Mission.Monster = NULL;
    Player.Mission.Current = 0;
    Player.Mission.Amount = 0;
}

void GetTargetMonster(MissionInfo *Mission)
{
    int Amount;
    MonsterInfoPtr PotentialMonsters[MAX_TEMP_MONSTERS];
    int NumPotentialMonsters;

    // Generate a list based on monsters' threat levels.
    for (int i = 0; i < MonsterDataAmount; i++)
    {
        MonsterInfoPtr TempMonster = &MonsterData[i];

        int TestDifficulty = TempMonster->Difficulty + (10 * TempMonster->ThreatLevel);
        int TestAmount;

        if (CompatMode != COMPAT_DRLA || CompatMonMode != COMPAT_DRLA)
        {
            TestAmount = (30 + (320 * Mission->Difficulty)) / TestDifficulty;
        }

        if (CompatMonMode == COMPAT_DRLA || CompatMonMode == COMPAT_PANDEMONIA)
        {
            if (Mission->Type == MT_KILL)
            {
                if (Mission->Difficulty == 0)
                    if (TempMonster->Difficulty > 0 & TempMonster->Difficulty < 60)
                        PotentialMonsters[NumPotentialMonsters++] = TempMonster;
                if (Mission->Difficulty == 1)
                    if (TempMonster->Difficulty > 20 & TempMonster->Difficulty < 80)
                        PotentialMonsters[NumPotentialMonsters++] = TempMonster;
                if (Mission->Difficulty == 2)
                    if (TempMonster->Difficulty > 40 & TempMonster->Difficulty < 100)
                        PotentialMonsters[NumPotentialMonsters++] = TempMonster;
                if (Mission->Difficulty == 3)
                    if (TempMonster->Difficulty > 100 & TempMonster->Difficulty < 160)
                        PotentialMonsters[NumPotentialMonsters++] = TempMonster;
                if (Mission->Difficulty == 4)
                    if (TempMonster->Difficulty > 130 & TempMonster->Difficulty < 190)
                        PotentialMonsters[NumPotentialMonsters++] = TempMonster;
                if (Mission->Difficulty == 5)
                    if (TempMonster->Difficulty > 160 & TempMonster->Difficulty < 220)
                        PotentialMonsters[NumPotentialMonsters++] = TempMonster;
                if (Mission->Difficulty == 6)
                    if (TempMonster->Difficulty > 220 & TempMonster->Difficulty < 280)
                        PotentialMonsters[NumPotentialMonsters++] = TempMonster;
                if (Mission->Difficulty == 7)
                    if (TempMonster->Difficulty > 240 & TempMonster->Difficulty < 300)
                        PotentialMonsters[NumPotentialMonsters++] = TempMonster;
                if (Mission->Difficulty == 8)
                    if (TempMonster->Difficulty > 260 & TempMonster->Difficulty < 320)
                        PotentialMonsters[NumPotentialMonsters++] = TempMonster;
            }

            if (Mission->Type == MT_ASSASSINATION)
            {
                if (Mission->Difficulty == 0)
                    if (TempMonster->Difficulty > 40 & TempMonster->Difficulty < 63)
                        PotentialMonsters[NumPotentialMonsters++] = TempMonster;
                if (Mission->Difficulty == 1)
                    if (TempMonster->Difficulty > 80 & TempMonster->Difficulty < 108)
                        PotentialMonsters[NumPotentialMonsters++] = TempMonster;
                if (Mission->Difficulty == 2)
                    if (TempMonster->Difficulty > 110 & TempMonster->Difficulty < 130)
                        PotentialMonsters[NumPotentialMonsters++] = TempMonster;
                if (Mission->Difficulty == 3)
                    if (TempMonster->Difficulty > 130 & TempMonster->Difficulty < 160)
                        PotentialMonsters[NumPotentialMonsters++] = TempMonster;
                if (Mission->Difficulty == 4)
                    if (TempMonster->Difficulty > 151 & TempMonster->Difficulty < 168)
                        PotentialMonsters[NumPotentialMonsters++] = TempMonster;
                if (Mission->Difficulty == 5)
                    if (TempMonster->Difficulty > 170 & TempMonster->Difficulty < 217)
                        PotentialMonsters[NumPotentialMonsters++] = TempMonster;
                if (Mission->Difficulty == 6)
                    if (TempMonster->Difficulty > 220 & TempMonster->Difficulty < 280)
                        PotentialMonsters[NumPotentialMonsters++] = TempMonster;
                if (Mission->Difficulty == 7)
                    if (TempMonster->Difficulty > 240 & TempMonster->Difficulty < 300)
                        PotentialMonsters[NumPotentialMonsters++] = TempMonster;
                if (Mission->Difficulty == 8)
                    if (TempMonster->Difficulty > 260 & TempMonster->Difficulty < 320)
                        PotentialMonsters[NumPotentialMonsters++] = TempMonster;
            }
        }
        else
        {
            if (Mission->Type != MT_KILL)
            {
                if (TempMonster->Difficulty > ((Mission->Difficulty + 1) * 11) - 11 &&
                        TempMonster->Difficulty < ((Mission->Difficulty + 1) * 11) + 11)
                    PotentialMonsters[NumPotentialMonsters++] = TempMonster;
            }
            else
            {
                if (!TempMonster->Boss && TestAmount >= 5 &&
                        TestAmount <= 40)
                    PotentialMonsters[NumPotentialMonsters++] = TempMonster;
            }
        }
    }

    if (NumPotentialMonsters < 1)
    {
        Log("\CgERROR: \C-GetTargetMonster for %S mission on difficulty %d has no monsters!", Mission->Type == MT_KILL? "kill" : "assassination", Mission->Difficulty + 1);
        Mission->Monster = NULL;
        Mission->Amount = 0;
        return;
    }

    MonsterInfoPtr MonsterPtr = PotentialMonsters[Random(0, NumPotentialMonsters - 1)];

    // Now that we know the monster type, we can calculate an amount
    int EffectiveDifficulty = MonsterPtr->Difficulty + (10 * MonsterPtr->ThreatLevel);
    int BaseAmount;

    if (CompatMonMode == COMPAT_DRLA || CompatMonMode == COMPAT_PANDEMONIA)
    {
        BaseAmount = (15.0 * (26.0 - (fixed)MonsterPtr->ThreatLevel) / 26.0) * (1.0 + (fixed)Mission->Difficulty / (4.0 + ((fixed)MonsterPtr->ThreatLevel + 1.0) / 5.2)) * (1.0 + (fixed)AveragePlayerLevel() / (75.0 - (fixed)Mission->Difficulty * 5.0));
    }
    else
    {
        BaseAmount = (30 + (320 * Mission->Difficulty)) / EffectiveDifficulty;
    }

    Amount = RoundInt(RandomFixed((fixed)BaseAmount * 0.8, (fixed)BaseAmount * 1.2));

    if (Amount < 1) Amount = 1;

    // Insert info into the passed struct
    Mission->Monster = MonsterPtr;
    Mission->Amount = Amount;
}

int CalculateAverageDifficulty()
{
    int NumPlayers;
    int AvgDifficulty;

    for (int i = 0; i < MAX_PLAYERS; i++)
    {
        if (!PlayerInGame(i)) continue;

        if (Players(i).Mission.Active)
            AvgDifficulty += Players(i).Mission.Difficulty;

        NumPlayers++;
    }

    AvgDifficulty /= NumPlayers;

    return AvgDifficulty;
}

str GetMissionMonsterActor(str Actor)
{
    if (CompatMonMode != COMPAT_NONE)
        return StrParam("%SRPG", Actor);
    else if (CompatMode == COMPAT_EXTRAS)
        return StrParam("DRPG%SExtras", Actor);

    return StrParam("DRPG%S", Actor);
}

int MissionTypeSort(void const *Left, void const *Right)
{
    MissionInfo LeftMission = *((MissionInfo *)Left);
    MissionInfo RightMission = *((MissionInfo *)Right);

    if (LeftMission.Type < RightMission.Type) return -1;
    if (LeftMission.Type > RightMission.Type) return 1;

    return 0;
}
