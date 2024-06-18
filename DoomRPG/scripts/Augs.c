#include "RPG.h"

#include "Arena.h"
#include "Augs.h"
#include "Outpost.h"
#include "Map.h"
#include "Shield.h"
#include "Stats.h"
#include "Skills.h"
#include "Utils.h"

AugInfo RPGMap AugData[AUG_MAX] =
{
    // Name, Max Level, Description...

    // Augs
    {
        .Name = "Muscle Replacer",
        .MaxLevel = 6,
        .Description =
        {
            "+10% Total Damage",
            "+15% Total Damage",
            "+20% Total Damage",
            "+30% Total Damage",
            "+40% Total Damage",
            "+50% Total Damage"
        },
        .TokenActor = "DRPGAugTokenStrength"
    },
    {
        .Name = "Dermal Armor",
        .MaxLevel = 5,
        .Description =
        {
            "+5% Total Damage Reduction",
            "+10% Total Damage Reduction",
            "+15% Total Damage Reduction",
            "+20% Total Damage Reduction",
            "+25% Total Damage Reduction"
        },
        .TokenActor = "DRPGAugTokenDefense"
    },
    {
        .Name = "Pain Inhibitor",
        .MaxLevel = 6,
        .Description =
        {
            "+50% HP Regen Amount",
            "+25% Max HP Amount",
            "+50% Max HP Amount",
            "+15% Status Effect Resist",
            "+20% Status Effect Resist",
            "+25% Status Effect Resist"
        },
        .TokenActor = "DRPGAugTokenVitality"
    },
    {
        .Name = "PSI Projector",
        .MaxLevel = 8,
        .Description =
        {
            "+50% EP Regen Amount",
            "+25% Max EP Amount",
            "+50% Max EP Amount",
            "+50% Aura Timer, +1 Aura Max Stack",
            "+10% Skill Cost Refund",
            "+15% Skill Cost Refund",
            "+20% Skill Cost Refund",
            "+25% Skill Cost Refund"
        },
        .TokenActor = "DRPGAugTokenEnergy"
    },
    {
        .Name = "Adrenaline Booster",
        .MaxLevel = 8,
        .Description =
        {
            "+25% HP Regen Rate",
            "+25% EP Regen Rate",
            "+50% HP Regen Amount",
            "+50% EP Regen Amount",
            "-5 Seconds Toxicity Timer, -10% Stims Toxicity",
            "-10 Seconds Toxicity Timer, -15% Stims Toxicity",
            "-15 Seconds Toxicity Timer, -20% Stims Toxicity",
            "-20 Seconds Toxicity Timer, -30% Stims Toxicity"
        },
        .TokenActor = "DRPGAugTokenRegen"
    },
    {
        .Name = "Wired Reflexes",
        .MaxLevel = 5,
        .Description =
        {
            "+5% Weapon Speed, +2% Survival Bonus",
            "+10% Weapon Speed, +4% Survival Bonus",
            "+15% Weapon Speed, +6% Survival Bonus",
            "+20% Weapon Speed, +8% Survival Bonus",
            "+25% Weapon Speed, +10% Survival Bonus"
        },
        .TokenActor = "DRPGAugTokenAgility"
    },
    {
        .Name = "Weight Distributor",
        .MaxLevel = 3,
        .Description =
        {
            "+25% Ammo Limits",
            "+50% Ammo Limits",
            "+100% Ammo Limits"
        },
        .TokenActor = "DRPGAugTokenCapacity"
    },
    {
        .Name = "Precognition Unit",
        .MaxLevel = 4,
        .Description =
        {
            "+50% Luck Drop Chances",
            "+100% Luck Drop Chances",
            "+200% Luck Drop Chances",
            "+300% Luck Drop Chances"
        },
        .TokenActor = "DRPGAugTokenLuck"
    },
    {
        .Name = "Enhanced Battery",
        .MaxLevel = 8,
        .Description =
        {
            "Capacity +25%, Self Recharge Rate +0.05/sec",
            "Capacity +50%, Self Recharge Rate +0.1/sec",
            "Capacity +75%, Self Recharge Rate +0.2/sec",
            "Capacity +100%, Self Recharge Rate +0.3/sec",
            "Capacity +125%, Self Recharge Rate +0.4/sec",
            "Capacity +150%, Self Recharge Rate +0.5/sec",
            "Capacity +175%, Self Recharge Rate +0.75/sec",
            "Capacity +200%, Self Recharge Rate +1/sec"
        },
        .TokenActor = "DRPGAugTokenBattery"
    },
    {
        .Name = "Telepathic Synchronizer",
        .MaxLevel = 8,
        .Description =
        {
            "Summoned Increased Vitality (depending on your Energy)",
            "Summoned Increased Defense (depending on your Energy)",
            "Summoned Increased Strength (depending on your Energy)",
            "Summoned Increased HP Regen Rate",
            "+10% Summoned Extra Power, -10% Summoning Reduced Cost",
            "+15% Summoned Extra Power, -15% Summoning Reduced Cost",
            "+20% Summoned Extra Power, -20% Summoning Reduced Cost",
            "+25% Summoned Extra Power, -25% Summoning Reduced Cost"
        },
        .TokenActor = "DRPGAugTokenSummoner"
    }
};

