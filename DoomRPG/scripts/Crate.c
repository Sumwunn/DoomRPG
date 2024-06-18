#include "Defs.h"
#include "RPG.h"

#include "Crate.h"
#include "ItemData.h"
#include "Map.h"
#include "Stats.h"
#include "Utils.h"

str const CrateRarityNames[MAX_DIFFICULTIES] =
{
    "\CcBasic",
    "\CjCommon",
    "\CqUncommon",
    "\CiSuperior",
    "\CnRare",
    "\CnVery Rare",
    "\CiElite",
    "\CtLegendary",
    "\CfSupreme"
};

RPGMap int CrateID = 0;
RPGMap CrateInfo Crates[CRATE_MAX];

NamedScript Type_OPEN void CrateLoop()
{
    while (true)
    {
        for (int i = 0; i < CrateID; i++)
        {
            // Hacking cooldown timer handling
            if (Crates[i].HackingCooldown > 0)
                Crates[i].HackingCooldown--;

            // Supply Drop spawned crates should never have a firewall
            if (Crates[i].SupplyDrop && Crates[i].Firewall)
            {
                Crates[i].Firewall = false;
                SetActorState(Crates[i].TID, "Normal");
            }
        }

        Delay(1);
    }
}

NamedScript DECORATE void InitCrate()
{
    // Delay while the map is being initialized
    while (!CurrentLevel->Init) Delay(1);

    int TID = UniqueTID();
    int Modifier;
    int Amount = 3;
    int Rarity = 0;
    int Firewall = 0;

    // Calculate Modifier
    if (GetCVar("drpg_loot_type") == 0)
        Modifier = RoundInt(7.5 * MapLevelModifier + 7.5 * (fixed)AveragePlayerLuck() / 100.0);
    if (GetCVar("drpg_loot_type") == 1)
        Modifier = RoundInt(15.0 * MapLevelModifier);
    if (GetCVar("drpg_loot_type") == 2)
        Modifier = RoundInt(15.0 * (fixed)AveragePlayerLuck() / 100.0);
    if (GetCVar("drpg_loot_type") == 3)
        Modifier = Random(0,15);
    if (Modifier > 15)
        Modifier = 15;

    if (CrateID >= CRATE_MAX)
    {
        Log("\CgERROR:\C- Maximum Crate ID exceeded!");
        Thing_Remove(0);
        return;
    }

    // Calculate Rarity
    for (int i = Rarity; i < MAX_DIFFICULTIES - 1; i++)
        if (Random(0, Random(3, MAX_DIFFICULTIES) + Rarity - Modifier) <= 0)
            Rarity++;
    if (Rarity < 0) // Make sure the Rarity still isn't -1, or else bad things will happen
        Rarity = 0;
    if (Rarity > 1 + RoundInt(((fixed)MAX_DIFFICULTIES - 1.0) * MapLevelModifier))
        Rarity = 1 + RoundInt(((fixed)MAX_DIFFICULTIES - 1.0) * MapLevelModifier);
    if (Rarity > MAX_DIFFICULTIES - 1)
        Rarity = MAX_DIFFICULTIES - 1;

    // Calculate Amount
    for (int i = Amount; i < CRATE_MAX_ITEMS; i++)
        if (Random(0, i) <= 0)
            Amount++;
    Amount += PlayerCount() - 1;      // Add +1 per extra player in-game
    if (Amount > CRATE_MAX_ITEMS - 1) // Make sure we don't overflow
        Amount = CRATE_MAX_ITEMS;

    // Calculate Firewall odds
    Firewall = Rarity * 10;
    if (Firewall <= 10)
        Firewall = 10;
    if (Firewall > 100)
        Firewall = 100;

    // Setup Crate
    Crates[CrateID].Firewall = (Random(0, 100) <= Firewall);
    Crates[CrateID].TID = TID;
    Crates[CrateID].Amount = Amount;
    Crates[CrateID].Rarity = Rarity;
    Crates[CrateID].Hacking = -1;
    Thing_ChangeTID(0, TID);
    SetUserVariable(0, "user_id", CrateID);
    SetUserVariable(0, "user_firewall", (int)Crates[CrateID].Firewall);
    if (Crates[CrateID].Firewall)
        SetActorState(0, "Firewall");
    else
        SetActorState(0, "Normal");

    for (int x = 0; x < CRATE_MAX_ITEMS; x++)
    {
        Crates[CrateID].Active[x] = false;
        Crates[CrateID].Item[x] = NULL;
    }

    CrateID++;
}

NamedScript DECORATE void UseCrate(int ID)
{
    // Hackity hacks so you can't open the crate yet if the rewards list is not done being generated
    if (!RewardsInit) return;

    SetActivator(GetActorProperty(0, APROP_TargetTID));

    // Hack for if you somehow activate a crate while you're hacking another one
    if (Player.CrateHacking) return;

    // Someone else is hacking the crate
    if (Crates[ID].Hacking >= 0)
    {
        if (Crates[ID].Hacking != PlayerNumber())
        {
            PrintError(StrParam("%tS\C- is already hacking an item in this crate", Crates[Player.CrateID].Hacking + 1));
            ActivatorSound("menu/error", 127);
        }

        return;
    }

    // Check if you're in a menu
    if (Player.InMenu || Player.InShop || Player.CrateOpen) return;

    // Check if the crate is empty
    if (Crates[ID].Generated && CrateEmpty(ID)) return;

    // Check if the crate is firewalled, if so, initiate hacking
    if (Crates[ID].Firewall)
    {
        if (Crates[ID].HackingCooldown > 0)
        {
            PrintError(StrParam("Security Lockdown Triggered\n\nYou must wait %S before reattemtping the hack", FormatTime(Crates[ID].HackingCooldown)));
            ActivatorSound("menu/error", 127);
        }
        else
        {
            Player.CrateID = ID;
            CrateHack();
        }
        return;
    }

    // Generate the crate contents
    if (!Crates[ID].Generated)
    {
        GenerateCrate(ID, Crates[ID].Amount);
        Crates[ID].Generated = true;
    }

    Player.CrateOpen = true;
    Player.CrateID = ID;
    Player.CrateIndex = 0;

    ActivatorSound("crate/open", 127);

    Delay(2);

    while (Player.CrateOpen)
    {
        SetHudSize(GetActivatorCVar("drpg_menu_width"), GetActivatorCVar("drpg_menu_height"), true);

        // Draw cords
        fixed X = GetActivatorCVar("drpg_menu_x");
        fixed Y = GetActivatorCVar("drpg_menu_y");

        // Draw Border
        // These are pushed back a bit so the border doesn't overlap anything
        if (GetActivatorCVar("drpg_menu_background_border"))
            DrawBorder("Bor", -1, 8, X + (-5.0), Y + (-3.0), 470, 470);

        SetPlayerProperty(0, 1, PROP_TOTALLYFROZEN);

        DrawCrate(X, Y);

        Delay(1);

        CrateInput();
    }

    SetPlayerProperty(0, 0, PROP_TOTALLYFROZEN);
}

