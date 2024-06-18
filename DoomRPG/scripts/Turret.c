#include "Defs.h"

#include <stdlib.h>

#include "Arena.h"
#include "Augs.h"
#include "Crate.h"
#include "ItemData.h"
#include "Outpost.h"
#include "Map.h"
#include "Menu.h"
#include "RPG.h"
#include "Stims.h"
#include "Turret.h"
#include "Utils.h"

TurretUpgrade RPGMap TurretUpgradeData[MAX_UPGRADES] =
{
    // Build
    {
        "Build Turret", 1, 5,
        "Build the portable turret",
        "",
        "" // Generated in BuildTurretData()
    },

    // --------------------------------------------------
    // Weapons
    //

    // Bullet
    {
        "Weapon Module - Bullet", 1, 2,
        "The turret is able to fire rapid-fire low-damage bullets",
        "",
        "Issuing this command will load the bullet weapon module"
    },
    {
        "Weapon Module - Bullet - Damage", 20, 3,
        "Increases the damage of the turret's bullets",
        "",
        ""
    },
    {
        "Weapon Module - Bullet - Capacity", 10, 3,
        "Increases the storage capacity of bullets the turret can hold",
        "",
        "issuing this command will insert bullets"
    },
    {
        "Weapon Module - Bullet - Rate of Fire", 5, 5,
        "Increases the fire rate of the turret's bullets",
        "",
        ""
    },

    // Pellet
    {
        "Weapon Module - Pellet", 1, 3,
        "The turret is able to fire shotgun-like pellet spread shots",
        "",
        "Issuing this command will load the pellet weapon module"
    },
    {
        "Weapon Module - Pellet - Damage", 20, 3,
        "Increases the damage of the turret's pellets",
        "",
        ""
    },
    {
        "Weapon Module - Pellet - Capacity", 10, 3,
        "Increases the storage capacity of shells the turret can hold",
        "",
        "issuing this command will insert shells"
    },
    {
        "Weapon Module - Pellet - Rate of Fire", 5, 5,
        "Increases the fire rate of the turret's pellets",
        "",
        ""
    },
    {
        "Weapon Module - Pellet - Spread", 5, 5,
        "Decreases the spread of the pellets fired",
        "",
        ""
    },
    {
        "Weapon Module - Pellet - Amount", 5, 5,
        "Increases the number of pellets fired per shot",
        "",
        ""
    },

    // Rocket
    {
        "Weapon Module - Rocket", 1, 5,
        "The turret is able to fire explosive rockets",
        "",
        "Issuing this command will load the rocket weapon module"
    },
    {
        "Weapon Module - Rocket - Damage", 20, 3,
        "Increases the damage of the turret's rockets",
        "",
        ""
    },
    {
        "Weapon Module - Rocket - Capacity", 10, 3,
        "Increases the storage capacity of rockets the turret can hold",
        "",
        "issuing this command will insert rockets"
    },
    {
        "Weapon Module - Rocket - Rate of Fire", 5, 5,
        "Increases the fire rate of the turret's rockets",
        "",
        ""
    },
    {
        "Weapon Module - Rocket - Seeking", 5, 10,
        "Allows fired rockets to seek targets",
        "",
        ""
    },

    // Plasma
    {
        "Weapon Module - Plasma", 1, 10,
        "The turret is able to fire superheated balls of plasma",
        "",
        "Issuing this command will load the plasma weapon module"
    },
    {
        "Weapon Module - Plasma - Damage", 20, 3,
        "Increases the damage of the turret's plasma shots",
        "",
        ""
    },
    {
        "Weapon Module - Plasma - Capacity", 10, 3,
        "Increases the storage capacity of cells used for plasma the turret can hold",
        "",
        "issuing this command will insert cells"
    },
    {
        "Weapon Module - Plasma - Rate of Fire", 5, 5,
        "Increases the fire rate of the turret's plasma shots",
        "",
        ""
    },

    // Railgun
    {
        "Weapon Module - Railgun", 1, 20,
        "The turret is able to fire piercing railgun shots",
        "",
        "Issuing this command will load the railgun weapon module"
    },
    {
        "Weapon Module - Railgun - Damage", 20, 3,
        "Increases the damage of the turret's railgun shots",
        "",
        ""
    },
    {
        "Weapon Module - Railgun - Capacity", 10, 3,
        "Increases the storage capacity of cells used for the railgun the turret can hold",
        "",
        "issuing this command will insert cells"
    },
    {
        "Weapon Module - Railgun - Rate of Fire", 5, 5,
        "Increases the fire rate of the turret's railgun shots",
        "",
        ""
    },
    {
        "Weapon Module - Railgun - Piercing", 1, 10,
        "Allows the railgun to pierce through targets",
        "",
        ""
    },
    // Ammo
    {
        "Ammo Module - Autoloader", 1, 5,
        "Autoloads ammo from your own ammo pool (only for activated Weapon Module)",
        "",
        "issuing this command will toggle autoloading on and off"
    },
    {
        "Ammo Module - Nano-Generators", 10, 5,
        "Allows the turret to slowly generate it's own ammo supplies",
        "Upgrades increase generation speed",
        ""
    },
    {
        "Ammo Module - Nano-Generators - Bullets", 1, 10,
        "Allows the turret to slowly generate bullets",
        "",
        ""
    },
    {
        "Ammo Module - Nano-Generators - Shells", 1, 20,
        "Allows the turret to slowly generate shells",
        "",
        ""
    },
    {
        "Ammo Module - Nano-Generators - Rockets", 1, 50,
        "Allows the turret to slowly generate rockets",
        "",
        ""
    },
    {
        "Ammo Module - Nano-Generators - Cells", 1, 50,
        "Allows the turret to slowly generate cells",
        "",
        ""
    },

    // --------------------------------------------------
    // Armor
    //

    // Plating
    {
        "Armor Plating - Reinforcement", 10, 5,
        "Reinforce the plating of the turret, allowing it to take more damage",
        "Upgrades Increase the maximum health of the turret",
        ""
    },
    {
        "Armor Plating - Melee", 20, 3,
        "Plating which protects against melee damage",
        "Upgrades increase protection amount",
        ""
    },
    {
        "Armor Plating - Bullet", 20, 3,
        "Plating which protects against bullet damage",
        "Upgrades increase protection amount",
        ""
    },
    {
        "Armor Plating - Fire", 20, 3,
        "Plating which protects against fire damage",
        "Upgrades increase protection amount",
        ""
    },
    {
        "Armor Plating - Plasma", 20, 3,
        "Plating which protects against plasma damage",
        "Upgrades increase protection amount",
        ""
    },
    {
        "Armor Plating - Blast", 1, 20,
        "Plating which protects against blast damage",
        "",
        ""
    },

    // Modules
    {
        "Armor Module - Projectile Reflection", 3, 10,
        "Reflects projectiles fired at the turret",
        "Upgrades improve the abilities of the reflection",
        ""
    },
    {
        "Armor Module - Repair Nanites", 10, 5,
        "Slowly Repairs the health of the turret",
        "Upgrades increase repair speed",
        ""
    },
    {
        "Armor Module - Phase Inverter", 10, 5,
        "Randomly shifts the turret in and out of visibility",
        "Upgrades increase the time that the turret can stay invisible",
        ""
    },

    // --------------------------------------------------
    // Battery
    //

    {
        "Battery - AUG Battery Adapter", 1, 10,
        "Use turret from your AUG battery",
        "",
        "Issuing this command will toggle AUG Battery Adapter on and off"
    },
    {
        "Battery - Capacity", 10, 5,
        "Increases the capacity of the turret's battery",
        "Upgrades increase maximum battery capacity (also reduces consumption from AUG battery)",
        ""
    },

    // Generators
    {
        "Generator - Kinetic", 10, 5,
        "Charges the turret's battery based on movement",
        "Upgrades decrease interval between charges",
        ""
    },
    {
        "Generator - Illumination", 10, 5,
        "Charges the turret's battery based on light level",
        "Upgrades decrease interval between charges",
        ""
    },
    {
        "Generator - Force", 10, 5,
        "Charges the turret's battery from bullet and melee based damage",
        "Upgrades increase charge amount when recieving this kind of damage",
        ""
    },
    {
        "Generator - Thermal", 10, 5,
        "Charges the turret's battery from fire based damage",
        "Upgrades increase charge amount when recieving this kind of damage",
        ""
    },
    {
        "Generator - Plasma", 10, 5,
        "Charges the turret's battery from plasma based damage",
        "Upgrades increase charge amount when recieving this kind of damage",
        ""
    },
    {
        "Generator - Nuclear", 10, 5,
        "Charges the turret's battery from toxicity and radiation based damage",
        "Upgrades increase charge amount when recieving this kind of damage",
        ""
    },

    // --------------------------------------------------
    // Hardware
    //

    {
        "Hardware - Battery Charge Bus", 10, 5,
        "Decreases the time it takes for the turret's battery to be recharged when sent back to the Outpost",
        "Upgrades decrease charging time when in maintenance (outside the outpost)",
        ""
    },
    {
        "Hardware - Build Quality", 10, 5,
        "Decreases the parts it takes to repair the turret",
        "Upgrades decrease the turret parts necessary for repair when in maintenance",
        ""
    },
    {
        "Hardware - Parts Optimization", 10, 5,
        "Decreases the time it takes to repair the turret",
        "Upgrades decrease repair time when in maintenance (outside the outpost)",
        ""
    },
    {
        "Hardware - Specification Optimization", 10, 5,
        "Decreases the time it takes to upgrade and refit the turret",
        "Upgrades decrease refit time when in maintenance (outside the outpost)",
        ""
    },
    {
        "Hardware - Expert Fabrication", 10, 5,
        "Decreases the cost of maintenance",
        "Upgrades decrease cash flow required when in maintenance",
        ""
    },

    // --------------------------------------------------
    // Commands
    //

    {
        "Commands - Recall", 1, 10,
        "A module that recalls the turret to you, cancelling other commands",
        "",
        "Issuing this command will call the turret to you and cancel commands"
    },
    {
        "Commands - Draw Fire", 1, 10,
        "The turret will draw enemies' attention to itself",
        "",
        "" // Generated in BuildTurretData()
    },
    {
        "Commands - Hold Position", 1, 10,
        "The turret will stay and defend its current position",
        "",
        "Issuing this command will set the turret to hold its position"
    },
};

