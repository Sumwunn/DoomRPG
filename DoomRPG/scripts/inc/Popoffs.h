#ifndef DOOMRPG_POPOFFS_H
#define DOOMRPG_POPOFFS_H

#include "Defs.h"

NamedScript void DamageNumbers();
NamedScript void InfoPopoffs();
NamedScript DECORATE void ModulePopoffs();

void Popoff(int, int, int, str, bool);
int GetDamageNumbersDelay();

#endif