NamedScriptSync bool GenerateCrateNodes(CrateInfo RPGMap *Crate)
{
    int StopCount = 4000;
    bool Conflict;
    int Start, End;

    while (Crate->GenType < NODE_MAX)
    {
        Conflict = false;
        Start = Random(0, 300 - 12);
        End = 4 * Random(1, 2);

        // Make sure we don't generate over the max amount of allowed node types
        if (Crate->GenNodes[Crate->GenType] >= Crate->NodeMax[Crate->GenType])
        {
            Crate->GenType++;
            continue;
        }

        // Make sure there are no node conflicts
        for (int i = 0; i < MAX_NODES; i++)
            if (Crate->Nodes[i].Active && !Conflict)
                for (int j = 0; j < End; j++)
                    if (Start + j >= Crate->Nodes[i].Start && Start + j <= Crate->Nodes[i].Start + Crate->Nodes[i].End)
                    {
                        Conflict = true;
                        break;
                    }

        // Get new coordinates due to a conflict
        if (Conflict)
            continue;

        // Setup Node
        Crate->Nodes[Crate->GenTotal].Active = true;
        Crate->Nodes[Crate->GenTotal].Type = Crate->GenType;
        Crate->Nodes[Crate->GenTotal].Start = Start;
        Crate->Nodes[Crate->GenTotal].End = End;

        Crate->GenTotal++;
        Crate->GenNodes[Crate->GenType]++;

        if (StopCount-- == 0)
            return false;
    }

    Crate->HacksGenerated = true;
    return true;
}