NamedScript DECORATE OptionalArgs(1) void DisableAugs(bool NoDrain)
{
    // Disable Augs and give tokens designating which Augs were disabled
    for (int i = 0; i < AUG_MAX; i++)
    {
        if (Player.Augs.Active[i])
        {
            // Disable Aug
            Player.Augs.Active[i] = false;
            SetInventory(AugData[i].TokenActor, Player.Augs.Level[i]);

            // Screen fuckery
            SetHudSize(640, 480, false);
            SetFont("AugDView");
            HudMessage("A");
            EndHudMessage(HUDMSG_FADEOUT | HUDMSG_ADDBLEND, 0, "Untranslated", 320.0, 240.0, 3.0, 0.5);
        }
    }

    // Reset Aug slots
    Player.Augs.SlotsUsed = 0;

    // Halve your current battery amount
    if (!NoDrain)
        Player.Augs.Battery /= 2;

    // Disabled sound
    ActivatorSound("aug/disable", 127);
}

NamedScript KeyBind void ReactivateDisabledAugs()
{
    // Check that we *can* activate augs.
    if (Player.StatusType[SE_EMP] || Player.Augs.Battery <= 0)
        return;

    for (int i = 0; i < AUG_MAX; i++)
    {
        // If there aren't enough slots left to equip any more augs, then don't try to activate any more.
        if (Player.Augs.SlotsUsed >= Player.Augs.Slots)
            break;

        if (CheckInventory(AugData[i].TokenActor) && !Player.Augs.Active[i] && Player.Augs.Level[i] > 0)
            EquipAug(i);
    }

    ClearDisabledAugs();
}

void ClearDisabledAugs()
{
    for (int i = 0; i < AUG_MAX; i++)
        TakeInventory(AugData[i].TokenActor, AugData[i].MaxLevel);
}

NamedScript DECORATE void AddBattery(int Amount)
{
    int PrevBattery = Player.Augs.Battery;

    Player.Augs.Battery += Amount;

    if (Player.Augs.Battery > Player.Augs.BatteryMax)
    {
        Player.Augs.Battery = Player.Augs.BatteryMax;
        DrawBattery();
    }

    if (PrevBattery != Player.Augs.Battery)
        DrawBattery();
}

void CheckAugSlots()
{
    Player.Augs.Slots = Player.Augs.BaseSlots + CheckInventory("DRPGAugSlotUpgrade");

    // AW-G01 Accessory handling
    if (Player.Shield.Accessory && Player.Shield.Accessory->PassiveEffect == SHIELD_PASS_AUGSLOT && Player.Shield.Active)
        Player.Augs.Slots++;

    // Cap max Aug slots at the max amount of available Augs
    if (Player.Augs.Slots > AUG_MAX) Player.Augs.Slots = AUG_MAX;

    // Unequip Augs if you pass the limit (sell a slot upgrade, change difficulty, etc)
    if (Player.Augs.SlotsUsed > Player.Augs.Slots)
        for (int i = AUG_MAX - 1; i >= 0 && Player.Augs.SlotsUsed > Player.Augs.Slots; i--)
            if (Player.Augs.Active[i])
            {
                Player.Augs.Active[i] = false;
                Player.Augs.SlotsUsed--;
                break;
            }
}

