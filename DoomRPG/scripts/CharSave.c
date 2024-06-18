#include <stdlib.h>
#include <string.h>

#include "Defs.h"

#include "Arena.h"
#include "Augs.h"
#include "CharSave.h"
#include "ItemData.h"
#include "Map.h"
#include "RPG.h"
#include "Shield.h"
#include "Shop.h"
#include "Skills.h"
#include "Stats.h"
#include "Utils.h"

/*

    --- Character Save Format ---

    GENERAL NOTES
    - Items such as Shield Parts, Aug canisters/upgrades and Stim injectors will be stored in the Locker for retrieval instead of in the save data itself
    - Each byte will be stored with a leading 0 if necessary, so a level of 10 and a rank level of 5 will come out as 0A05

    Version / Compatibility Flag
        - Version is referenced from defs, stored in 2 bytes
        - Mirror of the CompatMode static, stored in 1 byte
    Level / Rank Level
        - Only Level and Rank Level are stored, the XP to give is determined from these
        - Both Level and Rank Level are stored in 1 byte each
    Stats
        - Each is stored in 1 byte since they will never go above 200 by normal means
          [KS] They can be negative but we can't handle that so just ignore it in that case
    Skills
        - Each skill level is stored in 1 byte
    Skill Wheel
        - 16 bytes total
        - Each slot is 2 bytes
        - First byte is the category, second byte is the index
        - Both bytes being 255 (FFFF) means it is blank
    Augmentations
        - Each augmentation's level is stored in 1 byte
    Stims
        - Each stim vial type is stored in 4 bytes
    Turret
        - Each turret upgrade is stored in 1 byte
    Misc
        - Credits
          - Stored in 4 bytes
        - Modules
          - Stored in 4 bytes
        - Medkit
          - Stored in 2 bytes
        - Chips
          - Gold Chip stored in 2 bytes
          - Platinum Chip stored in 2 bytes
        - UAC Card
          - Stored in 1 byte
        - Augmentation Battery
          - Stored in 2 byte
        - Toxicity
          - Stored in 1 byte
        - Arena Wave
          - Stored in 2 bytes
    Locker
        - Each item's count will be stored in 4 bytes
        - Each weapon's associated DRLA Modpacks will be stored in 1 byte after the amount
    Shop Auto Mode
        - Each item's auto mode is stored in 1 byte

    ----- COMPATIBILITY EXTENSIONS -----

    DRLA Tokens
        - Each token is stored in 1 byte

    Checksum
        - Checksum is seperated out at the end via a |
        - Checksum is determined by XOR'ing the value of each entry into a total buffer and doing the following:
          (Total * 8) / 2
          [KS] I removed the LShift because that only leaves us with 24 bits for comparison. Instead, we XOR (^) the values together.

*/

bool Depositing = false;
//bool LoadedData = false;
int DepositItems = 0;
int DepositTotal = 0;
CharSaveInfo Info;

str DRLATokens[DRLA_MAX_TOKENS] =
{
    // Basic Assembly Learnt Tokens
    "RLChainswordAssemblyLearntToken",
    "RLPiercingBladeAssemblyLearntToken",
    "RLSilencedPistolAssemblyLearntToken",
    "RLShredderPistolAssemblyLearntToken",
    "RLSpeedloaderPistolAssemblyLearntToken",
    "RLHighPowerWeaponAssemblyLearntToken",
    "RLElephantGunAssemblyLearntToken",
    "RLHeavyShotgunAssemblyLearntToken",
    "RLPlasmaticShrapnelAssemblyLearntToken",
    "RLTacticalShotgunAssemblyLearntToken",
    "RLGatlingGunAssemblyLearntToken",
    "RLMicroLauncherAssemblyLearntToken",
    "RLFlamethrowerAssemblyLearntToken",
    "RLShrapnelCannonAssemblyLearntToken",
    "RLSniperRifleAssemblyLearntToken",

    // Advanced Assembly Learnt Tokens
    "RLDoubleChainsawAssemblyLearntToken",
    "RLStormPistolAssemblyLearntToken",
    "RLPlasmaPistolAssemblyLearntToken",
    "RLAutoshotgunAssemblyLearntToken",
    "RLNanoShrapnelAssemblyLearntToken",
    "RLFocusedDoubleShotgunAssemblyLearntToken",
    "RLBulletstormChaingunAssemblyLearntToken",
    "RLAssaultRifleAssemblyLearntToken",
    "RLBurstCannonAssemblyLearntToken",
    "RLTacticalRocketLauncherAssemblyLearntToken",
    "RLGrenadeLauncherAssemblyLearntToken",
    "RLHyperblasterAssemblyLearntToken",
    "RLPlasmaRifleMkIIAssemblyLearntToken",
    "RLVBFG9000AssemblyLearntToken",
    "RLMiniMissilePistolAssemblyLearntToken",
    "RLNuclearPlasmaShotgunAssemblyLearntToken",

    // Master Assembly Learnt Tokens
    "RLRipperAssemblyLearntToken",
    "RLDemolitionAmmoAssemblyLearntToken",
    "RLNanomanufactureAmmoAssemblyLearntToken",
    "RLMotherInLawAssemblyLearntToken",
    "RLPlasmaRefractorAssemblyLearntToken",
    "RLBiggestFuckingGunAssemblyLearntToken",
    "RLAutocannonAssemblyLearntToken",
    "RLMegatonShotgunAssemblyLearntToken",
    "RLVoltgunAssemblyLearntToken",
    "RLConquerorShotgunAssemblyLearntToken",
    "RLClusterBombLauncherAssemblyLearntToken",
    "RLGaussRifleAssemblyLearntToken",
    "RLAntiMaterielRifleAssemblyLearntToken",
    "RLZeusCannonAssemblyLearntToken",
    "RLSuperMachinegunAssemblyLearntToken",
    "RLPlasmaInfusionAssemblyLearntToken",

    // Unique Exotic Mods Effects Learnt Tokens
    "RLJackhammerNanoLearntToken",
    "RLJackhammerFirestormLearntToken",
    "RLJackhammerSniperLearntToken",
    "RLRailgunNanoLearntToken",
    "RLRailgunFirestormLearntToken",
    "RLRailgunSniperLearntToken",
    "RLMysteriousMagnumNanoLearntToken",
    "RLMysteriousMagnumFirestormLearntToken",
    "RLMysteriousMagnumSniperLearntToken",
    "RLBFG10KNanoLearntToken",
    "RLBFG10KFirestormLearntToken",
    "RLBFG10KSniperLearntToken",
    "RLUnknownHeraldNanoLearntToken",
    "RLUnknownHeraldFirestormLearntToken",
    "RLUnknownHeraldSniperLearntToken",
    "RLFragShotgunNanoLearntToken",
    "RLFragShotgunFirestormLearntToken",
    "RLFragShotgunSniperLearntToken",
    "RLQuadShotgunNanoLearntToken",
    "RLQuadShotgunFirestormLearntToken",
    "RLQuadShotgunSniperLearntToken",
    "RLLightweaverNanoLearntToken"
    "RLLightweaverFirestormLearntToken",
    "RLLightweaverSniperLearntToken",
    "RLTrigunNanoLearntToken",
    "RLTrigunFirestormLearntToken",
    "RLTrigunSniperLearntToken",
    "RLGrammatonClericBerettaNanoLearntToken",
    "RLGrammatonClericBerettaFirestormLearntToken",
    "RLGrammatonClericBerettaSniperLearntToken",
    "RLAntiFreakJackalNanoLearntToken",
    "RLAntiFreakJackalFirestormLearntToken",
    "RLAntiFreakJackalSniperLearntToken",
    "RLHellsingARMSCasullNanoLearntToken",
    "RLHellsingARMSCasullFirestormLearntToken",
    "RLHellsingARMSCasullSniperLearntToken",
    "RLTantrumCannonNanoLearntToken",
    "RLTantrumCannonFirestormLearntToken",
    "RLTantrumCannonSniperLearntToken",
    "RLRevenantsLauncherNanoLearntToken",
    "RLRevenantsLauncherFirestormLearntToken",
    "RLRevenantsLauncherSniperLearntToken",
    "RLNullPointerNanoLearntToken",
    "RLNullPointerFirestormLearntToken",
    "RLNullPointerSniperLearntToken",
    "RLParticleBeamCannonNanoLearntToken",
    "RLParticleBeamCannonFirestormLearntToken",
    "RLParticleBeamCannonSniperLearntToken",
    "RLPlasmaRedirectionCannonNanoLearntToken",
    "RLPlasmaRedirectionCannonFirestormLearntToken",
    "RLPlasmaRedirectionCannonSniperLearntToken",
    "RLSteelBeastNanoLearntToken",
    "RLSteelBeastFirestormLearntToken",
    "RLSteelBeastSniperLearntToken",
    "RLSussGunNanoLearntToken",
    "RLSussGunFirestormLearntToken",
    "RLSussGunSniperLearntToken",
    "RLMIRVLauncherNanoLearntToken",
    "RLMIRVLauncherFirestormLearntToken",
    "RLMIRVLauncherSniperLearntToken",
    "RLSubtleKnifeNanoLearntToken",
    "RLSubtleKnifeFirestormLearntToken",
    "RLSubtleKnifeSniperLearntToken",
    "RLNuclearOnslaughtNanoLearntToken",
    "RLNuclearOnslaughtFirestormLearntToken",
    "RLNuclearOnslaughtSniperLearntToken",
    "RLSpectreRevolverNanoLearntToken",
    "RLSpectreRevolverFirestormLearntToken",
    "RLSpectreRevolverSniperLearntToken",
    "RLMarathonAssaultRifleNanoLearntToken",
    "RLMarathonAssaultRifleFirestormLearntToken",
    "RLMarathonAssaultRifleSniperLearntToken",
    "RLMarathonShotgunsNanoLearntToken",
    "RLMarathonShotgunsFirestormLearntToken",
    "RLMarathonShotgunsSniperLearntToken",
    "RLChameleonRifleNanoLearntToken",
    "RLChameleonRifleFirestormLearntToken",
    "RLChameleonRifleSniperLearntToken",
    "RLWidowmakerSMGNanoLearntToken",
    "RLWidowmakerSMGFirestormLearntToken",
    "RLWidowmakerSMGSniperLearntToken",
    "RLLuciferCannonNanoLearntToken",
    "RLLuciferCannonFirestormLearntToken",
    "RLLuciferCannonSniperLearntToken",
    "RLDirectHitNanoLearntToken",
    "RLDirectHitFirestormLearntToken",
    "RLDirectHitSniperLearntToken",
    "RLTriadCannonNanoLearntToken",
    "RLTriadCannonFirestormLearntToken",
    "RLTriadCannonSniperLearntToken",

    // Demonic Exotic Mods Effects Learnt Tokens
    "RLDeathsGazeNanoLearntToken",
    "RLDeathsGazeFirestormLearntToken",
    "RLDeathsGazeSniperLearntToken",
    "RLHellsReignNanoLearntToken",
    "RLHellsReignFirestormLearntToken",
    "RLHellsReignSniperLearntToken",
    "RLSoulstormRifleNanoLearntToken",
    "RLSoulstormRifleFirestormLearntToken"
    "RLSoulstormRifleSniperLearntToken",
    "RLMortalyzerNanoLearntToken",
    "RLMortalyzerFirestormLearntToken",
    "RLMortalyzerSniperLearntToken",
    "RLDreadshotMortarNanoLearntToken",
    "RLDreadshotMortarFirestormLearntToken",
    "RLDreadshotMortarSniperLearntToken",

    // Legendary Exotic Mods Effects Learnt Tokens
    "RLLonghornNanoLearntToken"
    "RLLonghornFirestormLearntToken"
    "RLLonghornSniperLearntToken"
    "RLJudgeOfTheDeadNanoLearntToken",
    "RLJudgeOfTheDeadFirestormLearntToken",
    "RLJudgeOfTheDeadSniperLearntToken",
    "RLHurricaneCannonNanoLearntToken",
    "RLHurricaneCannonFirestormLearntToken",
    "RLHurricaneCannonSniperLearntToken",
    "RLNeuralStunnerNanoLearntToken",
    "RLNeuralStunnerFirestormLearntToken",
    "RLNeuralStunnerSniperLearntToken",
    "RLBaronBlasterNanoLearntToken",
    "RLBaronBlasterFirestormLearntToken",
    "RLBaronBlasterSniperLearntToken",
    "RLRealityDistortionArrayNanoLearntToken",
    "RLRealityDistortionArrayFirestormLearntToken",
    "RLRealityDistortionArraySniperLearntToken",
    "RLBFGInfinityNanoLearntToken",
    "RLBFGInfinityFirestormLearntToken",
    "RLBFGInfinitySniperLearntToken",
    "RLDuke2RifleNanoLearntToken",
    "RLDuke2RifleFirestormLearntToken",
    "RLDuke2RifleSniperLearntToken"
};