NamedScript Type_ENTER void TurretLoop()
{
    int PlayerNum = PlayerNumber();
    int Health;
    int PrevHealth;

Start:

    // Horrible hack
    // Just entered the level, despawn the turret
    if (Timer() <= 4 && Player.Turret.Init && Player.Turret.Active)
        TurretDespawn();

    // Turret first time init
    if (Player.Turret.Upgrade[TU_BUILD] && !Player.Turret.Init)
    {
        // Initial levels
        if (!Player.Turret.Upgrade[TU_WEAPON_BULLET_CAPACITY])
            Player.Turret.Upgrade[TU_WEAPON_BULLET_CAPACITY] = 1;
        if (!Player.Turret.Upgrade[TU_WEAPON_PELLET_CAPACITY])
            Player.Turret.Upgrade[TU_WEAPON_PELLET_CAPACITY] = 1;
        if (!Player.Turret.Upgrade[TU_WEAPON_ROCKET_CAPACITY])
            Player.Turret.Upgrade[TU_WEAPON_ROCKET_CAPACITY] = 1;
        if (!Player.Turret.Upgrade[TU_WEAPON_PLASMA_CAPACITY])
            Player.Turret.Upgrade[TU_WEAPON_PLASMA_CAPACITY] = 1;
        if (!Player.Turret.Upgrade[TU_WEAPON_RAILGUN_CAPACITY])
            Player.Turret.Upgrade[TU_WEAPON_RAILGUN_CAPACITY] = 1;
        if (!Player.Turret.Upgrade[TU_ARMOR_PLATING])
            Player.Turret.Upgrade[TU_ARMOR_PLATING] = 1;
        if (!Player.Turret.Upgrade[TU_BATTERY_CAPACITY])
            Player.Turret.Upgrade[TU_BATTERY_CAPACITY] = 1;

        // Reset the current weapon
        Player.Turret.Weapon = TW_NONE;

        // Small delay to let the initial upgrades kick in and for total to be calculated
        Delay(1);

        // Initial Health
        if (Player.Turret.TID != 0)
            SetActorProperty(Player.Turret.TID, APROP_Health, Player.Turret.HealthMax);

        // Initial Battery Charge
        Player.Turret.Battery = Player.Turret.BatteryMax;

        // Set Flags
        Player.Turret.Init = true;
        Player.Turret.PaidForRepair = true;
    }

    // Sanity check. Without this, using `kill monsters` (or being really unlucky) while the turret is active will semi-destroy it, making it impossible to deploy or repair.
    if (Player.Turret.Health >= Player.Turret.HealthMax)
    {
        Player.Turret.Destroyed = false;
        Player.Turret.PaidForRepair = true;
    }

    while (Player.Turret.Active && Player.Turret.TID != 0)
    {
        // Pre-health check
        Health = GetActorProperty(Player.Turret.TID, APROP_Health);

        // Prevent Health overflow
        if (Health > Player.Turret.HealthMax)
        {
            Health = Player.Turret.HealthMax;
            SetActorProperty(Player.Turret.TID, APROP_Health, Health);
        }

        // Despawn the turret if itself or the owning player dies
        if (GetActorProperty(Player.TID, APROP_Health) <= 0 || GetActorProperty(Player.Turret.TID, APROP_Health) <= 0)
        {
            Player.Turret.Destroyed = true;
            Player.Turret.PaidForRepair = false;
            TurretDespawn();
            break;
        }

        // Nametag
        SetActorPropertyString(Player.Turret.TID, APROP_Nametag, StrParam("%tS\C-'s Turret", PlayerNumber() + 1));

        // Battery is drained
        if ((Player.Turret.Battery <= 0 && !Player.Turret.AugBattery) || (Player.Turret.Battery <= 0 && Player.Turret.AugBattery && Player.Augs.Battery <= 0))
        {
            TurretDespawn();
            break;
        }

        // Autoloader
        if (Player.Turret.Upgrade[TU_AMMO_AUTOLOADER] && Player.Turret.Autoload)
        {
            if (GetUserVariable(Player.Turret.TID, "user_weapon") == TW_BULLET && Player.Turret.Upgrade[TU_WEAPON_BULLET] && Player.Turret.BulletAmmo <= Player.Turret.BulletAmmoMax - 50)
                TurretLoadAmmo(TU_WEAPON_BULLET);
            if (GetUserVariable(Player.Turret.TID, "user_weapon") == TW_PELLET && Player.Turret.Upgrade[TU_WEAPON_PELLET] && Player.Turret.ShellAmmo <= Player.Turret.ShellAmmoMax - 20)
                TurretLoadAmmo(TU_WEAPON_PELLET);
            if (GetUserVariable(Player.Turret.TID, "user_weapon") == TW_ROCKET && Player.Turret.Upgrade[TU_WEAPON_ROCKET] && Player.Turret.RocketAmmo <= Player.Turret.RocketAmmoMax - 5)
                TurretLoadAmmo(TU_WEAPON_ROCKET);
            if (GetUserVariable(Player.Turret.TID, "user_weapon") == TW_PLASMA && Player.Turret.Upgrade[TU_WEAPON_PLASMA] && Player.Turret.PlasmaAmmo <= Player.Turret.PlasmaAmmoMax - 100)
                TurretLoadAmmo(TU_WEAPON_PLASMA);
            if (GetUserVariable(Player.Turret.TID, "user_weapon") == TW_RAILGUN && Player.Turret.Upgrade[TU_WEAPON_RAILGUN] && Player.Turret.RailAmmo <= Player.Turret.RailAmmoMax - 1)
                TurretLoadAmmo(TU_WEAPON_RAILGUN);
        }

        // Nano Ammo Generators
        if (Player.Turret.Upgrade[TU_AMMO_NANOGEN] > 0 && (Timer() % (35 * (15 - Player.Turret.Upgrade[TU_AMMO_NANOGEN]))) == 0)
        {
            if (Player.Turret.Upgrade[TU_AMMO_NANOGEN_BULLET] && Player.Turret.BulletAmmo < Player.Turret.BulletAmmoMax)
                Player.Turret.BulletAmmo += 10;
            if (Player.Turret.Upgrade[TU_AMMO_NANOGEN_SHELL] && Player.Turret.ShellAmmo < Player.Turret.ShellAmmoMax)
                Player.Turret.ShellAmmo += 4;
            if (Player.Turret.Upgrade[TU_AMMO_NANOGEN_ROCKET] && Player.Turret.RocketAmmo < Player.Turret.RocketAmmoMax)
                Player.Turret.RocketAmmo++;
            if (Player.Turret.Upgrade[TU_AMMO_NANOGEN_CELL] && Player.Turret.PlasmaAmmo < Player.Turret.PlasmaAmmoMax)
                Player.Turret.PlasmaAmmo += 20;
            if (Player.Turret.Upgrade[TU_AMMO_NANOGEN_CELL] && Player.Turret.RailAmmo < Player.Turret.RailAmmoMax)
                Player.Turret.RailAmmo++;
        }

        // Armor/Protection
        if (Timer() % 5 == 0)
        {
            if (Player.Turret.Upgrade[TU_ARMOR_PLATING_MELEE] > 0)
                GiveActorInventory(Player.Turret.TID, StrParam("DRPGTurretProtectionMelee%d", Player.Turret.Upgrade[TU_ARMOR_PLATING_MELEE]), 1);
            if (Player.Turret.Upgrade[TU_ARMOR_PLATING_BULLET] > 0)
                GiveActorInventory(Player.Turret.TID, StrParam("DRPGTurretProtectionBullet%d", Player.Turret.Upgrade[TU_ARMOR_PLATING_BULLET]), 1);
            if (Player.Turret.Upgrade[TU_ARMOR_PLATING_FIRE] > 0)
                GiveActorInventory(Player.Turret.TID, StrParam("DRPGTurretProtectionFire%d", Player.Turret.Upgrade[TU_ARMOR_PLATING_FIRE]), 1);
            if (Player.Turret.Upgrade[TU_ARMOR_PLATING_PLASMA] > 0)
                GiveActorInventory(Player.Turret.TID, StrParam("DRPGTurretProtectionPlasma%d", Player.Turret.Upgrade[TU_ARMOR_PLATING_PLASMA]), 1);
            if (Player.Turret.Upgrade[TU_ARMOR_PLATING_BLAST] > 0)
                GiveActorInventory(Player.Turret.TID, "DRPGTurretProtectionBlast", 1);
            if (Player.Turret.Upgrade[TU_ARMOR_MODULE_REFLECT] > 0)
                GiveActorInventory(Player.Turret.TID, StrParam("DRPGTurretReflection%d", Player.Turret.Upgrade[TU_ARMOR_MODULE_REFLECT]), 1);
        }
        if (Player.Turret.Upgrade[TU_ARMOR_MODULE_REPAIR] > 0)
            if (Health < Player.Turret.HealthMax && (Timer() % (35 * (30 - (Player.Turret.Upgrade[TU_ARMOR_MODULE_REPAIR] * 2.5)))) == 0)
                SetActorProperty(Player.Turret.TID, APROP_Health, Health + 1);
        if (Player.Turret.Upgrade[TU_ARMOR_MODULE_PHASE] > 0 && Health < PrevHealth)
            GiveActorInventory(Player.Turret.TID, StrParam("DRPGTurretPhase%d", Player.Turret.Upgrade[TU_ARMOR_MODULE_PHASE]), 1);
        if (GetActorPowerupTics(Player.Turret.TID, "PowerGhost") > 0)
        {
            SetActorProperty(Player.Turret.TID, APROP_RenderStyle, STYLE_Translucent);
            SetActorPropertyFixed(Player.Turret.TID, APROP_Alpha, 0.5 + (Sin(Timer() / 32.0) * 0.5));
        }
        else
        {
            SetActorProperty(Player.Turret.TID, APROP_RenderStyle, STYLE_Normal);
            SetActorPropertyFixed(Player.Turret.TID, APROP_Alpha, 1.0);
        }

        // Generators
        if (Player.Turret.HitTimer <= 0)
        {
            if (Player.Turret.Upgrade[TU_BATTERY_GENERATOR_KINETIC] > 0 && (AbsFixed(GetActorVelX(0)) + AbsFixed(GetActorVelY(0))) > 0 && (Timer() % (35 * (11 - Player.Turret.Upgrade[TU_BATTERY_GENERATOR_KINETIC]))) == 0)
                Player.Turret.Battery++;
            if (Player.Turret.Upgrade[TU_BATTERY_GENERATOR_ILLUMINATION] > 0 && GetActorLightLevel(Player.Turret.TID) >= 192 && (Timer() % (35 * (11 - Player.Turret.Upgrade[TU_BATTERY_GENERATOR_ILLUMINATION]))) == 0)
                Player.Turret.Battery++;

            // Damage-based Generators
            if (Health < PrevHealth)
            {
                switch (GetUserVariable(Player.Turret.TID, "user_damage_type"))
                {
                case DT_NORMAL:
                case DT_MELEE:
                    if (Player.Turret.Upgrade[TU_BATTERY_GENERATOR_FORCE] > 0)
                        Player.Turret.Battery += Player.Turret.Upgrade[TU_BATTERY_GENERATOR_FORCE];
                    break;
                case DT_FIRE:
                    if (Player.Turret.Upgrade[TU_BATTERY_GENERATOR_THERMAL] > 0)
                        Player.Turret.Battery += Player.Turret.Upgrade[TU_BATTERY_GENERATOR_THERMAL];
                    break;
                case DT_PLASMA:
                case DT_LIGHTNING:
                    if (Player.Turret.Upgrade[TU_BATTERY_GENERATOR_PLASMA] > 0)
                        Player.Turret.Battery += Player.Turret.Upgrade[TU_BATTERY_GENERATOR_PLASMA];
                    break;
                case DT_TOXIC:
                case DT_RADIATION:
                    if (Player.Turret.Upgrade[TU_BATTERY_GENERATOR_NUCLEAR] > 0)
                        Player.Turret.Battery += Player.Turret.Upgrade[TU_BATTERY_GENERATOR_NUCLEAR];
                    break;
                }
            }
        }

        // Timers
        if (Health < PrevHealth && GetUserVariable(Player.Turret.TID, "user_damage_type") != DT_TOXIC && GetUserVariable(Player.Turret.TID, "user_damage_type") != DT_RADIATION)
            Player.Turret.HitTimer = 35 * 5;
        if (Player.Turret.HitTimer > 0)
            Player.Turret.HitTimer--;

        // Check AUG Battery
        if (Player.Turret.AugBattery && Player.Augs.Battery <= 0)
        {
            PrintError("Your aug battery is depleted");
            ActivatorSound("menu/error", 127);
            Player.Turret.AugBattery = false;
        }

        // Drain Battery
        if (((!CurrentLevel->UACBase || ArenaActive || MarinesHostile) && !CheckInventory("PowerTimeFreezer")) && Player.Turret.Battery > 0 && (Timer() % 35) == 0)
        {
            if (!Player.Turret.AugBattery)
                Player.Turret.Battery--;
        }

        // Prevent battery overflow
        if (Player.Turret.Battery > Player.Turret.BatteryMax)
            Player.Turret.Battery = Player.Turret.BatteryMax;

        // Pass info to uservars
        TurretPassVars();

        // Reset damage type
        SetUserVariable(Player.Turret.TID, "user_damage_type", DT_NONE);

        Delay(1);

        // Post-health check
        PrevHealth = Health;
        Player.Turret.Health = Health;

    }

    Delay(1);
    goto Start;
}