void CheckAugs()
{
    // Sanity check to prevent negative slots used
    if (Player.Augs.SlotsUsed < 0)
        Player.Augs.SlotsUsed = 0;

    // Determine max Battery
    Player.Augs.BatteryMax = 100;

    // Battery Aug
    if (Player.Augs.Active[AUG_BATTERY])
    {
        if (Player.Augs.CurrentLevel[AUG_BATTERY] == 1)
            Player.Augs.BatteryMax += 25;
        if (Player.Augs.CurrentLevel[AUG_BATTERY] == 2)
            Player.Augs.BatteryMax += 50;
        if (Player.Augs.CurrentLevel[AUG_BATTERY] == 3)
            Player.Augs.BatteryMax += 75;
        if (Player.Augs.CurrentLevel[AUG_BATTERY] == 4)
            Player.Augs.BatteryMax += 100;
        if (Player.Augs.CurrentLevel[AUG_BATTERY] == 5)
            Player.Augs.BatteryMax += 125;
        if (Player.Augs.CurrentLevel[AUG_BATTERY] == 6)
            Player.Augs.BatteryMax += 150;
        if (Player.Augs.CurrentLevel[AUG_BATTERY] == 7)
            Player.Augs.BatteryMax += 175;
        if (Player.Augs.CurrentLevel[AUG_BATTERY] >= 8)
            Player.Augs.BatteryMax += 200;
    }

    // Play energy drained sound
    if (Player.Augs.Battery <= 0 && Player.Augs.SlotsUsed > 0)
        ActivatorSound("aug/dead", 127);

    // Disable Augs if your battery is dead
    if (Player.Augs.Battery <= 0)
    {
        for (int i = 0; i < AUG_MAX; i++)
            if (Player.Augs.Active[i])
                Player.Augs.Active[i] = false;

        // Reset the amount of slots used
        Player.Augs.SlotsUsed = 0;
    }

    // Battery Handling
    if ((!CurrentLevel->UACBase || ArenaActive || MarinesHostile) && !CheckInventory("PowerTimeFreezer"))
    {
        // Reset drain
        Player.Augs.BatteryDrain = 0;

        // Calculate Battery Drain
        for (int i = 0; i < AUG_MAX; i++)
            if (Player.Augs.Active[i] && i != AUG_BATTERY)
                Player.Augs.BatteryDrain += Player.Augs.CurrentLevel[i];

        // Calculate Battery Drain for Tutrret
        if (Player.Turret.Active && Player.Turret.AugBattery)
        {
            // Battery Drain for Turret Active
            Player.Augs.BatteryDrain++;

            // Battery Drain for Tutrret's Weapon Module Active and Upgrades
            if (GetUserVariable(Player.Turret.TID, "user_weapon") == TW_BULLET) Player.Augs.BatteryDrain += 1.0 + (Player.Turret.Upgrade[TU_WEAPON_BULLET_DAMAGE] + TU_WEAPON_BULLET_ROF) * (1.0 - ((fixed)Player.Turret.Upgrade[TU_BATTERY_CAPACITY] / 10.0)) * 0.5;
            if (GetUserVariable(Player.Turret.TID, "user_weapon") == TW_PELLET) Player.Augs.BatteryDrain += 1.0 + (Player.Turret.Upgrade[TU_WEAPON_PELLET_DAMAGE] + TU_WEAPON_PELLET_ROF + TU_WEAPON_PELLET_SPREAD + TU_WEAPON_PELLET_AMOUNT) * (1.0 - ((fixed)Player.Turret.Upgrade[TU_BATTERY_CAPACITY] / 10.0)) * 0.5;
            if (GetUserVariable(Player.Turret.TID, "user_weapon") == TW_ROCKET) Player.Augs.BatteryDrain += 1.0 + (Player.Turret.Upgrade[TU_WEAPON_ROCKET_DAMAGE] + TU_WEAPON_ROCKET_ROF + TU_WEAPON_ROCKET_SEEKING) * (1.0 - ((fixed)Player.Turret.Upgrade[TU_BATTERY_CAPACITY] / 10.0)) * 0.5;
            if (GetUserVariable(Player.Turret.TID, "user_weapon") == TW_PLASMA) Player.Augs.BatteryDrain += 1.0 + (Player.Turret.Upgrade[TU_WEAPON_PLASMA_DAMAGE] + TU_WEAPON_PLASMA_ROF) * (1.0 - ((fixed)Player.Turret.Upgrade[TU_BATTERY_CAPACITY] / 10.0)) * 0.5;
            if (GetUserVariable(Player.Turret.TID, "user_weapon") == TW_RAILGUN) Player.Augs.BatteryDrain += 1.0 + (Player.Turret.Upgrade[TU_WEAPON_RAILGUN_DAMAGE] + TU_WEAPON_RAILGUN_ROF + TU_WEAPON_RAILGUN_RIPPING) * (1.0 - ((fixed)Player.Turret.Upgrade[TU_BATTERY_CAPACITY] / 10.0)) * 0.5;

            // Battery Drain for Tutrret's Ammo Module Active
            if (Player.Turret.Upgrade[TU_AMMO_NANOGEN] > 0)
            {
                // Battery Drain for Tutrret's Ammo Nano-Generators Module Active
                Player.Augs.BatteryDrain += 1.0 + Player.Turret.Upgrade[TU_AMMO_NANOGEN] * (1.0 - ((fixed)Player.Turret.Upgrade[TU_BATTERY_CAPACITY] / 10.0)) * 0.5;

                // Battery Drain for Tutrret's Ammo Nano-Generators Module Upgrades
                if (Player.Turret.Upgrade[TU_AMMO_NANOGEN_BULLET] && Player.Turret.BulletAmmo < Player.Turret.BulletAmmoMax) Player.Augs.BatteryDrain++;
                if (Player.Turret.Upgrade[TU_AMMO_NANOGEN_SHELL] && Player.Turret.ShellAmmo < Player.Turret.ShellAmmoMax) Player.Augs.BatteryDrain++;
                if (Player.Turret.Upgrade[TU_AMMO_NANOGEN_ROCKET] && Player.Turret.RocketAmmo < Player.Turret.RocketAmmoMax) Player.Augs.BatteryDrain++;
                if (Player.Turret.Upgrade[TU_AMMO_NANOGEN_CELL] && Player.Turret.PlasmaAmmo < Player.Turret.PlasmaAmmoMax) Player.Augs.BatteryDrain++;
                if (Player.Turret.Upgrade[TU_AMMO_NANOGEN_CELL] && Player.Turret.RailAmmo < Player.Turret.RailAmmoMax) Player.Augs.BatteryDrain++;
            }
        }

        // Decrease Battery
        if ((Player.Augs.SlotsUsed > 0 || (Player.Turret.Active && Player.Turret.AugBattery)) && (Timer() % 35) == 0)
        {
            Player.Augs.Battery -= (Player.Augs.BatteryDrain * 0.055);
            DrawBattery();
        }

        // Permanently Display battery amount on the HUD if CVAR is set
        if ((Player.Augs.SlotsUsed > 0 && GetActivatorCVar("drpg_aug_alwaysshow")) || GetActivatorCVar("drpg_hud_preview"))
            DrawBattery();
    }

    // Sanity check to prevent battery from going negative
    if (Player.Augs.Battery < 0)
        Player.Augs.Battery = 0;

    // Sanity check to prevent Battery from exceeding max amount
    if (Player.Augs.Battery > Player.Augs.BatteryMax)
        Player.Augs.Battery = Player.Augs.BatteryMax;

    // Battery Recharging
    if ((!CurrentLevel->UACBase || ArenaActive || MarinesHostile) && Player.Augs.Active[AUG_BATTERY] && Player.Augs.Battery > 0 && Player.Augs.Battery < Player.Augs.BatteryMax)
        if ((Timer() % 35) == 0)
        {
            fixed Charge;
            if (Player.Augs.CurrentLevel[AUG_BATTERY] == 1)
                Charge = 0.05;
            else if (Player.Augs.CurrentLevel[AUG_BATTERY] == 2)
                Charge = 0.10;
            else if (Player.Augs.CurrentLevel[AUG_BATTERY] == 3)
                Charge = 0.20;
            else if (Player.Augs.CurrentLevel[AUG_BATTERY] == 4)
                Charge = 0.30;
            else if (Player.Augs.CurrentLevel[AUG_BATTERY] == 5)
                Charge = 0.40;
            else if (Player.Augs.CurrentLevel[AUG_BATTERY] == 6)
                Charge = 0.50;
            else if (Player.Augs.CurrentLevel[AUG_BATTERY] == 7)
                Charge = 0.75;
            else if (Player.Augs.CurrentLevel[AUG_BATTERY] >= 8)
                Charge = 1.00;

            if (GetCVar("drpg_regen_movement") && IsPlayerMoving())
                Charge += GetCVar("drpg_move_aug_battery_regen") / 100.0;

            Player.Augs.Battery += Charge;
            DrawBattery();
        }

    // Strength Aug
    if (Player.Augs.Active[AUG_STRENGTH])
    {
        if (Player.Augs.CurrentLevel[AUG_STRENGTH] == 1)
            Player.DamageMult += 0.101;
        if (Player.Augs.CurrentLevel[AUG_STRENGTH] == 2)
            Player.DamageMult += 0.151;
        if (Player.Augs.CurrentLevel[AUG_STRENGTH] == 3)
            Player.DamageMult += 0.201;
        if (Player.Augs.CurrentLevel[AUG_STRENGTH] == 4)
            Player.DamageMult += 0.301;
        if (Player.Augs.CurrentLevel[AUG_STRENGTH] == 5)
            Player.DamageMult += 0.401;
        if (Player.Augs.CurrentLevel[AUG_STRENGTH] >= 6)
            Player.DamageMult += 0.501;
    }

    // Defense Aug
    if (Player.Augs.Active[AUG_DEFENSE])
        Player.DamageFactor -= (fixed)Player.Augs.CurrentLevel[AUG_DEFENSE] / 20.0;
    //Player.DamageFactor *= (1.0 - (fixed)Player.Augs.CurrentLevel[AUG_DEFENSE] * 0.05);

    // Vitality Aug
    if (Player.Augs.Active[AUG_VITALITY])
    {
        if (Player.Augs.CurrentLevel[AUG_VITALITY] >= 1)
            Player.HPAmount *= 1.5;
        if (Player.Augs.CurrentLevel[AUG_VITALITY] == 2)
            Player.HealthMax *= 1.25;
        if (Player.Augs.CurrentLevel[AUG_VITALITY] >= 3)
            Player.HealthMax *= 1.5;
        if (Player.Augs.CurrentLevel[AUG_VITALITY] == 4)
            Player.StatusEffectResist += 15;
        if (Player.Augs.CurrentLevel[AUG_VITALITY] == 5)
            Player.StatusEffectResist += 20;
        if (Player.Augs.CurrentLevel[AUG_VITALITY] >= 6)
            Player.StatusEffectResist += 25;
    }

    // Energy Aug
    if (Player.Augs.Active[AUG_ENERGY])
    {
        if (Player.Augs.CurrentLevel[AUG_ENERGY] >= 1)
            Player.EPAmount *= 1.5;
        if (Player.Augs.CurrentLevel[AUG_ENERGY] == 2)
            Player.EPMax *= 1.25;
        if (Player.Augs.CurrentLevel[AUG_ENERGY] >= 3)
            Player.EPMax *= 1.5;
    }
    if (Player.Augs.Active[AUG_ENERGY] && Player.Augs.CurrentLevel[AUG_ENERGY] >= 4)
        Player.AuraBonus = true;
    else
        Player.AuraBonus = false;

    // Regeneration Aug
    if (Player.Augs.Active[AUG_REGENERATION])
    {
        if (Player.Augs.CurrentLevel[AUG_REGENERATION] >= 1)
        {
            Player.HPTime /= 1.25;
        }
        if (Player.Augs.CurrentLevel[AUG_REGENERATION] >= 2)
        {
            Player.EPTime /= 1.25;
        }
        if (Player.Augs.CurrentLevel[AUG_REGENERATION] >= 3)
        {
            Player.HPAmount *= 1.5;
        }
        if (Player.Augs.CurrentLevel[AUG_REGENERATION] >= 4)
        {
            Player.EPAmount *= 1.5;
        }
        if (Player.Augs.CurrentLevel[AUG_REGENERATION] == 5)
            Player.ToxicityRegenBonus += 5;
        if (Player.Augs.CurrentLevel[AUG_REGENERATION] == 6)
            Player.ToxicityRegenBonus += 10;
        if (Player.Augs.CurrentLevel[AUG_REGENERATION] == 7)
            Player.ToxicityRegenBonus += 15;
        if (Player.Augs.CurrentLevel[AUG_REGENERATION] >= 8)
            Player.ToxicityRegenBonus += 20;
    }

    // Agility Aug
    if (Player.Augs.Active[AUG_AGILITY])
    {
        if (Player.Augs.CurrentLevel[AUG_AGILITY] == 1)
        {
            Player.WeaponSpeed += 5;
            Player.SurvivalBonus += 2;
        }
        if (Player.Augs.CurrentLevel[AUG_AGILITY] == 2)
        {
            Player.WeaponSpeed += 10;
            Player.SurvivalBonus += 4;
        }
        if (Player.Augs.CurrentLevel[AUG_AGILITY] == 3)
        {
            Player.WeaponSpeed += 15;
            Player.SurvivalBonus += 6;
        }
        if (Player.Augs.CurrentLevel[AUG_AGILITY] == 4)
        {
            Player.WeaponSpeed += 20;
            Player.SurvivalBonus += 8;
        }
        if (Player.Augs.CurrentLevel[AUG_AGILITY] >= 5)
        {
            Player.WeaponSpeed += 25;
            Player.SurvivalBonus += 10;
        }
    }

    // Capacity Aug
    if (Player.Augs.Active[AUG_CAPACITY])
    {
        fixed AmmoMult;

        if (Player.Augs.CurrentLevel[AUG_CAPACITY] == 1)
            AmmoMult = 1.25;
        if (Player.Augs.CurrentLevel[AUG_CAPACITY] == 2)
            AmmoMult = 1.5;
        if (Player.Augs.CurrentLevel[AUG_CAPACITY] >= 3)
            AmmoMult = 2;

        SetAmmoCapacity("Clip", (60 + Player.CapacityTotal * 10) * AmmoMult);
        SetAmmoCapacity("Shell", (20 + Player.CapacityTotal * 2) * AmmoMult);
        SetAmmoCapacity("RocketAmmo", (2 + Player.CapacityTotal * 0.6) * AmmoMult);
        SetAmmoCapacity("Cell", (Player.CapacityTotal * 10) * AmmoMult);
    }

    // Luck Aug
    if (Player.Augs.Active[AUG_LUCK])
    {
        fixed LuckMult;

        if (Player.Augs.CurrentLevel[AUG_LUCK] == 1)
            LuckMult = 1.5;
        if (Player.Augs.CurrentLevel[AUG_LUCK] == 2)
            LuckMult = 2;
        if (Player.Augs.CurrentLevel[AUG_LUCK] == 3)
            LuckMult = 3;
        if (Player.Augs.CurrentLevel[AUG_LUCK] >= 4)
            LuckMult = 4;

        Player.HealthChance *= LuckMult;
        Player.EPChance *= LuckMult;
        Player.AmmoChance *= LuckMult;
        Player.TurretChance *= LuckMult;
        Player.ModuleChance *= LuckMult;
        Player.ArmorChance *= LuckMult;
        Player.WeaponChance *= LuckMult;
        Player.ShieldChance *= LuckMult;
        Player.AugChance *= LuckMult;
    }
}