NamedScript MenuEntry void SaveCharacter()
{
    char const *SaveString;
    char *EncodedSaveString;
    //CharSaveInfo Info;

    // You need to be in the Outpost to do this
    if (!CurrentLevel->UACBase && !DebugLog)
    {
        PrintError("\CgERROR: \CjYou must be in the Outpost to save and deposit your equipment");
        ActivatorSound("menu/error", 127);
        return;
    }

    // Disable Augmentations
    for (int i = 0; i < AUG_MAX; i++)
    {
        Player.Augs.Active[i] = false;
        Player.Augs.SlotsUsed = 0;
    }

    Delay(1);

    // Disable Shield
    DeactivateShield();

    // Bulk Deposit
    DepositInventory();

    // Compatibility Handling - DoomRL Extended
    // Deposit Items for Phase Sisters
    if (CompatModeEx == COMPAT_DRLAX && PlayerClass(PlayerNumber()) == 9) // Phase Sisters
    {
        while (Depositing)
        {
            SetFont("BIGFONT");
            if (CheckInventory("RLPhaseSistersSwapToken") == 1)
                HudMessage("\CtPortia:\C- \CkDepositing Items: \Cd%d \C-/ \Cd%d", DepositItems, DepositTotal);
            else
                HudMessage("\CvTerri:\C- \CkDepositing Items: \Cd%d \C-/ \Cd%d", DepositItems, DepositTotal);
            EndHudMessage(HUDMSG_FADEOUT, DEPOSIT_ID, "White", 0.5, 0.5, 0.05, 0.5);
            Delay(1);
        }

        // Swap Sisters
        SetInventory("RLPhaseSistersHomingTrigger", 1);

        Delay(1);

        // Disable Shield
        DeactivateShield();

        // Bulk Deposit
        DepositInventory();

        while (Depositing)
        {
            SetFont("BIGFONT");
            if (CheckInventory("RLPhaseSistersSwapToken") == 1)
                HudMessage("\CtPortia:\C- \CkDepositing Items: \Cd%d \C-/ \Cd%d", DepositItems, DepositTotal);
            else
                HudMessage("\CvTerri:\C- \CkDepositing Items: \Cd%d \C-/ \Cd%d", DepositItems, DepositTotal);
            EndHudMessage(HUDMSG_FADEOUT, DEPOSIT_ID, "White", 0.5, 0.5, 0.05, 0.5);
            Delay(1);
        }
    }
    else
    {
        while (Depositing)
        {
            SetFont("BIGFONT");
            HudMessage("\CkDepositing Items: \Cd%d \C-/ \Cd%d", DepositItems, DepositTotal);
            EndHudMessage(HUDMSG_FADEOUT, DEPOSIT_ID, "White", 0.5, 0.5, 0.05, 0.5);
            Delay(1);
        }
    }

    SetFont("BIGFONT");
    HudMessage("===== Save In Progress =====");
    EndHudMessage(HUDMSG_PLAIN | HUDMSG_LOG, 70, "Yellow", 0.5, 0.3, 0);
    SetFont("SMALLFONT");
    HudMessage("Do not quit the game or power off your console.");
    EndHudMessage(HUDMSG_PLAIN | HUDMSG_LOG, 71, "Orange", 0.5, 0.35, 0);

    ClearInfo(&Info);

    // Populate the data
    PopulateCharData(&Info);
    SaveString = MakeSaveString(&Info);
    EncodedSaveString = calloc(strlen(SaveString) + 1, sizeof(char));

    EncodeRLE(SaveString, EncodedSaveString);

    //EncodedSaveString = realloc(EncodedSaveString, strlen(EncodedSaveString) + 1);
    if (DebugLog)
        Log("\CdDEBUG: \C-Encoded Save Data: %s", EncodedSaveString);

    int EncStrSize = strlen(EncodedSaveString);
    int PartialStringsNeeded = EncStrSize / CHARSAVE_MAXSIZE;
    if (EncStrSize % CHARSAVE_MAXSIZE > 0)
        PartialStringsNeeded++;
    char *PartialSaveString = calloc(CHARSAVE_MAXSIZE + 1, sizeof(char));
    PartialSaveString[CHARSAVE_MAXSIZE] = 0;

    for (int i = 0; i < CHARSAVE_MAXCVARS; i++)
        SetActivatorCVarString(StrParam("drpg_char_data_%d", i), "");

    bool Success = true;

    if (PartialStringsNeeded > CHARSAVE_MAXCVARS)
        Success = false;
    if (Success && !SetActivatorCVar("drpg_char_data_len", PartialStringsNeeded))
        Success = false;

    // Save data
    for (int i = 0; Success && i < PartialStringsNeeded; i++)
    {
        strncpy(PartialSaveString, EncodedSaveString + (CHARSAVE_MAXSIZE * i), CHARSAVE_MAXSIZE - 1);

        Delay(1);
        //if (DebugLog)
        //  Log("Writing Save Data: %s", PartialSaveString);
        if (!SetActivatorCVarString(StrParam("drpg_char_data_%d", i), StrParam("%s", PartialSaveString)))
            Success = false;
    }

    free((void *)SaveString);
    free((void *)EncodedSaveString);
    free((void *)PartialSaveString);

    // Compatibility Handling - DoomRL Arsenal Monsters
    // ------ Danger Level ------
    if (Success && CompatMonMode == COMPAT_DRLA && GetActivatorCVar("drpg_char_load_rl_dangerlevel"))
    {
        int rlmDL = CheckInventory("RLDangerLevel");

        SetActivatorCVar("drpg_char_data_rl_dangerlevel", rlmDL);

        if (DebugLog)
            Log("\CdDEBUG: \C-Saved Char RL Danger Level: %i", rlmDL);
    }

    // ------ Save Level Data ------
    if (Success && GetActivatorCVar("drpg_char_load_maplevel"))
    {
        LevelInfo *LastLevelData = klArrayUtils(2, CurrentWAD, GetKnownLevelCount(CurrentWAD) - 1);

        int CW = CurrentWAD;
        str LL = LastLevelData->LumpName;
        int LN = LastLevelData->LevelNum;

        SetActivatorCVarString("drpg_char_data_level", StrParam("!CW%i!LL%S!LN%i", CW, LL, LN));

        if (DebugLog)
            Log("\CdDEBUG: \C-Saved Char Level Data; CW: %i, LL: %S, LN: %i", CW, LL, LN);
    }

    if (Success)
    {
        // Saving Successful
        ActivatorSound("charsave/save", 127);
        SetFont("BIGFONT");
        HudMessage("===== Character Saved =====");
        EndHudMessage(HUDMSG_FADEOUT | HUDMSG_LOG, 70, "Green", 0.5, 0.3, 3.0, 2.0);
        HudMessage("");
        EndHudMessage(HUDMSG_FADEOUT, 71, "Orange", 0.5, 0.35, 3.0, 2.0);
    }
    else
    {
        // Saving Failed
        ActivatorSound("charsave/fail", 127);
        SetFont("BIGFONT");
        HudMessage("===== Character Save Failed =====");
        EndHudMessage(HUDMSG_FADEOUT | HUDMSG_LOG, 70, "Red", 0.5, 0.3, 5.0, 2.0);
        SetFont("SMALLFONT");
        HudMessage("A CVar could not be written to.");
        EndHudMessage(HUDMSG_FADEOUT | HUDMSG_LOG, 71, "Orange", 0.5, 0.35, 5.0, 2.0);
        HudMessage("Perhaps your save is too large?");
        EndHudMessage(HUDMSG_FADEOUT | HUDMSG_LOG, 72, "Orange", 0.5, 0.4, 5.0, 2.0);
        HudMessage("Post your ini if you see this.");
        EndHudMessage(HUDMSG_FADEOUT | HUDMSG_LOG, 73, "Orange", 0.5, 0.45, 5.0, 2.0);
    }
}

