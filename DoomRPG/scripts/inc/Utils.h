#ifndef _UTILS_DH_
#define _UTILS_DH_

#include "Defs.h"

extern bool DebugLog;
extern int const AuraTID;
extern str const AuraIcons[AURA_MAX + 1];
extern str const ColorNames[26];
extern str const ColorCodes[26];

// Skill levels stuff
extern str const SkillLevelsDF[MAX_SKILLLEVELS_DF];
extern str const SkillLevelsDRLA[MAX_SKILLLEVELS_DRLA];
extern int SkillLevelsMax;

NamedScript DECORATE int InOutpost();
NamedScript DECORATE int GetUniqueTID();
NamedScript DECORATE int GetAmmoMax(int);
NamedScript DECORATE void SoulEffect(int);
NamedScript DECORATE void SoulCalculate();
NamedScript DECORATE void DestroyImmunity();
NamedScript DECORATE int GetSkillLevel();
NamedScript DECORATE int GetShopCard();
NamedScript DECORATE void SetDamageType(int);
NamedScript DECORATE int CheckAugBatteryMax();
NamedScript DECORATE int CheckInventoryMax();
NamedScript DECORATE int CheckCapacity();
NamedScript DECORATE int GetTimer();
NamedScript DECORATE int GetSpeed(int);
NamedScript DECORATE int HPEPMax();
NamedScript DECORATE int PowersuitCheck();
NamedScript DECORATE int ShieldRemoveArmor();
NamedScript DECORATE int GetAveragePlayerLevel();
NamedScript DECORATE int GetModuleSpawnFactor();
NamedScript DECORATE int StatusEffectSeverity(int);
NamedScript DECORATE int StatusEffectTimer(int);
NamedScript DECORATE void RainbowTranslationPulse(int);
NamedScript DECORATE int CheckActorSky();

// Monsters
NamedScript DECORATE void HealMonster(int, int);
NamedScript DECORATE void TeleportMonster();
void DropMoney(int, int, int);
OptionalArgs(6) int DropMonsterItem(int, int, str, int, fixed, fixed, fixed, fixed, fixed, fixed);
bool MonsterSeeTarget(int);
fixed MonsterDistanceTarget(int);
bool ActorSeePlayers(int, int);
bool ActorNotSeePlayers(int, int, bool);
NamedScript DECORATE void CorpsesCleanup();

// Players
NamedScript DECORATE void RegenBoost();
NamedScript KeyBind  void SetSkill(int);
NamedScript KeyBind  void Respec(bool, bool);
NamedScript DECORATE int GetAugBattery();
NamedScript DECORATE int GetAugBatteryMax();
NamedScript DECORATE int GetToxicity();
NamedScript DECORATE int GetStimPowerupActive();
NamedScript DECORATE void AddToxicity(int);
NamedScript DECORATE void AddStimImmunity(int);
NamedScript DECORATE void ClearBurnout();
NamedScript KeyBind  void PurgeDrops();
NamedScript Console  void CheckArmorStats();

bool StatsCapped();
int AveragePlayerLevel();
int AveragePlayerRank();
int AveragePlayerCredits();
int AveragePlayerLuck();
bool NomadInGame();
bool HaveStatusEffect();
bool DropPlayerItem(str);
bool IsPlayerMoving();
int FindPlayerID(int);
OptionalArgs(1) bool SetActivatorToTargetExtended(int, int);
bool IsTimeFrozen();
NamedScript DECORATE fixed MapLevelMod();
fixed StatsNatMod();

// Inventory
int CheckInventoryTID(int, str);
int SetInventory(str, int);
int SetActorInventory(int, str, int);
void DropEntireInventory();

// Items
int GetAmmoAmount(str);

// Health
NamedScript DECORATE int GetHealthPercent(int);
NamedScript DECORATE int HealthBelowPercent(int);
NamedScript DECORATE int GetHealthMax();
NamedScript DECORATE void CapHealthItem();

// Shield
NamedScript DECORATE bool ShieldActive();
NamedScript DECORATE int GetShield();
NamedScript DECORATE int GetShieldMax();
NamedScript DECORATE bool ShieldHealthMax();
NamedScript Console int GetPlayerShieldCharge(int);
NamedScript Console int GetPlayerShieldCapacity(int);

// EP
NamedScript DECORATE void AddEP(int, bool);
NamedScript DECORATE OptionalArgs(1) void RemoveEP(int, int);
NamedScript DECORATE int GetEP();
NamedScript DECORATE int GetEPMax();

