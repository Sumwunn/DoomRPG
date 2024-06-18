#ifndef DRPG_TURRET_H
#define DRPG_TURRET_H

#include "Defs.h"

extern TurretUpgrade RPGMap TurretUpgradeData[MAX_UPGRADES];

NamedScript Type_ENTER void TurretCommandWheel();
NamedScript DECORATE void TurretSetOffset(int, int, int);
NamedScript DECORATE void TurretUseAmmo(int);
NamedScript DECORATE int TurretGetProjectileDamage(int);
NamedScript DECORATE int TurretGetProjectileProperty(int);

NamedScript Type_ENTER void TurretLoop();
NamedScript Type_ENTER void TurretLoopMaintenance();

// Turret AI
NamedScript void TurretAI(int);
NamedScript bool TurretCheckTarget();
NamedScript bool TurretCheckTargetIsSelf();
NamedScript bool TurretCheckPlayerTarget();
NamedScript bool TurretCheckTargetSuitable();
NamedScript bool TurretCheckTargetDead();
NamedScript void TurretGetAttention(int, int);
NamedScript bool TurretForceTargetTID(int);
NamedScript void TurretSyncWithPlayerTarget();
NamedScript int TurretGetOwningPlayer();
NamedScript int TurretGetTargetTID();
NamedScript bool TurretWantsToSwitchToPlayerTarget();

void TurretFly(fixed, fixed, fixed);
void TurretTurn(fixed, fixed);

void BuildTurretData();
bool TurretTeleport(int);
void TurretMaintenance();
bool TurretLoadAmmo(int);
void TurretCommand(int);
int TurretUpgradeCost(int);
void TurretPassVars();

void TurretSpawn();
void TurretDespawn();

#endif