NamedScript MenuEntry void LoadCharacter()
{
    char *EncodedSaveString;
    char *SaveString;
    //CharSaveInfo Info;

    int NumCVars = GetActivatorCVar("drpg_char_data_len");
    if (NumCVars <= 0)
    {
        ActivatorSound("charsave/fail", 127);
        SetFont("BIGFONT");
        HudMessage("===== No Character Data =====");
        EndHudMessage(HUDMSG_FADEOUT | HUDMSG_LOG, 70, "Red", 0.5, 0.3, 3.0, 2.0);
        return;
    }

    SetFont("BIGFONT");
    HudMessage("===== Load In Progress =====");
    EndHudMessage(HUDMSG_PLAIN | HUDMSG_LOG, 70, "Yellow", 0.5, 0.3, 0);
    SetFont("SMALLFONT");
    HudMessage("Make sure to withdraw your items from the shop!");
    EndHudMessage(HUDMSG_PLAIN, 71, "Orange", 0.5, 0.35, 0);

    EncodedSaveString = calloc(4000, sizeof(char));
    EncodedSaveString[0] = '\x00';

    for (int i = 0; i < NumCVars; i++)
    {
        char *tmp = StringToCharP(GetActivatorCVarString(StrParam("drpg_char_data_%d", i)));
        strcat(EncodedSaveString, tmp);
        free((void *)tmp);
    }

    //EncodedSaveString = realloc(EncodedSaveString, strlen(EncodedSaveString) + 1);
    if (DebugLog)
        Log("\CdDEBUG: \C-Load Data (Encoded): %S", EncodedSaveString);

    SaveString = calloc(25000, sizeof(char));

    DecodeRLE(EncodedSaveString, SaveString);

    //SaveString = realloc(SaveString, strlen(SaveString) + 1);
    //if (DebugLog)
    //    Log("Load Data (Raw): %s", SaveString);
    free((void *)EncodedSaveString);

    ClearInfo(&Info);
    LoadCharDataFromString(&Info, SaveString);

    /*while(!LoadedData)
        Delay(1);
    */
    free((void *)SaveString);

    // Version / Compatibility Flag
    if (DebugLog)
        Log("\CdDEBUG: \C-Version: %d", Info.Version);

    if (Info.Version < 0)
    {
        str const ReasonStrings[3] =
        {
            "You loaded this with different mods!",     // 1: Wrong CompatMode
            "Your save version is outdated.",           // 2: SaveVersion is too old
            "Your save was corrupted."                  // 3: Bad checksum or something else went wrong
        };
        int Reason = -(Info.Version) - 1;

        ActivatorSound("charsave/fail", 127);
        SetFont("BIGFONT");
        HudMessage("===== Character Load Failed =====");
        EndHudMessage(HUDMSG_FADEOUT | HUDMSG_LOG, 70, "Red", 0.5, 0.3, 3.0, 2.0);
        SetFont("SMALLFONT");
        HudMessage("%S (Error %d)", ReasonStrings[Reason], Reason + 1);
        EndHudMessage(HUDMSG_FADEOUT | HUDMSG_LOG, 71, "Orange", 0.5, 0.35, 3.0, 2.0);
        return;
    }

    // Level / Rank Level
    if (Info.Level > 0)
    {
        Player.Level = Info.Level;
    }
    if (Info.RankLevel > 0)
    {
        Player.RankLevel = Info.RankLevel;
    }

    // XP / Rank
    if (Info.XP > 0)
    {
        Player.XP = Info.XP;
    }
    if (Info.Rank > 0)
    {
        Player.Rank = Info.Rank;
    }

    // Misc
    SetInventory("DRPGCredits", Info.Credits);
    Player.PrevCredits = Info.Credits;
    SetInventory("DRPGModule", Info.Modules);
    Player.Medkit = Info.Medkit;
    SetInventory("DRPGChipGold", Info.GoldChips);
    SetInventory("DRPGChipPlatinum", Info.PlatinumChips);
    SetInventory("DRPGUACCard", Info.ShopCard);
    if (Info.ShopCard == 5)
        SetInventory("DRPGDiamondUACCard", 1);
    Player.Augs.Battery = Info.Battery;
    Player.Toxicity = Info.Toxicity;
    ArenaMaxWave = Info.ArenaWave;

    // We need to delay here so that given items don't affect other things like Stat XP
    Delay(1);

    // Stats
    Player.Strength     = Info.Stats[0];
    Player.Defense      = Info.Stats[1];
    Player.Vitality     = Info.Stats[2];
    Player.Energy       = Info.Stats[3];
    Player.Regeneration = Info.Stats[4];
    Player.Agility      = Info.Stats[5];
    Player.Capacity     = Info.Stats[6];
    Player.Luck         = Info.Stats[7];

    // Natural Bonuses
    Player.StrengthNat     = Info.StatsNat[0];
    Player.DefenseNat      = Info.StatsNat[1];
    Player.VitalityNat     = Info.StatsNat[2];
    Player.EnergyNat       = Info.StatsNat[3];
    Player.RegenerationNat = Info.StatsNat[4];
    Player.AgilityNat      = Info.StatsNat[5];
    Player.CapacityNat     = Info.StatsNat[6];
    Player.LuckNat         = Info.StatsNat[7];

    // Total values
    Player.StrengthTotal = Player.Strength;
    Player.DefenseTotal = Player.Defense;
    Player.VitalityTotal = Player.Vitality;
    Player.EnergyTotal = Player.Energy;
    Player.RegenerationTotal = Player.Regeneration;
    Player.AgilityTotal = Player.Agility;
    Player.CapacityTotal = Player.Capacity;
    Player.LuckTotal = Player.Luck;

    // Natural stats values
    if (GetCVar("drpg_levelup_natural"))
    {
        Player.StrengthTotal += Player.StrengthNat;
        Player.DefenseTotal += Player.DefenseNat;
        Player.VitalityTotal += Player.VitalityNat;
        Player.EnergyTotal += Player.EnergyNat;
        Player.RegenerationTotal += Player.RegenerationNat;
        Player.AgilityTotal += Player.AgilityNat;
        Player.CapacityTotal += Player.CapacityNat;
        Player.LuckTotal += Player.LuckNat;
    }

    Player.EP = 50 + ((Player.Level + 1) / 2) * 5 + Player.EnergyTotal * 5;
    Player.HealthMax = 50 + ((Player.Level + 1) / 2) * 5 + Player.VitalityTotal * 5;
    Player.ActualHealth = Player.HealthMax;
    SetActorProperty(0, APROP_Health, Player.HealthMax);
    Player.PrevHealth = Player.ActualHealth;

    // Skills
    for (int i = 0; i < MAX_CATEGORIES; i++)
        for (int j = 0; j < MAX_SKILLS; j++)
        {
            Player.SkillLevel[i][j].Level = Info.Skills[i][j];
            Player.SkillLevel[i][j].CurrentLevel = Info.Skills[i][j];
        }

    // Skill Wheel
    for (int i = 0; i < MAX_SKILLKEYS; i++)
    {
        Player.SkillCategory[i] = Info.SkillWheel[i][0];
        Player.SkillIndex[i]    = Info.SkillWheel[i][1];
        if (Info.SkillWheel[i][0] == 255 || Info.SkillWheel[i][1] == 255)
        {
            Player.SkillCategory[i] = -1;
            Player.SkillIndex[i] = -1;
        }
    }

    // Augmentations
    for (int i = 0; i < AUG_MAX; i++)
    {
        Player.Augs.Level[i] = Info.Augs[i];
        Player.Augs.CurrentLevel[i] = Player.Augs.Level[i];
    }

    // Stims
    for (int i = 0; i < STIM_MAX; i++)
        Player.Stim.Vials[i] = Info.Stims[i];

    // Turret
    for (int i = 0; i < TU_MAX; i++)
        Player.Turret.Upgrade[i] = Info.TurretUpgrades[i];

    Delay(1);

    // Locker
    for (int i = 0; i < ITEM_CATEGORIES; i++)
        for (int j = 0; j < ITEM_MAX; j++)
        {
            if (Info.Locker[i][j])
                Player.Locker[i][j] += Info.Locker[i][j];

            if (Info.CompatMode == COMPAT_DRLA && i == 0) // Weapon Modpacks
            {
                Player.WeaponMods[j].Total = Info.WeaponMods[j][0];
                Player.WeaponMods[j].Power = Info.WeaponMods[j][1];
                Player.WeaponMods[j].Bulk = Info.WeaponMods[j][2];
                Player.WeaponMods[j].Agility = Info.WeaponMods[j][3];
                Player.WeaponMods[j].Technical = Info.WeaponMods[j][4];
                Player.WeaponMods[j].Sniper = Info.WeaponMods[j][5];
                Player.WeaponMods[j].Firestorm = Info.WeaponMods[j][6];
                Player.WeaponMods[j].Nano = Info.WeaponMods[j][7];
                Player.WeaponMods[j].Artifacts = Info.WeaponMods[j][8];
            }
        }

    // Auto-State
    for (int i = 0; i < ITEM_CATEGORIES; i++)
        for (int j = 0; j < ITEM_MAX; j++)
            Player.ItemAutoMode[i][j] = Info.ItemAutoMode[i][j];

    // Load Level Data
    if (GetActivatorCVar("drpg_char_load_maplevel"))
    {
        if (ScriptCall("DRPGZChar", "LoadLevelData", PlayerNumber()))
        {
            int CW = ScriptCall("DRPGZChar", "GetSavedCurrentWAD");
            str LL = (str)ScriptCall("DRPGZChar", "GetSavedLevelLump");
            int LN = ScriptCall("DRPGZChar", "GetSavedLevelNum");

            if (CW <= KnownWadCount && ScriptCall("DRPGZUtilities", "CheckForLevelLump", LL))
            {
                LevelInfo *NewMap = klArrayUtils(1, CW, NULL);
                NewMap->LumpName = LL;
                NewMap->NiceName = "Unknown Area";
                NewMap->LevelNum = LN;
                NewMap->SecretMap = false;
                NewMap->UACBase = false;
                NewMap->UACArena = false;
                NewMap->EWHub = false;
                NewMap->Completed = false;
                NewMap->NeedsRealInfo = true;

                if (DebugLog)
                    Log("\CdDEBUG: \C-Loaded Char Level Data; CW: %i, LL: %S, LN: %i", CW, LL, LN);
            }
            else
            {
                PrintError("\CgERROR: \CjInvalid Saved Level Data! Different WAD loaded?");
                ActivatorSound("menu/error", 127);
            }

            ScriptCall("DRPGZChar", "ClearRetrievedLevelData");
        }
    }

    // ----- COMPATIBILITY EXTENSIONS -----

    // Compatibility Handling - DoomRL Arsenal
    // DRLA Tokens
    if (CompatMode == COMPAT_DRLA)
        for (int i = 0; i < DRLA_MAX_TOKENS; i++)
            if (Info.DRLATokens[i])
                SetInventory(DRLATokens[i], 1);

    // Compatibility Handling - DoomRL Arsenal Monsters
    // ------ Danger Level ------
    if (CompatMonMode == COMPAT_DRLA && GetActivatorCVar("drpg_char_load_rl_dangerlevel"))
    {
        int rlmDL = GetActivatorCVar("drpg_char_data_rl_dangerlevel");

        SetInventory("RLDangerLevel", rlmDL);

        if (DebugLog)
            Log("\CdDEBUG: \C-Loaded Char RL Danger Level: %i", rlmDL);
    }

    // Compatibility Handling - DoomRL Arsenal
    // Chances for Assembled/Exotic/Superior/Unique/Demonic/Legendary armor and boots
    Player.ArmorAssembledChance = Info.ArmorChances[0];
    Player.ArmorExoticChance = Info.ArmorChances[1];
    Player.ArmorSuperiorChance = Info.ArmorChances[2];
    Player.ArmorUniqueChance = Info.ArmorChances[3];
    Player.ArmorDemonicChance = Info.ArmorChances[4];
    Player.ArmorLegendaryChance = Info.ArmorChances[5];

    // Compatibility Handling - DoomRL Arsenal
    // Chances for Exotic/Superior/Unique/Demonic/Legendary weapon
    Player.WeaponExoticChance = Info.WeaponsChances[0];
    Player.WeaponSuperiorChance = Info.WeaponsChances[1];
    Player.WeaponUniqueChance = Info.WeaponsChances[2];
    Player.WeaponDemonicChance = Info.WeaponsChances[3];
    Player.WeaponLegendaryChance = Info.WeaponsChances[4];

    // Compatibility Handling - DoomRL Arsenal
    // Weapons/Armors/Boots/Shield parts spawned values
    for (int i = 0; i < ItemMax[0]; i++)
        ItemData[0][i].Spawned = Info.WeaponsSpawned[i];
    for (int i = 0; i < ItemMax[3]; i++)
        ItemData[3][i].Spawned = Info.ArmorsSpawned[i];
    for (int i = 0; i < ItemMax[9]; i++)
        ItemData[9][i].Spawned = Info.BootsSpawned[i];
    for (int i = 0; i < ItemMax[5]; i++)
        ItemData[5][i].Spawned = Info.ShieldsSpawned[i];

    // Set Health and EP to their proper max values
    Player.ActualHealth = Player.HealthMax;
    Player.EP = Player.EPMax;

    UpdateShopAutoList();

    ActivatorSound("charsave/accept", 127);
    SetFont("BIGFONT");
    HudMessage("===== Character Load Complete =====");
    EndHudMessage(HUDMSG_FADEOUT | HUDMSG_LOG, 70, "Green", 0.5, 0.3, 3.0, 2.0);
    SetFont("SMALLFONT");
    HudMessage("Make sure to withdraw your items from the shop!");
    EndHudMessage(HUDMSG_FADEOUT | HUDMSG_LOG, 71, "White", 0.5, 0.35, 3.0, 2.0);
}