NamedScript void CrateHack()
{
    CrateInfo RPGMap *Crate = &Crates[Player.CrateID];

    if (!Crate->HacksGenerated)
    {
        Crate->Tries = 3;

        Crate->NodeMax[0] = 8;
        Crate->NodeMax[1] = 8;
        Crate->NodeMax[2] = 1;
        Crate->NodeMax[3] = 2;
        Crate->NodeMax[4] = 2;
        Crate->NodeMax[5] = 1;
        Crate->NodeMax[6] = 1;
        Crate->NodeMax[7] = 1;

        // DRLA - Technician gets bonuses
        if (CompatMode == COMPAT_DRLA && PlayerClass(PlayerNumber()) == 2)
        {
            Crate->NodeMax[NODE_CRITFAIL] /= 4;
            Crate->NodeMax[NODE_EXPLODE] /= 4;
            Crate->NodeMax[NODE_XP] *= 2;
            Crate->NodeMax[NODE_RANK] *= 2;
            Crate->NodeMax[NODE_RETRY] *= 2;
            Crate->NodeMax[NODE_ADD] *= 2;
        }

        // Initialize things
        for (int i = 0; i < MAX_NODES; i++)
            Crate->Nodes[i].Active = false;
        for (int i = 0; i < NODE_MAX; i++)
            Crate->GenNodes[i] = 0;

        while (!GenerateCrateNodes(Crate));
    };

    int X = Random(25, 275);
    int Direction = (Random(1, 2) == 1 ? -1 : 1);
    long int XPBonus;
    long int RankBonus;

    // Prevent the use input from leaking into the input handling below (aka exploding a crate immediately in your face)
    Delay(4);

    ActivatorSound("hacking/begin", 127);

    SetPlayerProperty(0, 1, PROP_TOTALLYFROZEN);
    Crates[Player.CrateID].Hacking = PlayerNumber();
    Player.CrateHacking = true;

    while (Player.CrateHacking)
    {
        SetHudSize(320, 240, false);

        // HACKIFICATIONS
        SetFont("SMALLFONT");
        HudMessage("HACKING IN PROGRESS...");
        EndHudMessage(HUDMSG_PLAIN, 0, "Green", 160.0, 100.0, 0.05);
        if (!GetCVar("drpg_toaster"))
            for (int i = 0; i < 16; i++)
            {
                HudMessage("%d", Random(0, 1));
                EndHudMessage(HUDMSG_FADEOUT, 0, ColorNames[Random(1, 25)], 10.0 + (8.0 * Random(1, 36)), 140.0 + (8.0 * Random(1, 10)), 0.25, 0.75);
            }

        // Cursor
        PrintSprite("HackCurs", 0, 10.1 + X, 120.0, 0.05);

        // Nodes
        for (int i = 0; i < MAX_NODES; i++)
            if (Crate->Nodes[i].Active)
            {
                SetHudClipRect(10 + Crate->Nodes[i].Start, 120 - 8, Crate->Nodes[i].End, 120 + 8);
                PrintSprite(StrParam("HackNod%d", Crate->Nodes[i].Type + 1), 0, 10.0 + Crate->Nodes[i].Start, 120.0, 0.05);
                SetHudClipRect(0, 0, 0, 0);
            }

        // Back
        PrintSprite(StrParam("HackBor%d", Crate->Tries), 0, 10.1, 120.0, 0.05);
        PrintSprite("HackBack", 0, 10.1, 120.0, 0.05);

        // Move Cursor
        X += Direction;
        if ((Crates[Player.CrateID].Rarity / 3) > 0)
            X += ((Crates[Player.CrateID].Rarity / 3) * Direction);
        if (X < 0)
        {
            X = 0;
            Direction = -Direction;
        }
        else if (X > 300)
        {
            X = 300;
            Direction = -Direction;
        }

        // Input
        if (CheckInput(BT_USE, KEY_PRESSED, false, PlayerNumber()))
        {
            bool HitNothing = true;
            XPBonus = ((XPTable[Player.Level] / (long)(10 + RoundInt(40.0 * (Player.Level / 100.0)))) + 50) / 50 * 50;
            RankBonus = ((RankTable[Player.RankLevel] / (long)(40 + RoundInt(160.0 * (Player.RankLevel / 24.0)))) + 250) / 250 * 250;

            // Check Nodes
            for (int i = 0; i < MAX_NODES; i++)
                if (Crate->Nodes[i].Active)
                    if (X >= Crate->Nodes[i].Start && X <= Crate->Nodes[i].Start + Crate->Nodes[i].End)
                    {
                        HitNothing = false;
                        Crate->Nodes[i].Active = false;

                        switch (Crate->Nodes[i].Type)
                        {
                        case NODE_CRITFAIL:
                            ActivatorSound("hacking/critfail", 127);
                            SetFont("SMALLFONT");
                            HudMessage("Security Lockdown Triggered!");
                            EndHudMessage(HUDMSG_FADEOUT, 0, "Red", 160.0, 140.0, 2.0, 1.0);
                            Crates[Player.CrateID].HackingCooldown += 35 * 60;
                            Crate->Tries = 3;
                            Player.CrateHacking = false;
                            break;
                        case NODE_EXPLODE:
                            ActivatorSound("hacking/critfail", 127);
                            SetFont("SMALLFONT");
                            HudMessage("Explosive Device Triggered!");
                            EndHudMessage(HUDMSG_FADEOUT, 0, "Orange", 160.0, 140.0, 2.0, 1.0);
                            SetActorState(Crates[Player.CrateID].TID, "Explode");
                            Player.CrateHacking = false;
                            Player.CrateOpen = false;
                            break;
                        case NODE_UNLOCK:
                            ActivatorSound("hacking/success", 127);
                            SetFont("SMALLFONT");
                            HudMessage("Hack Successful");
                            EndHudMessage(HUDMSG_FADEOUT, 0, "Green", 160.0, 140.0, 2.0, 1.0);
                            Crates[Player.CrateID].Firewall = false;
                            SetActorState(Crates[Player.CrateID].TID, "Normal");
                            SetUserVariable(Crates[Player.CrateID].TID, "user_firewall", (int)Crates[CrateID].Firewall);
                            Player.CrateHacking = false;
                            break;
                        case NODE_XP:
                            ActivatorSound("hacking/select", 127);
                            SetFont("SMALLFONT");
                            HudMessage("+%ld XP", XPBonus);
                            EndHudMessage(HUDMSG_FADEOUT, 0, "White", 160.0, 140.0, 2.0, 1.0);
                            Player.XP += XPBonus;
                            break;
                        case NODE_RANK:
                            ActivatorSound("hacking/select", 127);
                            SetFont("SMALLFONT");
                            HudMessage("+%ld Rank", RankBonus);
                            EndHudMessage(HUDMSG_FADEOUT, 0, "Yellow", 160.0, 140.0, 2.0, 1.0);
                            Player.Rank += RankBonus;
                            break;
                        case NODE_RETRY:
                            ActivatorSound("hacking/select", 127);
                            SetFont("SMALLFONT");
                            HudMessage("Extra Try");
                            EndHudMessage(HUDMSG_FADEOUT, 0, "Blue", 160.0, 140.0, 2.0, 1.0);
                            if (Crate->Tries < 3)
                                Crate->Tries++;
                            break;
                        case NODE_ADD:
                            ActivatorSound("hacking/select", 127);
                            SetFont("SMALLFONT");
                            HudMessage("Extra Item!");
                            EndHudMessage(HUDMSG_FADEOUT, 0, "Cyan", 160.0, 140.0, 2.0, 1.0);
                            if (Crates[Player.CrateID].Amount < CRATE_MAX_ITEMS - 1)
                                Crates[Player.CrateID].Amount++;
                            break;
                        case NODE_RARITY:
                            ActivatorSound("hacking/select", 127);
                            SetFont("SMALLFONT");
                            HudMessage("Rarity Upgrade!");
                            EndHudMessage(HUDMSG_FADEOUT, 0, "Purple", 160.0, 140.0, 2.0, 1.0);
                            if (Crates[Player.CrateID].Rarity < MAX_DIFFICULTIES - 1)
                                Crates[Player.CrateID].Rarity++;
                            break;
                        }

                        break;
                    }

            if (HitNothing)
            {
                ActivatorSound("hacking/select", 127);
                Crate->Tries--;

                // You fail it
                if (Crate->Tries <= 0)
                {
                    ActivatorSound("hacking/fail", 127);
                    Crates[Player.CrateID].HackingCooldown += 35 * 30;
                    Crate->Tries = 3;
                    Player.CrateHacking = false;
                }
            }
        }

        Delay(1);
    }

    Delay(4);

    SetPlayerProperty(0, 0, PROP_TOTALLYFROZEN);
    Crates[Player.CrateID].Hacking = -1;
    Player.CrateHacking = false;
}

typedef struct _setinfo
{
    str Name;
    str Token;
    bool Synthfire;

    struct SetItemInfo
    {
        str Name;
        fixed Chance;
    } Items[32];
} SetInfo;