void AugDamage(int Amount)
{
    // Lightning damage type increases your Battery
    if (Player.DamageType == DT_LIGHTNING && Amount / 50 > 0)
        AddBattery(Amount / 50);
}

void EquipAug(int Aug)
{
    // If you don't have this aug yet, return
    if (Player.Augs.Level[Aug] == 0 || Player.Augs.CurrentLevel[Aug] == 0)
    {
        PrintError("This augmentation has\nnot been activated yet");
        ActivatorSound("menu/error", 127);
        return;
    }

    // If battery is dead, return
    if (Player.Augs.Battery <= 0)
    {
        PrintError("Your augmentation\nbattery is depleted");
        ActivatorSound("menu/error", 127);
        return;
    }

    if (Player.Augs.Level[Aug] > 0)
    {
        if (Player.Augs.Active[Aug])
        {
            ActivatorSound("aug/equip", 127);
            Player.Augs.SlotsUsed--;
            Player.Augs.Active[Aug] = false;
        }
        else
        {
            Player.Augs.SlotsUsed++;

            if (Player.Augs.SlotsUsed > Player.Augs.Slots)
            {
                PrintError("You are already using all\nof your augmentation slots");
                ActivatorSound("menu/error", 127);
                Player.Augs.SlotsUsed--;
            }
            else
            {
                ActivatorSound("aug/equip", 127);
                Player.Augs.Active[Aug] = true;
            }
        }
    }
}