NamedScript Type_ENTER void TurretLoopMaintenance()
{
    int MaintCost = 0;
    int MaintCostCalc = 0;

Start:

    // Stats which need to be kept updated
    Player.Turret.HealthMax = 100 * Player.Turret.Upgrade[TU_ARMOR_PLATING];
    Player.Turret.BatteryMax = TURRET_BATTERY_CHARGE * Player.Turret.Upgrade[TU_BATTERY_CAPACITY];
    Player.Turret.BulletAmmoMax = 200 * Player.Turret.Upgrade[TU_WEAPON_BULLET_CAPACITY];
    Player.Turret.ShellAmmoMax = 50 * Player.Turret.Upgrade[TU_WEAPON_PELLET_CAPACITY];
    Player.Turret.RocketAmmoMax = 50 * Player.Turret.Upgrade[TU_WEAPON_ROCKET_CAPACITY];
    Player.Turret.PlasmaAmmoMax = 300 * Player.Turret.Upgrade[TU_WEAPON_PLASMA_CAPACITY];
    Player.Turret.RailAmmoMax = 10 * Player.Turret.Upgrade[TU_WEAPON_RAILGUN_CAPACITY];

    // Prevent ammo overflow
    if (Player.Turret.BulletAmmo > Player.Turret.BulletAmmoMax)
        Player.Turret.BulletAmmo = Player.Turret.BulletAmmoMax;
    if (Player.Turret.ShellAmmo > Player.Turret.ShellAmmoMax)
        Player.Turret.ShellAmmo = Player.Turret.ShellAmmoMax;
    if (Player.Turret.RocketAmmo > Player.Turret.RocketAmmoMax)
        Player.Turret.RocketAmmo = Player.Turret.RocketAmmoMax;
    if (Player.Turret.PlasmaAmmo > Player.Turret.PlasmaAmmoMax)
        Player.Turret.PlasmaAmmo = Player.Turret.PlasmaAmmoMax;
    if (Player.Turret.RailAmmo > Player.Turret.RailAmmoMax)
        Player.Turret.RailAmmo = Player.Turret.RailAmmoMax;

    // Calculate Maintenance Timers
    Player.Turret.ChargeTimer = Player.Turret.BatteryMax - Player.Turret.Battery;
    Player.Turret.RepairTimer = Player.Turret.HealthMax - Player.Turret.Health;

    // Reset maintenance cost this tic
    MaintCost = 0;

    // No Credits for payment
    if (Player.Turret.Maintenance && !CheckInventory("DRPGCredits"))
    {
        Player.Turret.Maintenance = false;
        PrintError("Maintenance has been postponed because you have run out of money");
        ActivatorSound("menu/error", 127);
    }

    // Maintenance
    if (Player.Turret.Maintenance)
    {
        // Charging
        if ((Timer() % (CurrentLevel->UACBase ? 5 : 35 - (Player.Turret.Upgrade[TU_HARDWARE_BATTERY] * 3))) == 0)
            if (Player.Turret.ChargeTimer > 0)
            {
                Player.Turret.Battery++;
                MaintCostCalc = RoundInt((CurrentLevel->UACBase ? 2.0 : 4.0) * (1.0 - ((fixed)Player.Turret.Upgrade[TU_HARDWARE_FABRICATION] / 10.0)));
                if (MaintCostCalc < 1) MaintCostCalc = 1;
                MaintCost += MaintCostCalc;

                // Done
                if (Player.Turret.Battery >= Player.Turret.BatteryMax)
                    ActivatorSound("turret/chargedone", 127);
            }

        // Repairing
        if ((Timer() % (CurrentLevel->UACBase ? 5 : 35 - (Player.Turret.Upgrade[TU_HARDWARE_PART] * 3))) == 0)
        {
            if (Player.Turret.RepairTimer > 0)
            {
                // Turret was destroyed, we need to pay parts up-front to fix it
                if (Player.Turret.Destroyed && !Player.Turret.PaidForRepair)
                {
                    int Parts = Player.Turret.Upgrade[TU_ARMOR_PLATING] - Player.Turret.Upgrade[TU_HARDWARE_BUILD];

                    // If the parts upgrade outweighs the plating upgrade, there is no cost
                    if (Parts <= 0)
                        Player.Turret.PaidForRepair = true;
                    else if (CheckInventory("DRPGTurretPart") >= Parts)
                    {
                        Player.Turret.PaidForRepair = true;
                        TakeInventory("DRPGTurretPart", Parts);
                    }
                }

                if (Player.Turret.PaidForRepair)
                {
                    Player.Turret.Health++;
                    MaintCostCalc = RoundInt((CurrentLevel->UACBase ? 4.0 : 8.0) * (1.0 - ((fixed)Player.Turret.Upgrade[TU_HARDWARE_FABRICATION] / 10.0)));
                    if (MaintCostCalc < 1) MaintCostCalc = 1;
                    MaintCost += MaintCostCalc;
                }

                // Done
                if (Player.Turret.Health >= Player.Turret.HealthMax)
                {
                    ActivatorSound("turret/repairdone", 127);
                    Player.Turret.Destroyed = false;
                }
            }
        }

        // Refitting
        if ((Timer() % (CurrentLevel->UACBase ? 5 : 35 - (Player.Turret.Upgrade[TU_HARDWARE_SPECS] * 3))) == 0)
            if (Player.Turret.RefitTimer > 0)
            {
                Player.Turret.RefitTimer--;
                MaintCostCalc = RoundInt((CurrentLevel->UACBase ? 10.0 : 20.0) * (1.0 - ((fixed)Player.Turret.Upgrade[TU_HARDWARE_FABRICATION] / 10.0)));
                if (MaintCostCalc < 1) MaintCostCalc = 1;
                MaintCost += MaintCostCalc;

                // Done
                if (Player.Turret.RefitTimer <= 0)
                    ActivatorSound("turret/refitdone", 127);
            }

        // Steady credit loss while maintenance is happening
        if (Player.Turret.ChargeTimer > 0 || (Player.Turret.PaidForRepair && Player.Turret.RepairTimer > 0) || Player.Turret.RefitTimer > 0)
            TakeInventory("DRPGCredits", MaintCost);
    }

    // Maintenance is done
    if (Player.Turret.Maintenance && Player.Turret.ChargeTimer <= 0 && Player.Turret.RepairTimer <= 0 && Player.Turret.RefitTimer <= 0)
    {
        Player.Turret.Maintenance = false;
        ActivatorSound("turret/maintenancedone", 127);
    }

    Delay(1);
    goto Start;
}