NamedScript MenuEntry void ClearCharacter()
{
    // Character Data
    for (int i = 0; i < CHARSAVE_MAXCVARS; i++)
        SetActivatorCVarString(StrParam("drpg_char_data_%d", i), "");

    SetActivatorCVar("drpg_char_data_len", 0);

    // Level Data
    SetActivatorCVarString("drpg_char_data_level", "");

    // Danger Level
    if (CompatMonMode == COMPAT_DRLA)
        SetActivatorCVar("drpg_char_data_rl_dangerlevel", 0);

    // Notification
    ActivatorSound("charsave/accept", 127);
    SetFont("BIGFONT");
    HudMessage("===== Character Cleared =====");
    EndHudMessage(HUDMSG_FADEOUT | HUDMSG_LOG, 0, "Yellow", 0.5, 0.3, 3.0, 2.0);
}

NamedScript Console void DumpCharacter()
{
    for (int i = 0; i < GetActivatorCVar("drpg_char_data_len"); i++)
    {
        str Data = GetActivatorCVarString(StrParam("drpg_char_data_%d", i));
        Log("\CdData %d \C-- \Ca%S", i, Data);
    }
}

NamedScript void DepositInventory()
{
    // Reset deposit vars
    DepositItems = 0;
    DepositTotal = 0;

    // Start the depositing process
    Depositing = true;
    SetPlayerProperty(0, 1, PROP_TOTALLYFROZEN);

    // Calculate Item Total
    for (int i = 0; i < ItemCategories; i++)
        for (int j = 0; j < ItemMax[i]; j++)
            DepositTotal += CheckInventory(ItemData[i][j].Actor);

    // Delay handler
    int TimeDivide = DepositTotal / (35 * 3);
    if (TimeDivide < 1)
        TimeDivide = 1;

    int Count = 0;
    for (int i = 0; i < ItemCategories; i++)
        for (int j = 0; j < ItemMax[i]; j++)
            if (CheckInventory(ItemData[i][j].Actor) > 0)
                while (CheckInventory(ItemData[i][j].Actor) > 0)
                {
                    DepositItem(i, j, true, true);
                    DepositItems++;
                    if ((Count++ % TimeDivide) == 0) Delay(1);
                }
    // Stop the depositing process
    DepositItems = DepositTotal;
    Delay(1);
    Depositing = false;
    SetPlayerProperty(0, 0, PROP_TOTALLYFROZEN);
}