void GenerateCrate(int ID, int Amount)
{
    int i;
    int Firewalls;
    int NumPlayers = PlayerCount();
    fixed Modifier;
    fixed ShieldPartChance = GetCVarFixed("drpg_loot_crate_spc");

    // Set variables for new item
    ItemInfoPtr NewItem;
    str NewItemName;
    str NewItemActor;
    int NewItemIndex;
    int NewItemCategory;
    int NewItemSpawned;
    int NewItemPenalty;
    bool NewItemSelected;
    bool NewItemIsAdditional;

    // Set variables for old item
    ItemInfoPtr OldItem;
    str OldItemActor;
    int OldItemCategory;

    // Compatibility Handling - DoomRL Arsenal Extended
    // Set variables for Trespasser's scanned item
    ItemInfoPtr ScannedItem;
    str ScannedItemActor;
    int ScannedItemCategory;
    bool TrespasserChance;
    bool ItemFound;

    // Calculate Modifier
    Modifier = (fixed)Crates[ID].Rarity / ((fixed)MAX_DIFFICULTIES - 1.0);

    if (Crates[ID].SupplyDrop)
    {
        NewItem = FindItem("DRPGBigBackpack");

        if (NewItem == GetBlankItem())
        {
            Log("\CgWARNING: Couldn't find the backpack item!");
        }

        // Create a backpack for every player in the game
        for (int j = 0; j < NumPlayers; j++)
        {
            Crates[ID].Active[i] = true;
            Crates[ID].Item[i] = NewItem;
            i++;
        }
    }

    // Get items for crate
    int Rarity = Crates[ID].Rarity;

    // Compatibility Handling - DoomRL Arsenal
    // Set variables for Common/Exotic/Superior/Unique/Demonic/Legendary items
    // For weapons
    bool WeaponExoticChance;
    bool WeaponSuperiorChance;
    bool WeaponUniqueChance;
    bool WeaponDemonicChance;
    bool WeaponLegendaryChance;

    // For armors and boots
    bool ArmorAssembledChance;
    bool ArmorExoticChance;
    bool ArmorSuperiorChance;
    bool ArmorUniqueChance;
    bool ArmorDemonicChance;
    bool ArmorLegendaryChance;

    // Compatibility Handling - DoomRL Arsenal
    if (CompatMode == COMPAT_DRLA)
    {
        // Calculate the chances for Common/Assembled/Exotic/Superior/Unique/Demonic/Legendary armor and boots
        // For weapon
        WeaponExoticChance = RandomFixed(0.0, 100.0) <= Player.WeaponExoticChance;
        WeaponSuperiorChance = RandomFixed(0.0, 100.0) <= Player.WeaponSuperiorChance;
        WeaponUniqueChance = RandomFixed(0.0, 100.0) <= Player.WeaponUniqueChance;
        WeaponDemonicChance = RandomFixed(0.0, 100.0) <= Player.WeaponDemonicChance;
        WeaponLegendaryChance = RandomFixed(0.0, 100.0) <= Player.WeaponLegendaryChance;

        // For armor
        ArmorAssembledChance = RandomFixed(0.0, 100.0) <= Player.ArmorAssembledChance;
        ArmorExoticChance = RandomFixed(0.0, 100.0) <= Player.ArmorExoticChance;
        ArmorSuperiorChance = RandomFixed(0.0, 100.0) <= Player.ArmorSuperiorChance;
        ArmorUniqueChance = RandomFixed(0.0, 100.0) <= Player.ArmorUniqueChance;
        ArmorDemonicChance = RandomFixed(0.0, 100.0) <= Player.ArmorDemonicChance;
        ArmorLegendaryChance = RandomFixed(0.0, 100.0) <= Player.ArmorLegendaryChance;

        // Compatibility Handling - DoomRL Arsenal Extended
        if (CompatModeEx == COMPAT_DRLAX)
        {
            // Set Trespasser's scanned item
            if (PlayerClass(PlayerNumber()) == 11) // Trespasser
            {
                // Get Trespasser's chance
                TrespasserChance = true;

                // Set scanned item actor
                ScannedItemActor = StrParam("%S", ScriptCall("DRLAX_DRPG_Functions", "GetTrespasserScans", PlayerNumber()));

                // Find actor for scanned item
                while(!ItemFound && TrespasserChance)
                {
                    if (FindItem(ScannedItemActor)->Actor != "None")
                    {
                        ScannedItem = FindItem(ScannedItemActor);
                        ScannedItemCategory = ScannedItem->Category;
                        ItemFound = true;
                    }
                    else
                    {
                        if (StrMid(ScannedItemActor, StrLen(ScannedItemActor) - 16, 16) == "WorldSpawnPickup") //RLJetpackArmorWorldSpawnPickup RLWidowmakerSMGWorldSpawnPickup
                            ScannedItemActor = StrParam("%SPickup", StrLeft(ScannedItemActor, StrLen(ScannedItemActor) - 16));
                        else
                            ScannedItemActor = StrLeft(ScannedItemActor, StrLen(ScannedItemActor) - 1);
                    }

                    // If find actor is fail
                    if (StrLen(ScannedItemActor) <= 0)
                        TrespasserChance = false;
                }
            }
        }
    }



    for (bool SkipShieldPart = true; i < Amount; i++)
    {
        if (RandomFixed(0.0, 99.9) > ShieldPartChance)
            SkipShieldPart = true;
        else
            SkipShieldPart = false;

        while (!NewItemSelected)
        {
            NewItem = GetRewardItem(Rarity, SkipShieldPart);

            Crates[ID].Active[i] = true;

            if (NewItem == GetBlankItem())
                NewItemSelected = false;
            else
                NewItemSelected = true;

            // Add items for random if item categories is Misc (chance is 35%)
            if (NewItem->Category == ItemCategories && Random(0, 100) > 65)
            {
                int AddItemCategory;
                int IndexMin;
                int IndexMax;

                switch (Random(0, 2))
                {
                // Credits
                case 0:
                    AddItemCategory = 10;
                    IndexMin = ItemMax[AddItemCategory] - (8 - RoundInt(4.0 * Modifier));
                    IndexMax = ItemMax[AddItemCategory] - (7 - RoundInt(6.0 * Modifier));
                    NewItemIsAdditional = true;
                    break;
                // Ammo
                case 1:
                    AddItemCategory = 1;
                    IndexMin = 0;
                    IndexMax = ItemMax[AddItemCategory] - 4;
                    NewItemIsAdditional = true;
                    break;
                // Loot
                case 2:
                    AddItemCategory = 7;
                    IndexMin = 0;
                    IndexMax = ItemMax[AddItemCategory] - 1;
                    NewItemIsAdditional = true;
                    break;
                }

                NewItem = &ItemData[AddItemCategory][Random(IndexMin, IndexMax)];
            }

            // Get info for new item
            NewItemName = NewItem->Name;
            NewItemActor = NewItem->Actor;
            NewItemIndex = NewItem->Index;
            NewItemCategory = NewItem->Category;
            NewItemSpawned = NewItem->Spawned;

            // Set penalty for an item already spawned
            if (NewItemSpawned < 5)
                NewItemPenalty = NewItemSpawned;
            else
                NewItemPenalty = 5;

            // Check the item
            for (int j = 0; j < i; j++)
            {
                // Get info for old item
                OldItem = Crates[ID].Item[j];
                OldItemActor = Crates[ID].Item[j]->Actor;
                OldItemCategory = Crates[ID].Item[j]->Category;

                //  Item check fails if the item was spawned before the crate was opened
                if (NewItemSpawned > 0 && Random(0, NewItemPenalty * (200 - NewItemPenalty * 20)) > 50)
                {
                    NewItemSelected = false;
                    break;
                }
                //  Item check fails if the item repetition in crate
                else if (NewItemActor == OldItemActor && !NewItemIsAdditional)
                {
                    NewItemSelected = false;
                    break;
                }
                //  Item check fails if the category repetition in crate
                else if (NewItemCategory == OldItemCategory && (NewItemCategory != 1 && NewItemCategory != 6 && NewItemCategory != 7 && NewItemCategory != ItemCategories))
                {
                    NewItemSelected = false;
                    break;
                }
                //  Item check fails if the small ammo appear in rare crates
                else if (NewItemCategory == 1 && Rarity > 1 && (NewItemIndex == 0 || NewItemIndex == 2 || NewItemIndex == 4 || NewItemIndex == 6))
                {
                    NewItemSelected = false;
                    break;
                }
                //  Item check fails if the weapons, armor and boots together in crate
                else if ((NewItemCategory == 0 && (OldItemCategory == 3 || OldItemCategory == 9))
                         || (NewItemCategory == 3 && (OldItemCategory == 0 || OldItemCategory == 9))
                         || (NewItemCategory == 9 && (OldItemCategory == 0 || OldItemCategory == 3)))
                {
                    NewItemSelected = false;
                    break;
                }
            }

            // Compatibility Handling - DoomRL Arsenal
            // Check chances spawn for the Assembled/Exotic/Superior/Unique/Demonic/Legendary weapons and armor
            if (NewItemSelected && CompatMode == COMPAT_DRLA)
            {
                // For weapon
                if (NewItemCategory == 0)
                {
                    if (!WeaponExoticChance && StrMid(NewItemName, StrLen(NewItemName) - 9, 6) == "Exotic") NewItemSelected = false;
                    else if (!WeaponSuperiorChance && StrMid(NewItemName, StrLen(NewItemName) - 11, 8) == "Superior") NewItemSelected = false;
                    else if (!WeaponUniqueChance && StrMid(NewItemName, StrLen(NewItemName) - 9, 6) == "Unique") NewItemSelected = false;
                    else if (!WeaponDemonicChance && StrMid(NewItemName, StrLen(NewItemName) - 10, 7) == "Demonic") NewItemSelected = false;
                    else if (!WeaponLegendaryChance && StrMid(NewItemName, StrLen(NewItemName) - 12, 9) == "Legendary") NewItemSelected = false;
                }
                // For armor and boots
                else if (NewItemCategory == 3 || NewItemCategory == 9)
                {
                    if (!ArmorAssembledChance && StrMid(NewItemName, StrLen(NewItemName) - 12, 9) == "Assembled") NewItemSelected = false;
                    else if (!ArmorExoticChance && StrMid(NewItemName, StrLen(NewItemName) - 9, 6) == "Exotic") NewItemSelected = false;
                    else if (!ArmorSuperiorChance && StrMid(NewItemName, StrLen(NewItemName) - 11, 8) == "Superior") NewItemSelected = false;
                    else if (!ArmorUniqueChance && StrMid(NewItemName, StrLen(NewItemName) - 9, 6) == "Unique") NewItemSelected = false;
                    else if (!ArmorDemonicChance && StrMid(NewItemName, StrLen(NewItemName) - 10, 7) == "Demonic") NewItemSelected = false;
                    else if (!ArmorLegendaryChance && StrMid(NewItemName, StrLen(NewItemName) - 12, 9) == "Legendary") NewItemSelected = false;
                }

                // Special chance check for boots (standard 10% chance from DoomRL Arsenal)
                if (NewItemCategory == 9 && Random(0, 100) < 90)  NewItemSelected = false;
            }

            // Compatibility Handling - DoomRL Arsenal Extended
            // Check Trespasser's scanned item
            if (NewItemSelected && TrespasserChance && CompatModeEx == COMPAT_DRLAX)
            {
                // Check scanned item category and Trespasser's chance
                if (NewItemCategory == ScannedItemCategory && Random(0, 100) <= 30)
                {
                    NewItem = ScannedItem;
                    TrespasserChance = false;
                }
            }

            // Set a new item for crate, if it has been selected
            if (NewItemSelected)
                Crates[ID].Item[i] = NewItem;
        }

        // Add to the spawned counter if the item is shield parts
        if (NewItemCategory == 5)
        {
            for (int k = 0; k < ItemMax[NewItemCategory]; k++)
                if (NewItemActor == ItemData[NewItemCategory][k].Actor)
                    ItemData[NewItemCategory][k].Spawned++;
        }

        // Compatibility Handling - DoomRL Arsenal
        // Set chances and add to the spawned counter for Assembled/Exotic/Superior/Unique/Demonic/Legendary weapons and armor
        if (CompatMode == COMPAT_DRLA)
        {
            // Set chances for weapons
            if (NewItemCategory == 0)
            {
                // For Exotic
                if (StrMid(NewItemName, StrLen(NewItemName) - 9, 6) == "Exotic")
                    Player.WeaponExoticChance = 0;
                else
                    Player.WeaponExoticChance += 15.0 * Modifier / 15.0;
                // For Superior
                if (StrMid(NewItemName, StrLen(NewItemName) - 11, 8) == "Superior")
                    Player.WeaponSuperiorChance = 0;
                else
                    Player.WeaponSuperiorChance += 0.5 * Modifier / 15.0;
                // For Unique
                if (StrMid(NewItemName, StrLen(NewItemName) - 9, 6) == "Unique")
                    Player.WeaponUniqueChance = 0;
                else
                    Player.WeaponUniqueChance += 2.5 * Modifier / 15.0;
                // For Demonic
                if (StrMid(NewItemName, StrLen(NewItemName) - 10, 7) == "Demonic")
                    Player.WeaponDemonicChance = 0;
                else
                    Player.WeaponDemonicChance += 0.3 * Modifier / 15.0;
                // For Legendary
                if (StrMid(NewItemName, StrLen(NewItemName) - 12, 9) == "Legendary")
                    Player.WeaponLegendaryChance = 0;
                else
                    Player.WeaponLegendaryChance += 0.2 * Modifier / 15.0;
            }
            // Set chances for armors and boots
            if (NewItemCategory == 3 || NewItemCategory == 9)
            {
                // For Assembled
                if (StrMid(NewItemName, StrLen(NewItemName) - 12, 9) == "Assembled")
                    Player.ArmorAssembledChance = 0;
                else
                    Player.ArmorAssembledChance += 50.0 * Modifier / 15.0;
                // For Exotic
                if (StrMid(NewItemName, StrLen(NewItemName) - 9, 6) == "Exotic")
                    Player.ArmorExoticChance = 0;
                else
                    Player.ArmorExoticChance += 15.0 * Modifier / 15.0;
                // For Superior
                if (StrMid(NewItemName, StrLen(NewItemName) - 11, 8) == "Superior")
                    Player.ArmorSuperiorChance = 0;
                else if (NewItemCategory == 3)
                    Player.ArmorSuperiorChance += 0.5 * Modifier / 15.0;
                // For Unique
                if (StrMid(NewItemName, StrLen(NewItemName) - 9, 6) == "Unique")
                    Player.ArmorUniqueChance = 0;
                else
                    Player.ArmorUniqueChance += 2.5 * Modifier / 15.0;
                // For Demonic
                if (StrMid(NewItemName, StrLen(NewItemName) - 10, 7) == "Demonic")
                    Player.ArmorDemonicChance = 0;
                else
                    Player.ArmorDemonicChance += 0.3 * Modifier / 15.0;
                // For Legendary
                if (StrMid(NewItemName, StrLen(NewItemName) - 12, 9) == "Legendary")
                    Player.ArmorLegendaryChance = 0;
                else
                    Player.ArmorLegendaryChance += 0.2 * Modifier / 15.0;
            }

            // Add to the spawned counter for Assembled/Exotic/Superior/Unique/Demonic/Legendary weapons and armor
            if ((NewItemCategory == 0 || NewItemCategory == 3 || NewItemCategory == 9) && StrMid(NewItemName, StrLen(NewItemName) - 9, 6) != "Common")
                for (int h = 0; h < ItemMax[NewItemCategory]; h++)
                    if (NewItemActor == ItemData[NewItemCategory][h].Actor)
                        ItemData[NewItemCategory][h].Spawned++;
        }

        // Reset the variables for next cycle
        NewItemSelected = false;
        NewItemIsAdditional = false;

        if (Random(0, MAX_DIFFICULTIES - Rarity) <= 0) Rarity--;
        if (Rarity < 0) Rarity = 0;
    }

    // DRLA Set shenanigans
    if (CompatMode == COMPAT_DRLA)
    {
        SetInfo const SetItems[MAX_DRLA_SETS] =
        {
            {
                "Death From Above", "RLDeathFromAboveSetBonusActive", false,
                {
                    { "RLAntigravSteelBoots",               20, },
                    { "RLAntigravProtectiveBoots",          15, },
                    { "RLAntigravPlasteelBoots",            10, },
                    { "RLJetpackArmor",                     20, },
                    { NULL, },
                },
            },
            {
                "Demonic", "RLDemonicSetBonusActive", false,
                {
                    { "RLDemonicCarapaceArmor",             20, },
                    { "RLDemonicBoots",                     10, },
                    { NULL, },
                },
            },
            {
                "Tactical", "RLTacticalSetBonusActive", false,
                {
                    { "RLTacticalArmor",                    20, },
                    { "RLTacticalBoots",                    10, },
                    { NULL, },
                },
            },
            {
                "Gothic", "RLGothicSetBonusActive", false,
                {
                    { "RLGothicArmor",                      20, },
                    { "RLGothicBoots",                      10, },
                    { NULL, },
                },
            },
            {
                "Phaseshift", "RLPhaseshiftSetBonusActive", false,
                {
                    { "RLPhaseshiftArmor",                  20, },
                    { "RLPhaseshiftBoots",                  10, },
                    { NULL, },
                },
            },
            {
                "Roysten", "RLRoystenSetBonusActive", false,
                {
                    { "RLRoystensCommandArmor",             15, },
                    { "RLRoystensCombatBoots",               5, },
                    { NULL, },
                },
            },
            {
                "Architect", "RLArchitectSetBonusActive", false,
                {
                    { "RLSoloOperativeSuitArmor",           15, },
                    { "RLSoloOperativeBoots",                5, },
                    { NULL, },
                },
            },
            {
                "Torgue", "RLTorgueSetBonusActive", false,
                {
                    { "RLTorgueBlastplateArmor",            15, },
                    { "RLTorgueBlastBoots",                  5, },
                    { NULL, },
                },
            },
            {
                "Enclave", "RLEnclaveSetBonusActive", false,
                {
                    { "RLNuclearPowerArmor",                15, },
                    { "RLNuclearPowerBoots",                 5, },
                    { NULL, },
                },
            },
            {
                "Sentry Sentinel", "RLSentrySentinelSetBonusActive", false,
                {
                    { "RLTacticalAssemblerSuitArmor",       15, },
                    { "RLTacticalAssemblerBoots",            5, },
                    { NULL, },
                },
            },
            {
                "Sensible Strategist", "RLSensibleStrategistSetBonusActive", false,
                {
                    { "RLTacticalAssemblerSuitArmor",       15, },
                    { "RLTacticalAssemblerBoots",            5, },
                    { NULL, },
                },
            },
            {
                "Lava", "RLLavaSetBonusActive", false,
                {
                    { "RLLavaArmor",                        10, },
                    { "RLLavaBoots",                         5, },
                    { NULL, },
                },
            },
            {
                "Inquisitor", "RLInquisitorsSetBonusActive", false,
                {
                    { "RLMaleksArmor",                      10, },
                    { "RLNyarlaptotepsBoots",                5, },
                    { NULL, },
                },
            },
            {
                "Cerberus", "RLCerberusSetBonusActive", false,
                {
                    { "RLTristarBlaster",                   15, },
                    { "RLCerberusArmor",                    15, },
                    { "RLCerberusBoots",                     5, },
                    { NULL, },
                },
            },
            {
                "Angelic", "RLAngelicAttireSetBonusActive", false,
                {
                    { "RLAngelicArmor",                      5, },
                    { "RLAngelicBoots",                      5, },
                    { NULL, },
                },
            },
            {
                "Nuclear", "RLNuclearWeaponSetBonusActive", false,
                {
                    { "RLNuclearPlasmaPistol",              15, },
                    { "RLNuclearPlasmaRifle",               10, },
                    { "RLNuclearBFG9000",                    5, },
                    { "RLNuclearOnslaught",                0.2, },
                    { "RLNuclearArmor",                     10, },
                    { "RLNuclearPowerArmor",                 2, },
                    { "RLHighPowerNuclearPlasmaPistol",      0, },
                    { "RLStormNuclearPlasmaPistol",          0, },
                    { "RLSuperchargedNuclearPlasmaPistol",   0, },
                    { "RLNuclearPlasmaShotgun",              0, },
                    { "RLNuclearPlasmaRifleMkII",            0, },
                    { "RLAssaultRifleNuclearPlasmaRifle",    0, },
                    { "RLBurstCannonNuclearPlasmaRifle",     0, },
                    { "RLHighPowerNuclearPlasmaRifle",       0, },
                    { "RLHighPowerNuclearBFG9000",           0, },
                    { "RLNuclearVBFG9000",                   0, },
                    { "RLNuclearBiggestFuckingGun",          0, },
                    { "RLNuclearPlasmaRevolver",             0, },
                    { "RLNuclearOnslaught",                  0, },
                    { NULL, },
                },
            },
            {
                "Alucard", "RLAntiFreakJackalDemonArtifacts", false,
                {
                    { "RLAntiFreakJackal",                   5, },
                    { "RLHellsingARMSCasull",                5, },
                    { NULL, },
                },
            },
            {
                "Security Officer", "RLMarathonShotgunsDemonArtifacts", true,
                {
                    { "RLMarathonShotguns",                  5, },
                    { NULL, },
                },
            },
        };

        int SetAmount = Amount;

        if (DebugLog)
            Log("\CdDEBUG: \C-Checking DRLA sets...");

        for (int i = 0; i < MAX_DRLA_SETS; i++)
        {
            if (DebugLog)
                Log("\CdDEBUG: \C-Set \Cd%d\C-: \Cd%S", i + 1, SetItems[i].Name);

            // Skip this set if you already have it
            if (CheckInventory(SetItems[i].Token)) continue;

            bool HaveItem = false;
            for (int j = 0; SetItems[i].Items[j].Name != NULL; j++)
            {
                if (DebugLog)
                    Log("\CdDEBUG: \C-Set \Cd%d \C-Item \Cd%d\C-: \Cd%S", i + 1, j + 1, SetItems[i].Items[j].Name);

                if (CheckInventory(SetItems[i].Items[j].Name) || CheckInventory(StrParam("%SToken", SetItems[i].Items[j].Name)))
                {
                    HaveItem = true;
                    if (DebugLog)
                        Log("\CdDEBUG: \C-Have this item, continue...");
                    break;
                }
            }

            if (HaveItem)
            {
                for (int j = 0; SetItems[i].Items[j].Name != NULL; j++)
                {
                    str PickupItemName = StrParam("%SPickup", SetItems[i].Items[j].Name);

                    // Get info for new item
                    NewItem = FindItem(PickupItemName);
                    NewItemName = NewItem->Name;
                    NewItemActor = NewItem->Actor;
                    NewItemIndex = NewItem->Index;
                    NewItemCategory = NewItem->Category;
                    NewItemSpawned = NewItem->Spawned;
                    NewItemPenalty = 0;

                    // Set penalty for an item already spawned
                    if (NewItemSpawned < 5)
                        NewItemPenalty = NewItemSpawned;
                    else
                        NewItemPenalty = 5;

                    fixed Chance = SetItems[i].Items[j].Chance * (1.0 + (Crates[ID].Rarity * 0.125));
                    fixed Pick = RandomFixed(0.0, 100.0);

                    // Skip this item if you have it and it's not a dual/synthfire set
                    if (!SetItems[i].Synthfire && CheckInventory(SetItems[i].Items[j].Name) || CheckInventory(StrParam("%SToken", SetItems[i].Items[j].Name))) continue;

                    // Stop checking items if we've filled the crate
                    if (SetAmount >= CRATE_MAX_ITEMS)
                    {
                        if (DebugLog)
                            Log("\CdDEBUG: \C-Crate is full! Stopping set checks");
                        return;
                    }

                    if (DebugLog)
                        Log("\CdDEBUG: \C-Set \Cd%d \C-Item \Cd%d\C-: \Cd%S \C-(Chance: \Cf%.2k%% \C-/ Pick: \Cf%.2k%%\C-)", i + 1, j + 1, SetItems[i].Items[j].Name, Chance, Pick);

                    if (Pick <= Chance && Random(0, NewItemPenalty * (200 - NewItemPenalty * 20)) <= 50)
                    {
                        // Couldn't find the item
                        if (NewItem == GetBlankItem()) continue;

                        Crates[ID].Active[SetAmount] = true;
                        Crates[ID].Item[SetAmount] = NewItem;

                        if (DebugLog)
                            Log("\CdDEBUG: \CfSet Item Spawned!");

                        SetAmount++;

                        // Add to the spawned counter for Assembled/Exotic/Superior/Unique/Demonic/Legendary weapons and armor
                        if ((NewItemCategory == 0 || NewItemCategory == 3 || NewItemCategory == 9) && StrMid(NewItemName, StrLen(NewItemName) - 9, 6) != "Common")
                            for (int h = 0; h < ItemMax[NewItemCategory]; h++)
                                if (NewItemActor == ItemData[NewItemCategory][h].Actor)
                                    ItemData[NewItemCategory][h].Spawned++;
                    }
                }
            }
        }
    }
}