NamedScript Type_ENTER void TurretCommandWheel()
{
    int const Commands[MAX_COMMANDS] =
    {
        TU_BUILD,
        TU_WEAPON_BULLET,
        TU_WEAPON_BULLET_CAPACITY,
        TU_WEAPON_PELLET,
        TU_WEAPON_PELLET_CAPACITY,
        TU_WEAPON_ROCKET,
        TU_WEAPON_ROCKET_CAPACITY,
        TU_WEAPON_PLASMA,
        TU_WEAPON_PLASMA_CAPACITY,
        TU_WEAPON_RAILGUN,
        TU_WEAPON_RAILGUN_CAPACITY,
        TU_AMMO_AUTOLOADER,
        TU_BATTERY_AUGBATTERY,
        TU_COMMAND_RECALL,
        TU_COMMAND_DRAW_FIRE,
        TU_COMMAND_HOLD_POSITION
    };

Start:
    NOP;

    bool Close = false;
    int CurrentCommands[MAX_COMMANDS];
    int CurrentCommandsCount = 0;
    int Radius = 160;
    int CurrentRadius = 0;
    fixed LerpPos = 0;

    // Rebuild currently known commands
    for (int i = 0; i < MAX_COMMANDS; i++)
        if (Player.Turret.Upgrade[Commands[i]] > 0)
            CurrentCommands[CurrentCommandsCount++] = Commands[i];

    // Open the wheel or quickly activate/deactivate the turret
    if (CheckInput(BT_SPEED, KEY_HELD, false, PlayerNumber()) && CheckInput(BT_USER2, KEY_PRESSED, false, PlayerNumber()))
        TurretCommand(TU_BUILD);
    else if (!CheckInput(BT_SPEED, KEY_HELD, false, PlayerNumber()) && CheckInput(BT_USER2, KEY_HELD, false, PlayerNumber()) && !((Player.InMenu && Player.Menu != 3) || Player.InShop || Player.OutpostMenu > 0 || Player.SkillWheelOpen) && Player.Turret.Init)
    {
        ActivatorSound("menu/click", 127);
        Player.Turret.WheelOpen = true;
        LerpPos = 1.0;
    }

    while (Player.Turret.WheelOpen || CurrentRadius > 0)
    {
        fixed Increment = 1.0 / CurrentCommandsCount;
        fixed Location = (Increment * (fixed)Player.Turret.WheelCommand);
        fixed OldLocation;
        fixed Angle;
        fixed X;
        fixed Y;

        if (Location < 0.5 && OldLocation > 0.5 && OldLocation - Location > 0.5)
            OldLocation -= 1.0;
        else if (Location > 0.5 && OldLocation < 0.5 && Location - OldLocation > 0.5)
            OldLocation += 1.0;

        // Freeze player
        SetPlayerProperty(0, 1, PROP_FROZEN);

        // Check for release
        if (!CheckInput(BT_USER2, KEY_HELD, false, PlayerNumber()) && !Close)
            Close = true;

        // Lerp position
        if (LerpPos <= 1.0)
            LerpPos += 0.1;
        if (LerpPos > 1.0)
            LerpPos = 1.0;

        // Animate wheel closed/open
        if (Close)
        {
            CurrentRadius -= 32;

            // Closed
            if (CurrentRadius <= 0)
            {
                SetPlayerProperty(0, 0, PROP_FROZEN);
                Player.Turret.WheelOpen = false;
            }
        }
        else if (CurrentRadius < Radius)
            CurrentRadius += 32;

        // Set Resolution
        SetHudSize(640, 480, false);

        // Turret Timers
        DrawTurretTimers(206.0, 32.0);

        // Command Icons/Info
        for (int i = 0; i < CurrentCommandsCount; i++)
        {
            Angle = 0.25 + Lerp(OldLocation, Location, LerpPos) - (Increment * i);
            X = 320.0 + (CurrentRadius * Cos(Angle));
            Y = 240.0 + (CurrentRadius * Sin(Angle));

            if (Player.Turret.Upgrade[CurrentCommands[i]] > 0)
                DrawTurretInfo((int)X, (int)Y, CurrentCommands[i]);

            PrintSprite(StrParam("T_UPG%d", CurrentCommands[i] + 1), 0, (int)X, (int)Y, 0.05);
        }

        // Box
        PrintSprite("ItemBoxH", 0, 320, 240 + CurrentRadius, 0.05);

        // Name
        SetFont("BIGFONT");
        HudMessage("%S", TurretUpgradeData[CurrentCommands[Player.Turret.WheelCommand]].Name);
        EndHudMessage(HUDMSG_PLAIN, 0, "White", 320, 240 + CurrentRadius + 32, 0.05);

        // Description
        SetFont("SMALLFONT");
        HudMessage("%S", TurretUpgradeData[CurrentCommands[Player.Turret.WheelCommand]].CommandInfo);
        EndHudMessage(HUDMSG_PLAIN, 0, "Yellow", 320.4, 240.1 + CurrentRadius + 48, 0.05);

        // Input
        if (!Close)
        {
            if (CheckInput(BT_USE, KEY_PRESSED, false, PlayerNumber()))
                TurretCommand(CurrentCommands[Player.Turret.WheelCommand]);
            if (CheckInput(BT_SPEED, KEY_PRESSED, false, PlayerNumber()))
                TurretMaintenance();
            if (CheckInput(BT_MOVELEFT, KEY_PRESSED, false, PlayerNumber()))
            {
                ActivatorSound("menu/click", 127);
                Player.Turret.WheelCommand--;
                OldLocation = Location;
                LerpPos = 0;
                if (Player.Turret.WheelCommand < 0) Player.Turret.WheelCommand = CurrentCommandsCount - 1;
            }
            if (CheckInput(BT_MOVERIGHT, KEY_PRESSED, false, PlayerNumber()))
            {
                ActivatorSound("menu/click", 127);
                Player.Turret.WheelCommand++;
                OldLocation = Location;
                LerpPos = 0;
                if (Player.Turret.WheelCommand > CurrentCommandsCount - 1) Player.Turret.WheelCommand = 0;
            }
        }

        Delay(1);
    }

    Delay(1);
    goto Start;
}

NamedScript DECORATE void TurretSetOffset(int AngleOffset, int DistanceOffset, int HeightOffset)
{
    Player.Turret.AngleOffset += (fixed)AngleOffset / 100.0;
    Player.Turret.DistanceOffset += (fixed)DistanceOffset;
    Player.Turret.HeightOffset += (fixed)HeightOffset;
}

NamedScript DECORATE void TurretUseAmmo(int Type)
{
    SetActivator(GetActorProperty(0, APROP_MasterTID)); // Transfer from Turret to Player

    switch(Type)
    {
    case TW_BULLET:
        Player.Turret.BulletAmmo--;
        break;
    case TW_PELLET:
        Player.Turret.ShellAmmo--;
        break;
    case TW_ROCKET:
        Player.Turret.RocketAmmo--;
        break;
    case TW_PLASMA:
        Player.Turret.PlasmaAmmo--;
        break;
    case TW_RAILGUN:
        Player.Turret.RailAmmo--;
        break;
    }
}

NamedScript DECORATE int TurretGetProjectileDamage(int Type)
{
    SetActivator(GetActorProperty(0, APROP_TargetTID)); // Transfer from missile to Turret
    SetActivator(GetActorProperty(0, APROP_MasterTID)); // Transfer from Turret to Player

    if (Type == TP_ROCKET)
        return (100 * (Player.Turret.Upgrade[TU_WEAPON_ROCKET_DAMAGE] + 1) * (1.0 + Player.Turret.Upgrade[TU_WEAPON_ROCKET_DAMAGE] / 40.0));
    else if (Type == TP_PLASMA)
        return (10 * (Player.Turret.Upgrade[TU_WEAPON_PLASMA_DAMAGE] + 1) * (1.0 + Player.Turret.Upgrade[TU_WEAPON_PLASMA_DAMAGE] / 40.0));

    return 0;
}

NamedScript DECORATE int TurretGetProjectileProperty(int Type)
{
    SetActivator(GetActorProperty(0, APROP_TargetTID)); // Transfer from missile to Turret
    SetActivator(GetActorProperty(0, APROP_MasterTID)); // Transfer from Turret to Player

    if (Type == TP_ROCKET)
        return Player.Turret.Upgrade[TU_WEAPON_ROCKET_SEEKING];

    return 0;
}

// Turret AI