// Hexadecimal storage
char ToHexChar(int i)
{
    return "0123456789ABCDEF"[i & 0xF];
}

int FromHexChar(char c)
{
    switch (c)
    {
    case '0':
        return 0;
    case '1':
        return 1;
    case '2':
        return 2;
    case '3':
        return 3;
    case '4':
        return 4;
    case '5':
        return 5;
    case '6':
        return 6;
    case '7':
        return 7;
    case '8':
        return 8;
    case '9':
        return 9;
    case 'A':
    case 'a':
        return 10;
    case 'B':
    case 'b':
        return 11;
    case 'C':
    case 'c':
        return 12;
    case 'D':
    case 'd':
        return 13;
    case 'E':
    case 'e':
        return 14;
    case 'F':
    case 'f':
        return 15;
    }

    return 0; // Default
}

NamedScript void PopulateCharData(CharSaveInfo *Info)
{
    // Version / Compatibility Flag
    Info->Version = CHARSAVE_VERSION;
    Info->CompatMode = CompatMode;

    // Level / Rank Level
    Info->Level = Player.Level;
    Info->RankLevel = Player.RankLevel;

    // XP / Rank
    Info->XP = Player.XP;
    Info->Rank = Player.Rank;

    // Stats
    Info->Stats[0] = Player.Strength;
    Info->Stats[1] = Player.Defense;
    Info->Stats[2] = Player.Vitality;
    Info->Stats[3] = Player.Energy;
    Info->Stats[4] = Player.Regeneration;
    Info->Stats[5] = Player.Agility;
    Info->Stats[6] = Player.Capacity;
    Info->Stats[7] = Player.Luck;

    for (int i = 0; i < STAT_MAX; i++)
        if (Info->Stats[i] < 0)
            Info->Stats[i] = 0;

    // Natural Bonuses
    Info->StatsNat[0] = Player.StrengthNat;
    Info->StatsNat[1] = Player.DefenseNat;
    Info->StatsNat[2] = Player.VitalityNat;
    Info->StatsNat[3] = Player.EnergyNat;
    Info->StatsNat[4] = Player.RegenerationNat;
    Info->StatsNat[5] = Player.AgilityNat;
    Info->StatsNat[6] = Player.CapacityNat;
    Info->StatsNat[7] = Player.LuckNat;

    // Skills
    for (int i = 0; i < MAX_CATEGORIES; i++)
        for (int j = 0; j < MAX_SKILLS; j++)
            Info->Skills[i][j] = Player.SkillLevel[i][j].Level;

    // Skill Wheel
    for (int i = 0; i < MAX_SKILLKEYS; i++)
    {
        Info->SkillWheel[i][0] = Player.SkillCategory[i];
        Info->SkillWheel[i][1] = Player.SkillIndex[i];
        if (Info->SkillWheel[i][0] == -1)
            Info->SkillWheel[i][0] = 255;
        if (Info->SkillWheel[i][1] == -1)
            Info->SkillWheel[i][1] = 255;
    }

    // Augmentations
    for (int i = 0; i < AUG_MAX; i++)
        Info->Augs[i] = Player.Augs.Level[i];

    // Stims
    for (int i = 0; i < STIM_MAX; i++)
        Info->Stims[i] = Player.Stim.Vials[i];
    for (int i = 0; i < TU_MAX; i++)
        Info->TurretUpgrades[i] = Player.Turret.Upgrade[i];

    // Misc
    Info->Credits = CheckInventory("DRPGCredits");
    Info->Modules = CheckInventory("DRPGModule");
    Info->Medkit = Player.Medkit;
    Info->GoldChips = CheckInventory("DRPGChipGold");
    Info->PlatinumChips = CheckInventory("DRPGChipPlatinum");
    Info->ShopCard = Player.ShopCard;
    Info->Battery = Player.Augs.Battery;
    Info->Toxicity = Player.Toxicity;
    Info->ArenaWave = ArenaMaxWave;

    // Locker
    for (int i = 0; i < ITEM_CATEGORIES; i++)
        for (int j = 0; j < ITEM_MAX; j++)
        {
            if (Player.Locker[i][j])
                Info->Locker[i][j] = Player.Locker[i][j];

            if (Info->CompatMode == COMPAT_DRLA && i == 0) // Weapon Modpacks
            {
                Info->WeaponMods[j][0] = Player.WeaponMods[j].Total;
                Info->WeaponMods[j][1] = Player.WeaponMods[j].Power;
                Info->WeaponMods[j][2] = Player.WeaponMods[j].Bulk;
                Info->WeaponMods[j][3] = Player.WeaponMods[j].Agility;
                Info->WeaponMods[j][4] = Player.WeaponMods[j].Technical;
                Info->WeaponMods[j][5] = Player.WeaponMods[j].Sniper;
                Info->WeaponMods[j][6] = Player.WeaponMods[j].Firestorm;
                Info->WeaponMods[j][7] = Player.WeaponMods[j].Nano;
                Info->WeaponMods[j][8] = Player.WeaponMods[j].Artifacts;
            }
        }

    // Auto-State
    for (int i = 0; i < ITEM_CATEGORIES; i++)
        for (int j = 0; j < ITEM_MAX; j++)
            Info->ItemAutoMode[i][j] = Player.ItemAutoMode[i][j];

    // ----- COMPATIBILITY EXTENSIONS -----

    // Compatibility Handling - DoomRL Arsenal
    // DRLA Tokens
    if (CompatMode == COMPAT_DRLA)
        for (int i = 0; i < DRLA_MAX_TOKENS; i++)
            if (CheckInventory(DRLATokens[i]))
                Info->DRLATokens[i] = true;

    // Compatibility Handling - DoomRL Arsenal
    // Chances for Assembled/Exotic/Superior/Unique/Demonic/Legendary armor and boots
    Info->ArmorChances[0] = Player.ArmorAssembledChance;
    Info->ArmorChances[1] = Player.ArmorExoticChance;
    Info->ArmorChances[2] = Player.ArmorSuperiorChance;
    Info->ArmorChances[3] = Player.ArmorUniqueChance;
    Info->ArmorChances[4] = Player.ArmorDemonicChance;
    Info->ArmorChances[5] = Player.ArmorLegendaryChance;

    // Compatibility Handling - DoomRL Arsenal
    // Chances for Exotic/Superior/Unique/Demonic/Legendary weapon
    Info->WeaponsChances[0] = Player.WeaponExoticChance;
    Info->WeaponsChances[1] = Player.WeaponSuperiorChance;
    Info->WeaponsChances[2] = Player.WeaponUniqueChance;
    Info->WeaponsChances[3] = Player.WeaponDemonicChance;
    Info->WeaponsChances[4] = Player.WeaponLegendaryChance;

    // Compatibility Handling - DoomRL Arsenal
    // Weapons/Armors/Boots/Shield parts spawned values
    for (int i = 0; i < ItemMax[0]; i++)
        Info->WeaponsSpawned[i] = ItemData[0][i].Spawned;
    for (int i = 0; i < ItemMax[3]; i++)
        Info->ArmorsSpawned[i] = ItemData[3][i].Spawned;
    for (int i = 0; i < ItemMax[9]; i++)
        Info->BootsSpawned[i] = ItemData[9][i].Spawned;
    for (int i = 0; i < ItemMax[5]; i++)
        Info->ShieldsSpawned[i] = ItemData[5][i].Spawned;
}