// Skills
NamedScript KeyBind  void PlayerTeleport(int);
bool FireProjectile(str);
bool Nova(str, int);

// Auras
NamedScript DECORATE void GetAuraTokens();
void SpawnAuras(int, bool);
bool IsStealth(int);
bool PlayerHasAura(int);
bool MonsterHasAura(MonsterStatsPtr);
bool PlayerHasShadowAura(int);
bool MonsterHasShadowAura(MonsterStatsPtr);

// Drawing
int HudMessage(str Format, ...);
OptionalArgs(3) void EndHudMessageSelect(bool bold, int flags, int id, str color, fixed x, fixed y, fixed holdtime, fixed opt1, fixed opt2, fixed opt3);
#define EndHudMessage(...) EndHudMessageSelect(false, __VA_ARGS__)
#define EndHudMessageBold(...) EndHudMessageSelect(true, __VA_ARGS__)
str StrParam(str Format, ...);
NamedScript void FadeRangeFlash(int, int, int, fixed, int, int, int, fixed, fixed);

NamedScript void PrintTextWiggle(char *, int, int, int, int, fixed, fixed, fixed, fixed);
NamedScript void DrawStatUp(int);
void PrintSpriteFade(str, int, fixed, fixed, fixed, fixed);
void PrintSpriteAlpha(str, int, fixed, fixed, fixed, fixed);
OptionalArgs(1) void PrintSpritePulse(str, int, fixed, fixed, fixed, fixed, fixed, bool);
OptionalArgs(2) void PrintMessage(str, int, fixed);
void PrintError(str);
void DrawBattery();
void DrawShieldInfo(int, fixed, fixed, int);
void DrawShieldModel(int, fixed, fixed, int);
void DrawMissionInfo(MissionInfo *, fixed, fixed, bool);
OptionalArgs(1) void DrawBar(str, int, int, int, bool);
void DrawProgressBar(str, int);
void DrawBorder(str, int, int, int, int, int, int);

// Compatibility/Extensions
void RemoveDRLAItem(int, int);
void GiveDRLAArmorToken(str);
void RemoveDRLAArmorToken(str);
void CheckDRLASetWeapons();
void NomadModPacksSave();
void NomadModPacksLoad();
void NanomaniacTransport();
NamedScript DECORATE void PhaseSistersDataSave();
NamedScript DECORATE void PhaseSistersDataLoad();

// Math
int CalcPercent(int, int);
int Pow(int, int);
fixed PowFixed(fixed, int);
int Abs(int);
int RoundInt(fixed);
long int RoundLongInt(long fixed);
fixed AbsFixed(fixed);
fixed Min(fixed, fixed);
fixed Max(fixed, fixed);
fixed Distance(int, int);
fixed Distance2D(int, int, int, int);
fixed Lerp(fixed, fixed, fixed);
int LerpInt(int, int, int);
void Interpolate(InterpData *);
fixed Map(fixed, fixed, fixed, fixed, fixed);
fixed Curve(fixed, fixed, fixed, fixed, fixed);
fixed AltCurve(fixed, fixed, fixed, fixed, fixed);

// Strings
OptionalArgs(1) bool StartsWith(str, str, bool);
OptionalArgs(1) bool EndsWith(str, str, bool);
OptionalArgs(1) bool Contains(str, str, bool);
str FormatTime(int);
str ColorCodeFromName(str);

// Debugging/Cheats
NamedScript Console void Cheat(int);
NamedScript Console void ModStat(int, int);
NamedScript Console void ModStatXP(int, long int);
NamedScript Console void AllSkills();
NamedScript Console void LookupXPTable(int index);
NamedScript Console void GiveCredits(int);
NamedScript Console void ShopSpecialShuffle();
NamedScript Console void GiveAugs(int, int, int);
NamedScript Console void FullShield();
NamedScript Console void FullLocker(int);
NamedScript Console void GiveCompounds(int);

// System
void CreateTranslations();
NamedScript DECORATE void UpdateInput(int, bool, bool);
bool CheckInput(int, int /* = KEY_PRESSED */, bool, int);
bool CheckInputHelper(int, int, int /* = KEY_PRESSED */, int);
void ClearInfo(CharSaveInfo *);

// Dynamic Arrays
int zsDynArrayUtils(str, int, int, int);

NamedScript DECORATE void SetDebugMode();

NamedScript Console void DumpDrops();

#endif