NamedScript void TurretAI(int TID)
{
    SetActivator(TID);

    int Weapon = GetUserVariable(0, "user_weapon");
    int Ammo = 0;
    int PlayerTID = GetActorProperty(0, APROP_MasterTID);
    int PlayerID = TurretGetOwningPlayer();
    fixed TurretSpeed = GetUserCVarFixed(PlayerID, "drpg_turret_movespeed");
    bool NeedsTeleport = false;
    bool HasTarget = false;
    bool TargetAssignedTID = false;
    bool TargetForced = false;
    bool AnnouncedTarget = false;
    int LosingInterestTime = 0;
    int MaxInterestTime = GetUserCVar(PlayerID, "drpg_turret_pursuittime");
    int TargetTID = 0;
    int TargetOriginalTID = 0;
    int TravelTime = 0;
    int TeleportWaitTime = GetUserCVar(PlayerID, "drpg_turret_telewaittime");
    int AggressionType = GetUserCVar(PlayerID, "drpg_turret_aggression");
    int AggressionMode = 0;
    int HeatLevel = 0;
    int CurrentCommand = GetUserVariable(0, "user_command");
    bool Cooldown = false;
    fixed TurretPlayerDistance = GetUserCVarFixed(PlayerID, "drpg_turret_playerdist");
    fixed TurretEnemyDistance = GetUserCVarFixed(PlayerID, "drpg_turret_enemydist");
    fixed TurretAngle;
    fixed TurretPitch;
    fixed EnemyAngle;
    fixed EnemyPitch;
    fixed DodgeDirection;
    fixed TurretIdleRotation = 0.0;

Start:

    if (ClassifyActor(0) == ACTOR_WORLD)
        return;

    // Just in case it was changed
    TurretSpeed = GetUserCVarFixed(PlayerID, "drpg_turret_movespeed");
    TurretPlayerDistance = GetUserCVarFixed(PlayerID, "drpg_turret_playerdist");
    TurretEnemyDistance = GetUserCVarFixed(PlayerID, "drpg_turret_enemydist");
    AggressionType = GetUserCVar(PlayerID, "drpg_turret_aggression");
    MaxInterestTime = GetUserCVar(PlayerID, "drpg_turret_pursuittime");
    TeleportWaitTime = GetUserCVar(PlayerID, "drpg_turret_telewaittime");

    CurrentCommand = GetUserVariable(0, "user_command");

    Weapon = GetUserVariable(0, "user_weapon");
    switch (Weapon)
    {
    case TW_BULLET:
        Ammo = GetUserVariable(0, "user_ammo_bullet");
        break;
    case TW_PELLET:
        Ammo = GetUserVariable(0, "user_ammo_shell");
        break;
    case TW_ROCKET:
        Ammo = GetUserVariable(0, "user_ammo_rocket");
        break;
    case TW_PLASMA:
        Ammo = GetUserVariable(0, "user_ammo_plasma");
        break;
    case TW_RAILGUN:
        Ammo = GetUserVariable(0, "user_ammo_rail");
        break;
    }

    // Target handling
    HasTarget = TurretCheckTarget() && !TurretCheckTargetIsSelf();
    if (HasTarget)
    {
        TargetTID = UniqueTID();
        TargetOriginalTID = TurretGetTargetTID();
        TargetAssignedTID = TurretForceTargetTID(TargetTID);
    }
    else
    {
        TargetTID = 0;
        TargetOriginalTID = 0;
    }

    if (HasTarget)
    {
        if ((!TurretCheckTargetSuitable() && !TargetForced) || TurretCheckTargetDead())
        {
            SetPointer(AAPTR_TARGET, 0, AAPTR_NULL);
            HasTarget = false;
            TargetForced = false;
        }
        else
        {
            if (!CheckSight(0, TargetTID, CSF_NOFAKEFLOORS) && !TargetForced)
            {
                LosingInterestTime++;

                if (LosingInterestTime >= 35 * MaxInterestTime)
                {
                    SetPointer(AAPTR_TARGET, 0, AAPTR_NULL);
                    HasTarget = false;
                    LosingInterestTime = 0;
                }
            }
            else
            {
                LosingInterestTime = 0;

                if (CurrentCommand == TC_DRAW_FIRE)
                    TurretGetAttention(TargetTID, ActivatorTID());
            }
        }
    }

    // Player is ordering us to switch targets
    if (CurrentCommand == TC_DRAW_FIRE && TurretCheckPlayerTarget() && TurretWantsToSwitchToPlayerTarget())
    {
        TurretSyncWithPlayerTarget();
        HasTarget = true;
        TargetForced = true;
        SetActorState(0, "Targeting");
        AnnouncedTarget = true;
    }

    // Air friction, don't slide away helplessly like cacodemons do.
    SetActorVelocity(0, GetActorVelX(0) * 0.90625, GetActorVelY(0) * 0.90625, GetActorVelZ(0) * 0.90625, false, false);

    // Float bobbing
    SetActorVelocity(0, 0, 0, 0.125 * Sin((fixed)Timer() / 70.0), true, false);

    if (AbsFixed(GetActorVelX(0)) < 0.01 && AbsFixed(GetActorVelY(0)) < 0.01 && AbsFixed(GetActorVelZ(0)) < 0.01)
        SetActorVelocity(0, 0, 0, 0, false, false); // Dead stop if we're going too slow

    // Movement

    // Stay off the floor
    if (GetActorZ(0) - GetActorFloorZ(0) < 32)
        SetActorVelocity(0, 0, 0, TurretSpeed, true, false);

    // Teleport detection - If the player suddenly poofs into another part of the map, we need to go with them.
    if (GetActorProperty(PlayerTID, APROP_Reactiontime) > 0 && Distance(0, PlayerTID) > 96.0)
        NeedsTeleport = true;

    if (GetUserVariable(0, "user_needs_teleport"))
    {
        NeedsTeleport = true;
        SetUserVariable(0, "user_needs_teleport", 0);
    }

    if (NeedsTeleport)
        NeedsTeleport = !TurretTeleport(PlayerTID);

    // Spawn a goal flag if we need to hold our position
    if (CurrentCommand == TC_HOLD_POSITION)
    {
        if (!GetUserVariable(0, "user_goal_tid"))
        {
            SetUserVariable(0, "user_goal_tid", UniqueTID());
            SpawnForced("DRPGTurretGoalFlag", GetActorX(0), GetActorY(0), GetActorFloorZ(0), GetUserVariable(0, "user_goal_tid"), GetActorAngle(0) * 256);
        }
    }
    else
    {
        if (GetUserVariable(0, "user_goal_tid"))
        {
            Thing_Remove(GetUserVariable(0, "user_goal_tid"));
            SetUserVariable(0, "user_goal_tid", 0);
        }
    }

    /* Turret Status Debug
    SetHudSize(640, 480, 1);
    HudMessage("Turret Status\n\nHas Target: %S\nTarget Forced: %S\nTarget Original TID: %d\nTarget TID: %d\nInterest Time: %d", HasTarget? "Yes" : "No", TargetForced? "Yes" : "No", TargetOriginalTID, TargetTID, LosingInterestTime);
    EndhudMessage(HUDMSG_FADEOUT, 864, "Green", 32.1, 32.1, 1.0, 1.0);
    */

    if (!HasTarget)
    {
        // Idle routine

        if (AnnouncedTarget)
            AnnouncedTarget = false;

        if (CurrentCommand == TC_HOLD_POSITION)
        {
            if (Distance(0, GetUserVariable(0, "user_goal_tid")) > 80.0)
            {
                TurretAngle = VectorAngle(GetActorX(GetUserVariable(0, "user_goal_tid")) - GetActorX(0), GetActorY(GetUserVariable(0, "user_goal_tid")) - GetActorY(0));
                TurretPitch = VectorAngle(Distance(0, GetUserVariable(0, "user_goal_tid")), GetActorZ(GetUserVariable(0, "user_goal_tid")) - GetActorZ(0));

                TurretTurn(TurretAngle, TurretPitch);
                TurretFly(TurretAngle, TurretPitch, TurretSpeed);

                TurretIdleRotation = 0.0;
                TravelTime++;
            }
            else
            {
                TurretAngle = (GetActorAngle(GetUserVariable(0, "user_goal_tid")) + TurretIdleRotation) % 1.0;
                TurretTurn(TurretAngle, 0);

                TurretIdleRotation += 0.007;
                TurretIdleRotation %= 1.0;
            }

            if (Weapon != TW_NONE && !(Timer() % 35))
            {
                SetActorState(0, "ScanForEnemies");
            }
        }
        else
        {
            if (Distance(0, PlayerTID) > TurretPlayerDistance)
            {
                if (TravelTime > 35 * TeleportWaitTime && (AbsFixed(GetActorVelX(0)) < 0.1 || AbsFixed(GetActorVelY(0)) < 0.1))
                    // Been trying to reach the player for the specified seconds and going nowhere, so give up and teleport to them if possible
                    NeedsTeleport = true;

                TurretAngle = VectorAngle(GetActorX(PlayerTID) - GetActorX(0), GetActorY(PlayerTID) - GetActorY(0));
                TurretPitch = VectorAngle(Distance(0, PlayerTID), GetActorZ(PlayerTID) - GetActorZ(0));

                TurretTurn(TurretAngle, TurretPitch);
                TurretFly(TurretAngle, TurretPitch, TurretSpeed);

                TravelTime++;
            }
            else
            {
                if (TravelTime)
                    TravelTime = 0;

                TurretTurn(GetActorAngle(PlayerTID), -GetActorPitch(PlayerTID));
            }

            if (Weapon != TW_NONE && !(Timer() % 35))
            {
                if (TurretCheckPlayerTarget())
                {
                    TurretSyncWithPlayerTarget();
                    HasTarget = true;
                }
                else
                    SetActorState(0, "ScanForEnemies");
            }
        }
    }
    else if (TargetAssignedTID)
    {
        // Battle routine - Circle around the target and fire at it, keeping a distance away.

        if (!AnnouncedTarget)
        {
            SetActorState(0, "Targeting");
            AnnouncedTarget = true;
        }

        if (CurrentCommand == TC_DRAW_FIRE)
            AggressionMode = 1;
        else if (CurrentCommand == TC_HOLD_POSITION)
            AggressionMode = 0;
        else if (AggressionType == 2) // Determine per weapon
        {
            if (Weapon == TW_PELLET || Weapon == TW_PLASMA)
                AggressionMode = 1;
            else
                AggressionMode = 0;
        }
        else
            AggressionMode = AggressionType;

        // We can't be aggressive if we can't fight back
        if (Weapon == TW_NONE || Ammo < 1)
            AggressionMode = 0;

        EnemyAngle = VectorAngle(GetActorX(TargetTID) - GetActorX(0), GetActorY(TargetTID) - GetActorY(0));
        EnemyPitch = VectorAngle(Distance(0, TargetTID), GetActorZ(TargetTID) - GetActorZ(0));

        TurretTurn(EnemyAngle, EnemyPitch);
        if (GetCVar("drpg_turret_targeting_laser"))
            GiveInventory("DRPGTurretTargetingLaser", 1);

        if (Distance(0, TargetTID) <= GetActorPropertyFixed(TargetTID, APROP_Radius) + TurretEnemyDistance)
        {
            // Invading our personal space, back away
            TurretFly(EnemyAngle, 0, -TurretSpeed);
        }

        if (AggressionMode == 0)
        {
            // Passive Fire - Stay in slow orbit by the player and only fire at the enemy

            if (CurrentCommand == TC_HOLD_POSITION)
            {
                if (GetActorZ(TargetTID) - GetActorZ(0) < -32)
                    SetActorVelocity(0, 0, 0, -TurretSpeed, true, false);
                else if (GetActorZ(TargetTID) - GetActorZ(0) > 32)
                    SetActorVelocity(0, 0, 0, TurretSpeed, true, false);

                if (Distance(0, TargetTID) > GetActorPropertyFixed(TargetTID, APROP_Radius) + TurretEnemyDistance || LosingInterestTime)
                    TurretFly(EnemyAngle, -EnemyPitch, TurretSpeed);

                TurretFly(EnemyAngle - 0.25, 0, TurretSpeed);

                TurretAngle = VectorAngle(GetActorX(GetUserVariable(0, "user_goal_tid")) - GetActorX(0), GetActorY(GetUserVariable(0, "user_goal_tid")) - GetActorY(0));
                TurretPitch = VectorAngle(Distance(0, GetUserVariable(0, "user_goal_tid")), GetActorZ(GetUserVariable(0, "user_goal_tid")) - GetActorZ(0));

                if (Distance(0, GetUserVariable(0, "user_goal_tid")) > 80.0)
                {
                    TurretFly(TurretAngle, TurretPitch, TurretSpeed);
                    TurretTurn(TurretAngle, TurretPitch);
                }
            }
            else
            {
                TurretAngle = VectorAngle(GetActorX(PlayerTID) - GetActorX(0), GetActorY(PlayerTID) - GetActorY(0));
                TurretPitch = VectorAngle(Distance(0, PlayerTID), GetActorZ(PlayerTID) - GetActorZ(0));

                if (Distance(0, PlayerTID) > TurretPlayerDistance)
                    TurretFly(TurretAngle, TurretPitch, TurretSpeed);
                else
                    TurretFly(TurretAngle, 0, -TurretSpeed);

                TurretFly(TurretAngle - 0.25, 0, TurretSpeed / 3.0);
            }
        }
        else
        {
            // Aggressive Fire - Orbit the enemy and draw them away from the player aggressively

            // Stay level with the target so we have a better time shooting it
            if (GetActorZ(TargetTID) - GetActorZ(0) < -32)
                SetActorVelocity(0, 0, 0, -TurretSpeed, true, false);
            else if (GetActorZ(TargetTID) - GetActorZ(0) > 32)
                SetActorVelocity(0, 0, 0, TurretSpeed, true, false);

            if (Distance(0, TargetTID) > GetActorPropertyFixed(TargetTID, APROP_Radius) + TurretEnemyDistance || LosingInterestTime)
                TurretFly(EnemyAngle, -EnemyPitch, TurretSpeed);

            TurretFly(EnemyAngle - 0.25, 0, TurretSpeed);
        }

        // Unload all you've got!
        if (Ammo > 0 && !Cooldown && HeatLevel < 1000 && !GetUserVariable(0, "user_firing") && CheckSight(0, TargetTID, CSF_NOFAKEFLOORS) && (Weapon != TW_ROCKET || ((Distance(0, TargetTID) > 176.0 || GetUserArray(0, "user_upgrades", TU_ARMOR_PLATING_BLAST)) && Distance(PlayerID, TargetTID) > 176.0)))
        {
            SetActorState(0, "Missile");
            switch (Weapon)
            {
            case TW_BULLET:
                HeatLevel += 50 - (GetUserVariable(0, "user_bullet_rof") * 6);
                break;
            case TW_PELLET:
                HeatLevel += 250 - (GetUserVariable(0, "user_pellet_rof") * 30);
                break;
            case TW_ROCKET:
                HeatLevel += 300 - (GetUserVariable(0, "user_rocket_rof") * 40);
                break;
            case TW_PLASMA:
                HeatLevel += 60 - (GetUserVariable(0, "user_plasma_rof") * 8);
                break;
            case TW_RAILGUN:
                HeatLevel += 500 - (GetUserVariable(0, "user_railgun_rof") * 65);
                Log("HeatLevel = %d", HeatLevel);
                break;
            }
        }
    }

    if (HeatLevel > 0)
    {
        if (HeatLevel >= 1000 && !GetUserVariable(0, "user_firing") && !Cooldown)
        {
            SetActorState(0, "Cooldown");
            Cooldown = true;
        }

        if (Cooldown)
            SpawnForced("DRPGTurretCooldownIcon", GetActorX(0), GetActorY(0), GetActorZ(0) + 40.0, 0, 0);

        if (!(Timer() % 16))
            HeatLevel -= 80;

        // More enemies aggression to turret
        if (!Cooldown && Random(0, 100) <= 15)
            GiveInventory("DRPGFriendlyAlertMonsters", 1);
    }
    else if (Cooldown)
        Cooldown = false;

    if (TargetAssignedTID)
    {
        Thing_ChangeTID(TargetTID, TargetOriginalTID);
        TargetAssignedTID = false;
    }

    /* Aim debugging assist
    if (DebugLog)
      SpawnForced("DRPGTurretTeleportParticle",
                  GetActorX(0) + (64.0 * Cos(GetActorAngle(0)) * Cos(-GetActorPitch(0))),
                  GetActorY(0) + (64.0 * Sin(GetActorAngle(0)) * Cos(-GetActorPitch(0))),
                  GetActorZ(0) + (64.0 * Sin(-GetActorPitch(0))), 0, 0); */

    Delay(1);

    goto Start;
}