void CrateTakeItem()
{
    str ActorToSpawn = Crates[Player.CrateID].Item[Player.CrateIndex]->Actor;

    // Compatibility Handling - DoomRL Arsenal
    // Set actors
    if (CompatMode == COMPAT_DRLA)
    {
        str ItemName = Crates[Player.CrateID].Item[Player.CrateIndex]->Name;
        int ItemCategory = Crates[Player.CrateID].Item[Player.CrateIndex]->Category;

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

    bool Spawned = SpawnForced(ActorToSpawn, GetActorX(0), GetActorY(0), GetActorZ(0), 0, 0);

    if (Spawned)
    {
        SetActorVelocity(Player.TID, 0.01, 0.01, 0, true, false);

        Crates[Player.CrateID].Active[Player.CrateIndex] = false;
        Crates[Player.CrateID].Item[Player.CrateIndex] = NULL;
    }
}

void CrateTakeAll()
{
    // Save player's index inside the crate so it doesn't get flung around by the code below.
    int Index = Player.CrateIndex;

    for (Player.CrateIndex = 0; Player.CrateIndex < Crates[Player.CrateID].Amount; Player.CrateIndex++)
    {
        str ActorToSpawn = Crates[Player.CrateID].Item[Player.CrateIndex]->Actor;

        // Compatibility Handling - DoomRL Arsenal
        // Set actors
        if (CompatMode == COMPAT_DRLA)
        {
            str ItemName = Crates[Player.CrateID].Item[Player.CrateIndex]->Name;
            int ItemCategory = Crates[Player.CrateID].Item[Player.CrateIndex]->Category;

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

        bool Spawned = SpawnForced(ActorToSpawn, GetActorX(0), GetActorY(0), GetActorZ(0), 0, 0);

        if (Spawned)
        {
            SetActorVelocity(Player.TID, 0.01, 0.01, 0, true, false);

            Crates[Player.CrateID].Active[Player.CrateIndex] = false;
            Crates[Player.CrateID].Item[Player.CrateIndex] = NULL;
        }
    }

    // Restore index.
    Player.CrateIndex = Index;
}

bool CrateEmpty(int ID)
{
    for (int i = 0; i < CRATE_MAX_ITEMS; i++)
        if (Crates[ID].Active[i])
            return false;

    return true;
}

// TODO: Tried to make this a more generic ItemSpace CrateInfo *FindCrate, but DH-ACC bitched, yell at David
// TODO: Actually fix this for GDCC
bool CrateEmptyTID(int TID)
{
    for (int i = 0; i < CrateID; i++)
        if (Crates[i].TID == TID)
            return Crates[i].Empty;

    return false;
}

void DrawCrate(fixed X, fixed Y)
{
    // Width/Height
    int Width = 9;
    int Height = 6;

    // Coordinates
    fixed BaseX;
    fixed BaseY = Y;
    fixed BoxX;
    fixed BoxY;
    fixed X1;
    fixed Y1;
    fixed IconX;
    fixed IconY;

    // Centre-ish
    X -= 10.0;

    // Title
    SetFont("BIGFONT");
    HudMessage("UAC Supply Crate (%S\C-)", CrateRarityNames[Crates[Player.CrateID].Rarity]);
    EndHudMessage(HUDMSG_PLAIN, 0, "Green", X + (470/2), Y + 24.0, 0.05);

    for (int i = 0; i < Height; i++)
    {
        // Reset base X
        BaseX = X;

        for (int j = 0; j < Width; j++)
        {
            int Index = j + (i * Width);

            ItemInfoPtr Item = Crates[Player.CrateID].Item[Index];
            bool Active = Crates[Player.CrateID].Active[Index];
            str Icon = Item->Sprite.Name;
            str Name = Item->Name;
            int IconXOff = Item->Sprite.XOff;
            int IconYOff = Item->Sprite.YOff;

            // Setup X and Y
            X1 = 26.0 + BaseX;
            Y1 = 50.0 + BaseY;
            BoxX = 48.0 + BaseX;
            BoxY = 72.0 + BaseY;
            IconX = 48.0 + BaseX;
            IconY = 72.0 + BaseY;
            IconX += IconXOff;
            IconY += IconYOff;

            // Icon
            SetHudClipRect(X1, Y1, 44, 44);
            if (Active)
                if (Player.CrateIndex == Index)
                    PrintSpritePulse(Icon, 0, IconX, IconY, 0.75, 32.0, 0.25);
                else
                    PrintSprite(Icon, 0, IconX, IconY, 0.05);
            SetHudClipRect(0, 0, 0, 0);

            // Box
            if (Player.CrateIndex == Index)
            {
                if (Active)
                    PrintSprite("ItemBoxH", 0, BoxX, BoxY, 0.05);
                else
                    PrintSprite("ItemBoxR", 0, BoxX, BoxY, 0.05);
            }
            PrintSprite("ItemBoxB", 0, BoxX, BoxY, 0.05);

            // Item Name
            if (Player.CrateIndex == Index && Active)
            {
                SetFont("BIGFONT");
                HudMessage("%S", Name);
                EndHudMessage(HUDMSG_PLAIN, 0, "White", X + 24.1, Y + 350.1, 0.05);
            }

            // Increment X
            BaseX += 48.0;
        }

        // Increment Y
        BaseY += 48.0;
    }
}

void CrateInput()
{
    if (CheckInput(BT_USE, KEY_PRESSED, false, PlayerNumber()))
    {
        ActivatorSound("menu/move", 127);
        CrateTakeItem();
    }
    if (CheckInput(BT_SPEED, KEY_HELD, false, PlayerNumber()))
    {
        if (CheckInput(BT_USE, KEY_PRESSED, false, PlayerNumber()))
        {
            ActivatorSound("transfer/complete", 127);
            CrateTakeAll();
        }
    }
    if (CheckInput(BT_FORWARD, KEY_PRESSED, false, PlayerNumber()))
    {
        ActivatorSound("menu/move", 127);
        Player.CrateIndex -= 9;
        if (Player.CrateIndex < 0) Player.CrateIndex = 0;
    }
    if (CheckInput(BT_BACK, KEY_PRESSED, false, PlayerNumber()))
    {
        ActivatorSound("menu/move", 127);
        Player.CrateIndex += 9;
        if (Player.CrateIndex > CRATE_MAX_ITEMS - 1) Player.CrateIndex = CRATE_MAX_ITEMS - 1;
    }
    if (CheckInput(BT_MOVELEFT, KEY_PRESSED, false, PlayerNumber()))
    {
        ActivatorSound("menu/move", 127);
        Player.CrateIndex--;
        if (Player.CrateIndex < 0) Player.CrateIndex = 0;
    }
    if (CheckInput(BT_MOVERIGHT, KEY_PRESSED, false, PlayerNumber()))
    {
        ActivatorSound("menu/move", 127);
        Player.CrateIndex++;
        if (Player.CrateIndex > CRATE_MAX_ITEMS - 1) Player.CrateIndex = CRATE_MAX_ITEMS - 1;
    }
}

NamedScript Console void DumpCrate()
{
    for (int i = 0; i < CrateID; i++)
    {
        Log("\Cd  ===== Crate %d =====", i);
        Log("Amount: %d", Crates[i].Amount);
        Log("Rarity: %d", Crates[i].Rarity);

        for (int j = 0; j < CRATE_MAX_ITEMS; j++)
        {
            if (!Crates[i].Active[j]) continue;

            Log("%d. %S (%S)", j, Crates[i].Item[j]->Name, Crates[i].Item[j]->Actor);
        }
    }
}