NamedScript void LoadCharDataFromString(CharSaveInfo *Info, char const *String)
{
    //LoadedData = false;
    int StringPos = 0;
    int Version = 0;
    Info->Version = -3;

    // Version / Compatibility Flag
    Version = HexToInteger(String + StringPos, 4);
    StringPos += 4;
    if (Version != CHARSAVE_VERSION)
    {
        Info->Version = -2;
        //LoadedData = true;
        return;
    }

    Info->CompatMode = HexToInteger(String + StringPos, 2);
    StringPos += 2;
    if (Info->CompatMode != CompatMode)
    {
        Info->Version = -1;
        //LoadedData = true;
        return;
    }

    // Level / Rank Level
    Info->Level = HexToInteger(String + StringPos, 2);
    StringPos += 2;
    Info->RankLevel = HexToInteger(String + StringPos, 2);
    StringPos += 2;

    // XP / Rank
    Info->XP = HexToInteger(String + StringPos, 8);
    StringPos += 8;
    Info->Rank = HexToInteger(String + StringPos, 8);
    StringPos += 8;

    // Stats
    for (int i = 0; i < STAT_MAX; i++)
    {
        Info->Stats[i] = HexToInteger(String + StringPos, 2);
        StringPos += 2;
    }

    // Natural Bonuses
    for (int i = 0; i < STAT_MAX; i++)
    {
        Info->StatsNat[i] = HexToInteger(String + StringPos, 2);
        StringPos += 2;
    }

    // Skills
    for (int i = 0; i < MAX_CATEGORIES; i++)
        for (int j = 0; j < MAX_SKILLS; j++)
        {
            Info->Skills[i][j] = HexToInteger(String + StringPos, 2);
            StringPos += 2;
        }

    // Skill Wheel
    for (int i = 0; i < MAX_SKILLKEYS; i++)
    {
        Info->SkillWheel[i][0] = HexToInteger(String + StringPos,     2);
        Info->SkillWheel[i][1] = HexToInteger(String + StringPos + 2, 2);
        StringPos += 4;
    }

    // Augmentations
    for (int i = 0; i < AUG_MAX; i++)
    {
        Info->Augs[i] = HexToInteger(String + StringPos, 2);
        StringPos += 2;
    }

    // Stims
    for (int i = 0; i < STIM_MAX; i++)
    {
        Info->Stims[i] = HexToInteger(String + StringPos, 8);
        StringPos += 8;
    }

    // Turret
    for (int i = 0; i < TU_MAX; i++)
    {
        Info->TurretUpgrades[i] = HexToInteger(String + StringPos, 1);
        StringPos += 1;
    }

    // Misc
    Info->Credits = HexToInteger(String + StringPos, 8);
    StringPos += 8;
    Info->Modules = HexToInteger(String + StringPos, 8);
    StringPos += 8;
    Info->Medkit = HexToInteger(String + StringPos, 4);
    StringPos += 4;
    Info->GoldChips = HexToInteger(String + StringPos, 4);
    StringPos += 4;
    Info->PlatinumChips = HexToInteger(String + StringPos, 4);
    StringPos += 4;
    Info->ShopCard = HexToInteger(String + StringPos, 1);
    StringPos += 1;
    Info->Battery = HexToInteger(String + StringPos, 4);
    StringPos += 4;
    Info->Toxicity = HexToInteger(String + StringPos, 2);
    StringPos += 2;
    Info->ArenaWave = HexToInteger(String + StringPos, 4);
    StringPos += 4;

    // Locker
    for (int i = 0; i < ITEM_CATEGORIES; i++)
    {
        for (int j = 0; j < ITEM_MAX; j++)
        {
            if (i < ItemCategories && j < ItemMax[i])
            {
                Info->Locker[i][j] = HexToInteger(String + StringPos, 4);
            }
            StringPos += 4;

            if (Info->CompatMode == COMPAT_DRLA && i == 0) // Weapon Modpacks
                for (int k = 0; k < DRLA_MODPACK_SIZE; k++)
                {
                    if (j < ItemMax[i])
                    {
                        Info->WeaponMods[j][k] = HexToInteger(String + StringPos, 1);
                    }
                    StringPos += 1;
                }
        }
        //Delay(1);
    }

    // Auto-State
    for (int i = 0; i < ITEM_CATEGORIES; i++)
        for (int j = 0; j < ITEM_MAX; j++)
        {
            if (i < ItemCategories && j < ItemMax[i])
            {
                Info->ItemAutoMode[i][j] = HexToInteger(String + StringPos, 1);
            }
            StringPos += 1;
        }

    // ----- COMPATIBILITY EXTENSIONS -----

    // Compatibility Handling - DoomRL Arsenal
    // DRLA Tokens
    for (int i = 0; i < DRLA_MAX_TOKENS; i++)
    {
        Info->DRLATokens[i] = HexToInteger(String + StringPos, 1);
        StringPos += 1;
    }

    // Compatibility Handling - DoomRL Arsenal
    // Chances for Assembled/Exotic/Superior/Unique/Demonic/Legendary armor and boots
    for (int i = 0; i < 6; i++)
    {
        Info->ArmorChances[i] = HexToInteger(String + StringPos, 2);
        StringPos += 2;
    }

    // Compatibility Handling - DoomRL Arsenal
    // Chances for Exotic/Superior/Unique/Demonic/Legendary weapon
    for (int i = 0; i < 5; i++)
    {
        Info->WeaponsChances[i] = HexToInteger(String + StringPos, 2);
        StringPos += 2;
    }

    // Compatibility Handling - DoomRL Arsenal
    // Weapons/Armors/Boots/Shield parts spawned values
    for (int i = 0; i < ItemMax[0]; i++)
    {
        Info->WeaponsSpawned[i] = HexToInteger(String + StringPos, 2);
        StringPos += 2;
    }
    for (int i = 0; i < ItemMax[3]; i++)
    {
        Info->ArmorsSpawned[i] = HexToInteger(String + StringPos, 2);
        StringPos += 2;
    }
    for (int i = 0; i < ItemMax[9]; i++)
    {
        Info->BootsSpawned[i] = HexToInteger(String + StringPos, 2);
        StringPos += 2;
    }
    for (int i = 0; i < ItemMax[5]; i++)
    {
        Info->ShieldsSpawned[i] = HexToInteger(String + StringPos, 2);
        StringPos += 2;
    }

    // Verify Checksum
    Info->Checksum = HexToInteger(String + StringPos, 8);
    //StringPos += 8;

    if (DebugLog)
        Log("\CdDEBUG: \C-Saved CRC is %d (%X)", Info->Checksum, Info->Checksum);

    unsigned int Checksum = (unsigned int)(crc(String, StringPos));

    if (DebugLog)
    {
        Log("\CdDEBUG: \C-CRC for recalled character is %d (%X)", Checksum, Checksum);
        Log("\CdDEBUG: \C-Version:%d", Version);
    }

    Info->Version = Version;

    if (Checksum != Info->Checksum)
    {
        Info->Version = -3;
        //LoadedData = true;
        return;
    }
    //LoadedData = true;
}