void TurretFly(fixed Angle, fixed Pitch, fixed Speed)
{
    SetActorVelocity(0, Speed * Cos(Angle) * Cos(Pitch), Speed * Sin(Angle) * Cos(Pitch), Speed * Sin(Pitch), true, false);
}

void TurretTurn(fixed NewAngle, fixed NewPitch)
{
    fixed OldAngle = GetActorAngle(0);
    fixed OldPitch = GetActorPitch(0);

    NewPitch = -(((0.5 + NewPitch) % 1.0) - 0.5);

    if (OldAngle == NewAngle && OldPitch == NewPitch)
        return;

    if (OldAngle != NewAngle)
    {
        if (OldAngle < NewAngle && NewAngle - OldAngle < 0.5)
            SetActorAngle(0, OldAngle + Clamp(0, NewAngle - OldAngle, 0.03));
        else if (OldAngle < NewAngle && NewAngle - OldAngle > 0.5)
            SetActorAngle(0, OldAngle + Clamp(-0.03, NewAngle - (OldAngle + 1.0), 0));
        else if (OldAngle > NewAngle && OldAngle - NewAngle < 0.5)
            SetActorAngle(0, OldAngle + Clamp(-0.03, NewAngle - OldAngle, 0));
        else
            SetActorAngle(0, OldAngle + Clamp(0, (NewAngle + 1.0) - OldAngle, 0.03));
    }

    if (OldPitch != NewPitch)
        SetActorPitch(0, OldPitch + Clamp(-0.03, NewPitch - OldPitch, 0.03));
}

NamedScript bool TurretCheckTarget()
{
    return SetActivator(0, AAPTR_TARGET);
}

NamedScript bool TurretCheckTargetIsSelf()
{
    return IsPointerEqual(AAPTR_TARGET, AAPTR_DEFAULT);
}

NamedScript bool TurretCheckPlayerTarget()
{
    if (!SetActivator(0, AAPTR_FRIENDPLAYER))
        return false;

    return SetActivator(0, AAPTR_PLAYER_GETTARGET);
}

NamedScript bool TurretCheckTargetSuitable()
{
    bool Suitable = true;

    if (!SetActivator(0, AAPTR_TARGET))
        return false;

    int ActorInfo = ClassifyActor(0);
    if (ActorInfo == ACTOR_WORLD || ActorInfo & ACTOR_DEAD || CheckFlag(0, "NONSHOOTABLE") || !CheckFlag(0, "SHOOTABLE") || CheckFlag(0, "FRIENDLY") || CheckFlag(0, "NEVERTARGET") || !CheckFlag(0, "ISMONSTER"))
        Suitable = false;

    return Suitable;
}

NamedScript bool TurretCheckTargetDead()
{
    bool TargetDead = false;

    if (!SetActivator(0, AAPTR_TARGET))
        return true;

    int ActorInfo = ClassifyActor(0);
    if (ActorInfo == ACTOR_WORLD || ActorInfo & ACTOR_DEAD)
        TargetDead = true;

    return TargetDead;
}

NamedScript void TurretGetAttention(int MonsterTID, int TurretTID)
{
    if (!SetActivator(MonsterTID))
        return;

    SetPointer(AAPTR_TARGET, TurretTID);
}

NamedScript bool TurretForceTargetTID(int NewTID)
{
    SetActivator(0, AAPTR_TARGET | AAPTR_NULL);

    if (ClassifyActor(0) != ACTOR_WORLD)
    {
        Thing_ChangeTID(0, NewTID);
        return true;
    }

    return false;
}

NamedScript void TurretSyncWithPlayerTarget()
{
    int EnemyOriginalTID = 0;
    int EnemyTID = 0;
    int TurretOriginalTID = ActivatorTID();
    int TurretTID = UniqueTID();
    Thing_ChangeTID(0, TurretTID);

    if (!SetActivator(0, AAPTR_FRIENDPLAYER))
    {
        Thing_ChangeTID(TurretTID, TurretOriginalTID);
        return;
    }

    if (SetActivator(0, AAPTR_PLAYER_GETTARGET | AAPTR_NULL) && ClassifyActor(0) != ACTOR_WORLD)
    {
        EnemyOriginalTID = ActivatorTID();
        EnemyTID = UniqueTID();
        Thing_ChangeTID(0, EnemyTID);

        SetActivator(TurretTID);
        SetPointer(AAPTR_TARGET, EnemyTID);
    }

    if (EnemyTID != 0)
        Thing_ChangeTID(EnemyTID, EnemyOriginalTID);

    Thing_ChangeTID(TurretTID, TurretOriginalTID);
}

NamedScript int TurretGetOwningPlayer()
{
    if (!SetActivator(0, AAPTR_FRIENDPLAYER))
        return -1;

    return PlayerNumber();
}

NamedScript int TurretGetTargetTID()
{
    if (SetActivator(0, AAPTR_TARGET | AAPTR_NULL) && ClassifyActor(0) != ACTOR_WORLD)
        return ActivatorTID();

    return 0;
}

NamedScript bool TurretWantsToSwitchToPlayerTarget()
{
    if (!SetActivator(0, AAPTR_FRIENDPLAYER))
        return false;

    if (CheckInput(BT_ATTACK, KEY_PRESSED, true, -1) && CheckInput(BT_SPEED, KEY_HELD, true, -1))
        return true;
    else
        return false;
}

void BuildTurretData()
{
    if (GetActivatorCVar("drpg_menu_input_axes"))
    {
        TurretUpgradeData[TU_BUILD].CommandInfo = StrParam("Issuing this command will enable or disable the turret\n\Ck(\Cd%S\C- + \Cd%S\Ck for quick use)", "Run", "Turret Wheel");
        TurretUpgradeData[TU_COMMAND_DRAW_FIRE].CommandInfo = StrParam("When issued, use \Cd%S\C- + \Cd%S\Ck to force a target switch", "Run", "Attack");
    }
    else
    {
        TurretUpgradeData[TU_BUILD].CommandInfo = StrParam("Issuing this command will enable or disable the turret\n\Ck(\Cd%jS\C- + \Cd%jS\Ck for quick use)", "+speed", "+user2");
        TurretUpgradeData[TU_COMMAND_DRAW_FIRE].CommandInfo = StrParam("When issued, use \Cd%jS\C- + \Cd%jS\Ck to force a target switch", "+speed", "+attack");
    }
}

bool TurretTeleport(int DestTID)
{
    fixed OldX = GetActorX(0);
    fixed OldY = GetActorY(0);
    fixed OldZ = GetActorZ(0);
    fixed TeleX[3] =
    {
        GetActorX(DestTID) + (64.0 * Cos(GetActorAngle(DestTID) + 0.25)),
        GetActorX(DestTID) + (64.0 * Cos(GetActorAngle(DestTID) - 0.25)),
        GetActorX(DestTID) + (64.0 * Cos(GetActorAngle(DestTID)))
    };
    fixed TeleY[3] =
    {
        GetActorY(DestTID) + (64.0 * Sin(GetActorAngle(DestTID) + 0.25)),
        GetActorY(DestTID) + (64.0 * Sin(GetActorAngle(DestTID) - 0.25)),
        GetActorY(DestTID) + (64.0 * Sin(GetActorAngle(DestTID)))
    };
    fixed TeleZ = GetActorZ(DestTID) + 32.0;

    bool Teleported = false;
    for (int i = 0; i < 3; i++)
    {
        // Try left, right, then in front
        Teleported = SetActorPosition(0, TeleX[i], TeleY[i], TeleZ, false);

        if (Teleported && CheckSight(DestTID, 0, CSF_NOFAKEFLOORS))
        {
            PlaySound(0, "turret/spawn", CHAN_BODY, 1.0, false, ATTN_NORM);
            SpawnForced("DRPGTurretTeleport", TeleX[i], TeleY[i], TeleZ - 32.0, 0, 0);

            SetPointer(AAPTR_TARGET, 0, AAPTR_NULL);
            break;
        }
        else
        {
            Teleported = false;
            continue;
        }
    }

    if (!Teleported)
        SetActorPosition(0, OldX, OldY, OldZ, false);

    return Teleported;
}

void TurretMaintenance()
{
    // Don't do anything if the turret isn't built yet
    if (!Player.Turret.Upgrade[TU_BUILD]) return;

    // Don't send for maintenance if there is no maintenance to be done
    if (!Player.Turret.Maintenance && Player.Turret.ChargeTimer <= 0 && Player.Turret.RepairTimer <= 0 && Player.Turret.RefitTimer <= 0) return;

    // You don't have any money to begin maintenance
    if (!CheckInventory("DRPGCredits"))
    {
        ActivatorSound("menu/error", 127);
        PrintError("Cannot perform maintenance because you have no money");
        return;
    }

    if (!Player.Turret.Maintenance)
    {
        ActivatorSound("menu/move", 127);
        Player.Turret.Maintenance = true;
        TurretDespawn();
    }
    else
    {
        ActivatorSound("menu/move", 127);
        Player.Turret.Maintenance = false;
    }
}

bool TurretLoadAmmo(int Type)
{
    str AmmoActor[5] =
    {
        "Clip",
        "Shell",
        "RocketAmmo",
        "Cell",
        "Cell"
    };
    str ReloadSound[5] =
    {
        "turret/reloadbullet",
        "turret/reloadpellet",
        "turret/reloadrocket",
        "turret/reloadplasma",
        "turret/reloadrail"
    };
    int MinAmount[5] = { 50, 20, 5, 50, 25 };
    int *Ammo[5] =
    {
        &Player.Turret.BulletAmmo,
        &Player.Turret.ShellAmmo,
        &Player.Turret.RocketAmmo,
        &Player.Turret.PlasmaAmmo,
        &Player.Turret.RailAmmo
    };
    int *AmmoMax[5] =
    {
        &Player.Turret.BulletAmmoMax,
        &Player.Turret.ShellAmmoMax,
        &Player.Turret.RocketAmmoMax,
        &Player.Turret.PlasmaAmmoMax,
        &Player.Turret.RailAmmoMax
    };
    int AmmoType;

    // Determine proper ammo type from param
    switch (Type)
    {
    case TU_WEAPON_BULLET:
        AmmoType = 0;
        break;
    case TU_WEAPON_PELLET:
        AmmoType = 1;
        break;
    case TU_WEAPON_ROCKET:
        AmmoType = 2;
        break;
    case TU_WEAPON_PLASMA:
        AmmoType = 3;
        break;
    case TU_WEAPON_RAILGUN:
        AmmoType = 4;
        break;
    }

    if (CheckInventory(AmmoActor[AmmoType]) >= MinAmount[AmmoType] && AmmoType == 4) // Railgun special case
    {
        (*Ammo[AmmoType])++;
        TakeInventory(AmmoActor[AmmoType], MinAmount[AmmoType]);
        ThingSound(Player.Turret.TID, ReloadSound[AmmoType], 127);
        return true;
    }
    else if (CheckInventory(AmmoActor[AmmoType]) >= MinAmount[AmmoType] && *Ammo[AmmoType] + MinAmount[AmmoType] > *AmmoMax[AmmoType])
    {
        int Amount = *AmmoMax[AmmoType] - *Ammo[AmmoType];
        *Ammo[AmmoType] += Amount;
        TakeInventory(AmmoActor[AmmoType], Amount);
        ThingSound(Player.Turret.TID, ReloadSound[AmmoType], 127);
        return true;
    }
    else if (CheckInventory(AmmoActor[AmmoType]) >= MinAmount[AmmoType])
    {
        *Ammo[AmmoType] += MinAmount[AmmoType];
        TakeInventory(AmmoActor[AmmoType], MinAmount[AmmoType]);
        ThingSound(Player.Turret.TID, ReloadSound[AmmoType], 127);
        return true;
    }

    return false;
}