NamedScript char const *MakeSaveString(CharSaveInfo *Info)
{
    char *SaveString = calloc(25000, sizeof(char));
    unsigned int pos = 0;

    // Version
    SaveString[pos + 3] = ToHexChar(Info->Version);
    SaveString[pos + 2] = ToHexChar(Info->Version >> 4);
    SaveString[pos + 1] = ToHexChar(Info->Version >> 8);
    SaveString[pos + 0] = ToHexChar(Info->Version >> 12);
    pos += 4;

    // Compatibility
    SaveString[pos + 1] = ToHexChar(Info->CompatMode);
    SaveString[pos + 0] = ToHexChar(Info->CompatMode >> 4);
    pos += 2;

    // Level & Rank
    SaveString[pos + 3] = ToHexChar(Info->RankLevel);
    SaveString[pos + 2] = ToHexChar(Info->RankLevel >> 4);
    SaveString[pos + 1] = ToHexChar(Info->Level);
    SaveString[pos + 0] = ToHexChar(Info->Level >> 4);
    pos += 4;

    // XP
    SaveString[pos + 7] = ToHexChar(Info->XP);
    SaveString[pos + 6] = ToHexChar(Info->XP >> 4);
    SaveString[pos + 5] = ToHexChar(Info->XP >> 8);
    SaveString[pos + 4] = ToHexChar(Info->XP >> 12);
    SaveString[pos + 3] = ToHexChar(Info->XP >> 16);
    SaveString[pos + 2] = ToHexChar(Info->XP >> 20);
    SaveString[pos + 1] = ToHexChar(Info->XP >> 24);
    SaveString[pos + 0] = ToHexChar(Info->XP >> 28);
    pos += 8;

    // Rank
    SaveString[pos + 7] = ToHexChar(Info->Rank);
    SaveString[pos + 6] = ToHexChar(Info->Rank >> 4);
    SaveString[pos + 5] = ToHexChar(Info->Rank >> 8);
    SaveString[pos + 4] = ToHexChar(Info->Rank >> 12);
    SaveString[pos + 3] = ToHexChar(Info->Rank >> 16);
    SaveString[pos + 2] = ToHexChar(Info->Rank >> 20);
    SaveString[pos + 1] = ToHexChar(Info->Rank >> 24);
    SaveString[pos + 0] = ToHexChar(Info->Rank >> 28);
    pos += 8;

    // Stats
    for (int i = 0; i < STAT_MAX; i++)
    {
        SaveString[pos + 1] = ToHexChar(Info->Stats[i]);
        SaveString[pos + 0] = ToHexChar(Info->Stats[i] >> 4);
        pos += 2;
    }

    // Natural Bonuses
    for (int i = 0; i < STAT_MAX; i++)
    {
        SaveString[pos + 1] = ToHexChar(Info->StatsNat[i]);
        SaveString[pos + 0] = ToHexChar(Info->StatsNat[i] >> 4);
        pos += 2;
    }

    // Skills
    for (int i = 0; i < MAX_CATEGORIES; i++)
        for (int j = 0; j < MAX_SKILLS; j++)
        {
            SaveString[pos + 1] = ToHexChar(Info->Skills[i][j]);
            SaveString[pos + 0] = ToHexChar(Info->Skills[i][j] >> 4);
            pos += 2;
        }

    // Skill Wheel
    for (int i = 0; i < MAX_SKILLKEYS; i++)
    {
        SaveString[pos + 3] = ToHexChar(Info->SkillWheel[i][1]);
        SaveString[pos + 2] = ToHexChar(Info->SkillWheel[i][1] >> 4);
        SaveString[pos + 1] = ToHexChar(Info->SkillWheel[i][0]);
        SaveString[pos + 0] = ToHexChar(Info->SkillWheel[i][0] >> 4);
        pos += 4;
    }

    // Augs
    for (int i = 0; i < AUG_MAX; i++)
    {
        SaveString[pos + 1] = ToHexChar(Info->Augs[i]);
        SaveString[pos + 0] = ToHexChar(Info->Augs[i] >> 4);
        pos += 2;
    }

    // Stims
    for (int i = 0; i < STIM_MAX; i++)
    {
        SaveString[pos + 7] = ToHexChar(Info->Stims[i]);
        SaveString[pos + 6] = ToHexChar(Info->Stims[i] >> 4);
        SaveString[pos + 5] = ToHexChar(Info->Stims[i] >> 8);
        SaveString[pos + 4] = ToHexChar(Info->Stims[i] >> 12);
        SaveString[pos + 3] = ToHexChar(Info->Stims[i] >> 16);
        SaveString[pos + 2] = ToHexChar(Info->Stims[i] >> 20);
        SaveString[pos + 1] = ToHexChar(Info->Stims[i] >> 24);
        SaveString[pos + 0] = ToHexChar(Info->Stims[i] >> 28);
        pos += 8;
    }

    // Turret
    for (int i = 0; i < TU_MAX; i++)
    {
        SaveString[pos + 0] = ToHexChar(Info->TurretUpgrades[i]);
        pos += 1;
    }

    // Misc

    // Credits
    SaveString[pos + 7] = ToHexChar(Info->Credits);
    SaveString[pos + 6] = ToHexChar(Info->Credits >> 4);
    SaveString[pos + 5] = ToHexChar(Info->Credits >> 8);
    SaveString[pos + 4] = ToHexChar(Info->Credits >> 12);
    SaveString[pos + 3] = ToHexChar(Info->Credits >> 16);
    SaveString[pos + 2] = ToHexChar(Info->Credits >> 20);
    SaveString[pos + 1] = ToHexChar(Info->Credits >> 24);
    SaveString[pos + 0] = ToHexChar(Info->Credits >> 28);
    pos += 8;

    // Modules
    SaveString[pos + 7] = ToHexChar(Info->Modules);
    SaveString[pos + 6] = ToHexChar(Info->Modules >> 4);
    SaveString[pos + 5] = ToHexChar(Info->Modules >> 8);
    SaveString[pos + 4] = ToHexChar(Info->Modules >> 12);
    SaveString[pos + 3] = ToHexChar(Info->Modules >> 16);
    SaveString[pos + 2] = ToHexChar(Info->Modules >> 20);
    SaveString[pos + 1] = ToHexChar(Info->Modules >> 24);
    SaveString[pos + 0] = ToHexChar(Info->Modules >> 28);
    pos += 8;

    // Medkit
    SaveString[pos + 3] = ToHexChar(Info->Medkit);
    SaveString[pos + 2] = ToHexChar(Info->Medkit >> 4);
    SaveString[pos + 1] = ToHexChar(Info->Medkit >> 8);
    SaveString[pos + 0] = ToHexChar(Info->Medkit >> 12);
    pos += 4;

    // Gold Chips
    SaveString[pos + 3] = ToHexChar(Info->GoldChips);
    SaveString[pos + 2] = ToHexChar(Info->GoldChips >> 4);
    SaveString[pos + 1] = ToHexChar(Info->GoldChips >> 8);
    SaveString[pos + 0] = ToHexChar(Info->GoldChips >> 12);
    pos += 4;

    // Platinum Chips
    SaveString[pos + 3] = ToHexChar(Info->PlatinumChips);
    SaveString[pos + 2] = ToHexChar(Info->PlatinumChips >> 4);
    SaveString[pos + 1] = ToHexChar(Info->PlatinumChips >> 8);
    SaveString[pos + 0] = ToHexChar(Info->PlatinumChips >> 12);
    pos += 4;

    // UAC Card
    SaveString[pos + 0] = ToHexChar(Info->ShopCard);
    pos += 1;

    // Aug Battery
    SaveString[pos + 3] = ToHexChar(Info->Battery);
    SaveString[pos + 2] = ToHexChar(Info->Battery >> 4);
    SaveString[pos + 1] = ToHexChar(Info->Battery >> 8);
    SaveString[pos + 0] = ToHexChar(Info->Battery >> 12);
    pos += 4;

    // Toxicity
    SaveString[pos + 1] = ToHexChar(Info->Toxicity);
    SaveString[pos + 0] = ToHexChar(Info->Toxicity >> 4);
    pos += 2;

    // Arena Wave
    SaveString[pos + 3] = ToHexChar(Info->ArenaWave);
    SaveString[pos + 2] = ToHexChar(Info->ArenaWave >> 4);
    SaveString[pos + 1] = ToHexChar(Info->ArenaWave >> 8);
    SaveString[pos + 0] = ToHexChar(Info->ArenaWave >> 12);
    pos += 4;

    // Locker
    for (int i = 0; i < ITEM_CATEGORIES; i++)
        for (int j = 0; j < ITEM_MAX; j++)
        {
            SaveString[pos + 3] = ToHexChar(Info->Locker[i][j]);
            SaveString[pos + 2] = ToHexChar(Info->Locker[i][j] >> 4);
            SaveString[pos + 1] = ToHexChar(Info->Locker[i][j] >> 8);
            SaveString[pos + 0] = ToHexChar(Info->Locker[i][j] >> 12);
            pos += 4;

            if (Info->CompatMode == COMPAT_DRLA && i == 0) // Weapon Modpacks
                for (int k = 0; k < DRLA_MODPACK_SIZE; k++)
                {
                    SaveString[pos + 0] = ToHexChar(Info->WeaponMods[j][k]);
                    pos += 1;
                }
        }

    // Auto-State
    for (int i = 0; i < ITEM_CATEGORIES; i++)
        for (int j = 0; j < ITEM_MAX; j++)
        {
            SaveString[pos + 0] = ToHexChar(Info->ItemAutoMode[i][j]);
            pos += 1;
        }

    // ----- COMPATIBILITY EXTENSIONS -----

    // Compatibility Handling - DoomRL Arsenal
    // DRLA Tokens
    for (int i = 0; i < DRLA_MAX_TOKENS; i++)
    {
        SaveString[pos + 0] = ToHexChar(Info->DRLATokens[i]);
        pos += 1;
    }

    // Compatibility Handling - DoomRL Arsenal
    // Chances for Assembled/Exotic/Superior/Unique/Demonic/Legendary armor and boots
    for (int i = 0; i < 6; i++)
    {
        SaveString[pos + 1] = ToHexChar(Info->ArmorChances[i]);
        SaveString[pos + 0] = ToHexChar(Info->ArmorChances[i] >> 4);
        pos += 2;
    }

    // Compatibility Handling - DoomRL Arsenal
    // Chances for Exotic/Superior/Unique/Demonic/Legendary weapon
    for (int i = 0; i < 5; i++)
    {
        SaveString[pos + 1] = ToHexChar(Info->WeaponsChances[i]);
        SaveString[pos + 0] = ToHexChar(Info->WeaponsChances[i] >> 4);
        pos += 2;
    }

    // Compatibility Handling - DoomRL Arsenal
    // Weapons/Armors/Boots/Shield parts spawned values
    for (int i = 0; i < ItemMax[0]; i++)
    {
        SaveString[pos + 1] = ToHexChar(Info->WeaponsSpawned[i]);
        SaveString[pos + 0] = ToHexChar(Info->WeaponsSpawned[i] >> 4);
        pos += 2;
    }
    for (int i = 0; i < ItemMax[3]; i++)
    {
        SaveString[pos + 1] = ToHexChar(Info->ArmorsSpawned[i]);
        SaveString[pos + 0] = ToHexChar(Info->ArmorsSpawned[i] >> 4);
        pos += 2;
    }
    for (int i = 0; i < ItemMax[9]; i++)
    {
        SaveString[pos + 1] = ToHexChar(Info->BootsSpawned[i]);
        SaveString[pos + 0] = ToHexChar(Info->BootsSpawned[i] >> 4);
        pos += 2;
    }
    for (int i = 0; i < ItemMax[5]; i++)
    {
        SaveString[pos + 1] = ToHexChar(Info->ShieldsSpawned[i]);
        SaveString[pos + 0] = ToHexChar(Info->ShieldsSpawned[i] >> 4);
        pos += 2;
    }

    Info->Checksum = (unsigned int)(crc(SaveString, pos));
    if (DebugLog)
        Log("\CdDEBUG: \C-CRC for save character is %d (%X)", Info->Checksum, Info->Checksum);

    // Checksum
    SaveString[pos + 7] = ToHexChar(Info->Checksum);
    SaveString[pos + 6] = ToHexChar(Info->Checksum >> 4);
    SaveString[pos + 5] = ToHexChar(Info->Checksum >> 8);
    SaveString[pos + 4] = ToHexChar(Info->Checksum >> 12);
    SaveString[pos + 3] = ToHexChar(Info->Checksum >> 16);
    SaveString[pos + 2] = ToHexChar(Info->Checksum >> 20);
    SaveString[pos + 1] = ToHexChar(Info->Checksum >> 24);
    SaveString[pos + 0] = ToHexChar(Info->Checksum >> 28);
    pos += 8;

    SaveString[pos] = '\x00';
    //length 23498. 105 strings @ 224 chars/str
    //SaveString = realloc(SaveString, strlen(SaveString) + 1);
    return SaveString;
}

NamedScriptSync void EncodeRLE(char const *InString, char *OutString)
{
    char *LastSize = NULL;
    int OutPos = 0;
    int InLength = strlen(InString);

    char LastSeen = '\x00';
    int LastCount = 0;

    for (int InPos = 0; InPos < InLength; InPos++)
    {
        char Seen = InString[InPos];

        if (Seen == LastSeen)
            LastCount++;
        else
        {
            if (LastCount > 3)
            {
                LastSize = calloc(48, sizeof(char));
                //memset(LastSize, 0, 48);
                snprintf(LastSize, 48, c"[%X]", LastCount - 1);
                for (int i = 0; i < strlen(LastSize); i++)
                    OutString[OutPos++] = (char)LastSize[i];
                free(LastSize);
                LastSize = NULL;
            }
            else
            {
                for (int i = LastCount - 1; i > 0; i--)
                    OutString[OutPos++] = LastSeen;
            }

            LastSeen = Seen;
            LastCount = 1;
            if (Seen)
                OutString[OutPos++] = Seen;
        }

        if (InPos % 1000 == 0) Delay(1);
    }

    if (LastCount > 1)
    {
        LastSize = calloc(48, sizeof(char));
        //memset(LastSize, 0, 48);
        snprintf(LastSize, 48, c"[%X]", LastCount - 1);
        for (int i = 0; i < strlen(LastSize); i++)
            OutString[OutPos++] = (char)LastSize[i];
        free(LastSize);
        LastSize = NULL;
    }

    OutString[OutPos] = '\x00';
}

NamedScriptSync void DecodeRLE(char const *InString, char *OutString)
{
    int OutPos = 0;
    int InLength = strlen(InString);

    char LastSeen = '\x00';
    char *CountString = calloc(8, sizeof(char));
    int CountStringLength = 0;
    int RepeatCount = 0;
    bool InBrackets = false;

    for (int InPos = 0; InPos < InLength; InPos++)
    {
        if (InString[InPos] == '[')
        {
            CountStringLength = 0;

            InBrackets = true;
        }
        else if (InString[InPos] == ']')
        {
            for (RepeatCount = HexToInteger(CountString, CountStringLength); RepeatCount > 0; RepeatCount--)
            {
                OutString[OutPos++] = LastSeen;
                if (OutPos % 1000 == 0) Delay(1);
            }

            InBrackets = false;
        }
        else if (InBrackets)
        {
            CountString[CountStringLength++] = InString[InPos];
        }
        else
        {
            LastSeen = InString[InPos];
            OutString[OutPos++] = LastSeen;
        }

        if (InPos % 1000 == 0) Delay(1);
    }

    free(CountString);
    OutString[OutPos] = '\x00';
}

int HexToInteger(char const *SourceStr, int Length)
{
    int NumDigits = Length;
    int Result = 0;

    if (Length <= 0)
        NumDigits = strlen(SourceStr);

    for (int i = NumDigits - 1; i >= 0; i--)
        Result += FromHexChar(SourceStr[i]) << 4 * ((NumDigits - 1) - i);

    return Result;
}

char *StringToCharP(str Source)
{
    int Length;
    Length = StrLen(Source);

    char *Destination = calloc(Length + 1, sizeof(char));
    Destination[Length] = '\x00';

    for (int i = 0; i < Length; i++)
        Destination[i] = Source[i];

    return Destination;
}

// CRC ALGORITHM
// ----------------------------------------------------------------------------

/* Table of CRCs of all 8-bit messages. */
unsigned long crc_table[32768];

/* Flag: has the table been computed? Initially false. */
bool crc_table_computed = false;

/* Make the table for a fast CRC. */
NamedScript void make_crc_table()
{
    int n;

    for (n = 0; n < 64; n++)
    {
        make_crc_piece(512 * n);
    }
    crc_table_computed = false;
}

NamedScript void make_crc_piece(int start)
{
    for (int n = 0; n < 512; n++)
    {
        unsigned long c = (unsigned long) start + n;
        for (int k = 0; k < 8; k++)
        {
            if (c & 1)
                c = 0xEDB88320L ^ (c >> 1);
            else
                c = c >> 1;
        }
        crc_table[start + n] = c;
    }
}

/* Update a running CRC with the bytes buf[0..len-1]--the CRC
should be initialized to all 1's, and the transmitted value
is the 1's complement of the final running CRC (see the
crc() routine below)). */

NamedScript unsigned long update_crc(unsigned long crclx, char const *buf, int len)
{
    unsigned long c = crclx;
    int n;
    if (len > 32768) len = 32768;

    if (!crc_table_computed)
        make_crc_table();

    for (n = 0; n < 64; n++)
        c = do_crc_piece(buf, c, 512 * n, len);

    return c;
}

NamedScript unsigned long do_crc_piece(char const *buf, unsigned long c, int start, int stop)
{
    for (int n = 0; n < 512 && start + n < stop; n++)
    {
        c = crc_table[(c ^ buf[start + n]) & 0xFF] ^ (c >> 8);
    };

    return c;
}

/* Return the CRC of the bytes buf[0..len-1]. */
NamedScript unsigned long crc(char const *buf, int len)
{
    unsigned long i = update_crc(0xFFFFFFFFL, buf, len) ^ 0xFFFFFFFFL;
    crc_table_computed = false;
    return i;
}

NamedScript Console void TestLocker(int item)
{
    int ItemCount = Player.Locker[0][item];
    int ModsCount = Player.WeaponMods[item].Total;
    int PowerMods = Player.WeaponMods[item].Power;
    int BulkMods = Player.WeaponMods[item].Bulk;
    int AgilityMods = Player.WeaponMods[item].Agility;
    int TechnicalMods = Player.WeaponMods[item].Technical;
    int SniperMods = Player.WeaponMods[item].Sniper;
    int FirestormMods = Player.WeaponMods[item].Firestorm;
    int NanoMods = Player.WeaponMods[item].Nano;
    int Artifacts = Player.WeaponMods[item].Artifacts;
    Log("Item # %i\nName: %S\nTotal Mods: %i\n\
    Power Mods:   %i  Bulk Mods:      %i\n\
    Agility Mods: %i  Tech Mods:      %i\n\
    Sniper Mods:  %i  Firestorm Mods: %i\n\
    Nano mods:    %i  Artifacts:      %i",
        ItemCount,ItemData[0][item].Name,ModsCount,PowerMods,BulkMods,AgilityMods,TechnicalMods,SniperMods,FirestormMods,NanoMods,Artifacts);
}

// ----------------------------------------------------------------------------