void TurretCommand(int Index)
{
    // Don't issue the command if you don't have the base upgrade yet
    if (!Player.Turret.Upgrade[Index]) return;

    if (Index == TU_BUILD)
    {
        if (Player.Turret.Active)
            TurretDespawn();
        else
            TurretSpawn();
    }

    // Do these checks after TurretSpawn/TurretDespawn, because TurretSpawn will print its own errors for these conditions, and these conditions aren't even possible for TurretDespawn.
    if (Player.Turret.Destroyed || Player.Turret.Maintenance) return;

    if (Index == TU_WEAPON_BULLET && Player.Turret.Active)
    {
        if (Player.Turret.Weapon != TW_BULLET)
        {
            Player.Turret.Weapon = TW_BULLET;
            ThingSound(Player.Turret.TID, "turret/loadbullet", 127);
        }
        else
        {
            Player.Turret.Weapon = TW_NONE;
            ThingSound(Player.Turret.TID, "turret/cooldown", 127);
        }
    }

    if (Index == TU_WEAPON_BULLET_CAPACITY && Player.Turret.BulletAmmo < Player.Turret.BulletAmmoMax && Player.Turret.Active)
    {
        if (!TurretLoadAmmo(TU_WEAPON_BULLET))
        {
            ActivatorSound("menu/error", 127);
            PrintError("You need at least \Ca50 Bullets\C- to load into the turret");
        }
    }

    if (Index == TU_WEAPON_PELLET && Player.Turret.Active)
    {
        if (Player.Turret.Weapon != TW_PELLET)
        {
            Player.Turret.Weapon = TW_PELLET;
            ThingSound(Player.Turret.TID, "turret/loadpellet", 127);
        }
        else
        {
            Player.Turret.Weapon = TW_NONE;
            ThingSound(Player.Turret.TID, "turret/cooldown", 127);
        }
    }

    if (Index == TU_WEAPON_PELLET_CAPACITY && Player.Turret.ShellAmmo < Player.Turret.ShellAmmoMax && Player.Turret.Active)
    {
        if (!TurretLoadAmmo(TU_WEAPON_PELLET))
        {
            ActivatorSound("menu/error", 127);
            PrintError("You need at least \Ci20 Shells\C- to load into the turret");
        }
    }

    if (Index == TU_WEAPON_ROCKET && Player.Turret.Active)
    {
        if (Player.Turret.Weapon != TW_ROCKET)
        {
            Player.Turret.Weapon = TW_ROCKET;
            ThingSound(Player.Turret.TID, "turret/loadrocket", 127);
        }
        else
        {
            Player.Turret.Weapon = TW_NONE;
            ThingSound(Player.Turret.TID, "turret/cooldown", 127);
        }
    }

    if (Index == TU_WEAPON_ROCKET_CAPACITY && Player.Turret.RocketAmmo < Player.Turret.RocketAmmoMax && Player.Turret.Active)
    {
        if (!TurretLoadAmmo(TU_WEAPON_ROCKET))
        {
            ActivatorSound("menu/error", 127);
            PrintError("You need at least \Cc5 Rockets\C- to load into the turret");
        }
    }

    if (Index == TU_WEAPON_PLASMA && Player.Turret.Active)
    {
        if (Player.Turret.Weapon != TW_PLASMA)
        {
            Player.Turret.Weapon = TW_PLASMA;
            ThingSound(Player.Turret.TID, "turret/loadplasma", 127);
        }
        else
        {
            Player.Turret.Weapon = TW_NONE;
            ThingSound(Player.Turret.TID, "turret/cooldown", 127);
        }
    }

    if (Index == TU_WEAPON_PLASMA_CAPACITY && Player.Turret.PlasmaAmmo < Player.Turret.PlasmaAmmoMax && Player.Turret.Active)
    {
        if (!TurretLoadAmmo(TU_WEAPON_PLASMA))
        {
            ActivatorSound("menu/error", 127);
            PrintError("You need at least \Cd50 Cells\C- to load into the turret");
        }
    }

    if (Index == TU_WEAPON_RAILGUN && Player.Turret.Active)
    {
        if (Player.Turret.Weapon != TW_RAILGUN)
        {
            Player.Turret.Weapon = TW_RAILGUN;
            ThingSound(Player.Turret.TID, "turret/loadrail", 127);
        }
        else
        {
            Player.Turret.Weapon = TW_NONE;
            ThingSound(Player.Turret.TID, "turret/cooldown", 127);
        }
    }

    if (Index == TU_WEAPON_RAILGUN_CAPACITY && Player.Turret.RailAmmo < Player.Turret.RailAmmoMax && Player.Turret.Active)
    {
        if (!TurretLoadAmmo(TU_WEAPON_RAILGUN))
        {
            ActivatorSound("menu/error", 127);
            PrintError("You need at least \Cd25 Cells\C- to load into the turret");
        }
    }

    if (Index == TU_AMMO_AUTOLOADER)
    {
        ActivatorSound("menu/move", 127);
        Player.Turret.Autoload = !Player.Turret.Autoload;
    }

    if (Index == TU_BATTERY_AUGBATTERY)
    {
        if (Player.Augs.Battery <= 0)
        {
            PrintError("Your aug battery is depleted");
            ActivatorSound("menu/error", 127);
            Player.Turret.AugBattery = false;
        }
        else
        {
            ActivatorSound("menu/move", 127);
            Player.Turret.AugBattery = !Player.Turret.AugBattery;
        }
    }

    if (Index == TU_COMMAND_RECALL && Player.Turret.Active)
    {
        ActivatorSound("menu/move", 127);
        Player.Turret.Command = TC_NONE;
        SetUserVariable(Player.Turret.TID, "user_needs_teleport", 1);
    }

    if (Index == TU_COMMAND_DRAW_FIRE && Player.Turret.Active)
    {
        ActivatorSound("menu/move", 127);
        if (Player.Turret.Command != TC_DRAW_FIRE)
            Player.Turret.Command = TC_DRAW_FIRE;
        else
            Player.Turret.Command = TC_NONE;
    }

    if (Index == TU_COMMAND_HOLD_POSITION && Player.Turret.Active)
    {
        ActivatorSound("menu/move", 127);
        if (Player.Turret.Command != TC_HOLD_POSITION)
            Player.Turret.Command = TC_HOLD_POSITION;
        else
            Player.Turret.Command = TC_NONE;
    }
}

int TurretUpgradeCost(int Index)
{
    int Cost = TurretUpgradeData[Index].Cost * (Player.Turret.Upgrade[Index] + 1);

    // Technician gets a 25% cost reduction bonus
    if (CompatMode == COMPAT_DRLA && PlayerClass(PlayerNumber()) == 2)
        return (int)(Cost * 0.75);

    return Cost;
}

void TurretPassVars()
{
    // Master TID
    SetUserVariable(Player.Turret.TID, "user_master", Player.TID);

    // Upgrade info
    for (int i = 0; i < MAX_UPGRADES; i++)
        SetUserArray(Player.Turret.TID, "user_upgrade", i, Player.Turret.Upgrade[i]);

    // Weapon Type
    SetUserVariable(Player.Turret.TID, "user_weapon", Player.Turret.Weapon);

    // Bullet
    SetUserVariable(Player.Turret.TID, "user_bullet_damage", Player.Turret.Upgrade[TU_WEAPON_BULLET_DAMAGE]);
    SetUserVariable(Player.Turret.TID, "user_bullet_rof", Player.Turret.Upgrade[TU_WEAPON_BULLET_ROF]);

    // Pellet
    SetUserVariable(Player.Turret.TID, "user_pellet_damage", Player.Turret.Upgrade[TU_WEAPON_PELLET_DAMAGE]);
    SetUserVariable(Player.Turret.TID, "user_pellet_rof", Player.Turret.Upgrade[TU_WEAPON_PELLET_ROF]);
    SetUserVariable(Player.Turret.TID, "user_pellet_spread", Player.Turret.Upgrade[TU_WEAPON_PELLET_SPREAD]);
    SetUserVariable(Player.Turret.TID, "user_pellet_amount", Player.Turret.Upgrade[TU_WEAPON_PELLET_AMOUNT]);

    // Rocket
    SetUserVariable(Player.Turret.TID, "user_rocket_rof", Player.Turret.Upgrade[TU_WEAPON_ROCKET_ROF]);

    // Plasma
    SetUserVariable(Player.Turret.TID, "user_plasma_rof", Player.Turret.Upgrade[TU_WEAPON_PLASMA_ROF]);

    // Railgun
    SetUserVariable(Player.Turret.TID, "user_railgun_damage", Player.Turret.Upgrade[TU_WEAPON_RAILGUN_DAMAGE]);
    SetUserVariable(Player.Turret.TID, "user_railgun_rof", Player.Turret.Upgrade[TU_WEAPON_RAILGUN_ROF]);
    SetUserVariable(Player.Turret.TID, "user_railgun_ripping", Player.Turret.Upgrade[TU_WEAPON_RAILGUN_RIPPING] * 2);

    // Ammo
    SetUserVariable(Player.Turret.TID, "user_ammo_bullet", Player.Turret.BulletAmmo);
    SetUserVariable(Player.Turret.TID, "user_ammo_shell", Player.Turret.ShellAmmo);
    SetUserVariable(Player.Turret.TID, "user_ammo_rocket", Player.Turret.RocketAmmo);
    SetUserVariable(Player.Turret.TID, "user_ammo_plasma", Player.Turret.PlasmaAmmo);
    SetUserVariable(Player.Turret.TID, "user_ammo_rail", Player.Turret.RailAmmo);

    // Misc
    SetUserVariable(Player.Turret.TID, "user_command", Player.Turret.Command);
}

// Spawning/Despawning

void TurretSpawn()
{
    if (GetActorProperty(0, APROP_Health) <= 0) return;

    if ((Player.Turret.Battery <= 0 && !Player.Turret.AugBattery) || (Player.Turret.Battery <= 0 && Player.Turret.AugBattery && Player.Augs.Battery <= 0))
    {
        ActivatorSound("menu/error", 127);
        PrintError("Your turret's battery is depleted");
        return;
    }

    if (Player.Turret.Destroyed)
    {
        ActivatorSound("menu/error", 127);
        PrintError("Your turret is destroyed and must be repaired");
        return;
    }

    if (Player.Turret.Maintenance)
    {
        ActivatorSound("menu/error", 127);
        PrintError("Your turret is currently in the Outpost for maintenance");
        return;
    }

    if (Player.Turret.RefitTimer > 0)
    {
        ActivatorSound("menu/error", 127);
        PrintError("Your turret is currently being refitted with upgrades");
        return;
    }

    int TID = UniqueTID();
    fixed X = GetActorX(0) + (80.0 * Cos(GetActorAngle(0)));
    fixed Y = GetActorY(0) + (80.0 * Sin(GetActorAngle(0)));
    fixed Z = GetActorZ(0) + 32.0;

    bool Spawned = Spawn("DRPGPortableTurret", X, Y, Z, TID, GetActorAngle(0) * 256);
    bool CanSee = CheckSight(Player.TID, TID, 0);

    if (Spawned && CanSee)
    {
        PlaySound(TID, "turret/spawn", CHAN_BODY, 1.0, false, ATTN_NORM);
        Player.Turret.TID = TID;
        SetActorProperty(TID, APROP_MasterTID, Player.TID);
        Thing_SetTranslation(TID, -1);
        SpawnForced("DRPGTurretTeleport", X, Y, Z - 32.0, 0, 0);

        if (Player.Turret.Health > 0)
            SetActorProperty(TID, APROP_Health, Player.Turret.Health);

        TurretAI(TID);

        Player.Turret.Active = true;
    }
    else
    {
        Thing_Remove(TID);
        ActivatorSound("menu/error", 127);
        PrintError("The turret has no room to spawn here");
    }
}

void TurretDespawn()
{
    if (!Player.Turret.Active) return;

    Player.Turret.Active = false;
    Thing_Remove(GetUserVariable(Player.Turret.TID, "user_goal_tid"));

    PlaySound(Player.Turret.TID, "turret/despawn", CHAN_BODY, 1.0, false, ATTN_NORM);
    SpawnForced("DRPGTurretTeleport", GetActorX(Player.Turret.TID), GetActorY(Player.Turret.TID), GetActorZ(Player.Turret.TID) - 32.0, 0, 0);
    Thing_Remove(Player.Turret.TID);
    Player.Turret.TID = 0;
}

// Debugging

NamedScript Console void GiveTurret()
{
    Player.Turret.Upgrade[TU_BUILD] = true;
}

NamedScript Console void FullTurret()
{
    Player.Turret.Upgrade[TU_BUILD] = true;

    Delay(1);

    for (int i = 1; i < TU_MAX; i++)
        Player.Turret.Upgrade[i] = TurretUpgradeData[i].MaxLevel;

    Delay(1);

    Player.Turret.Health = Player.Turret.HealthMax;
    TurretSpawn();
}
