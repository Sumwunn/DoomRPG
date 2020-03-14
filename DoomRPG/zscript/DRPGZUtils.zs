class DRPGZUtilities
{
    static int GetLevelSectorCount()
    {
        return level.Sectors.Size();
    }
    static bool CheckActorInMap(int TID)
    {
        ActorIterator AI = ActorIterator.Create(TID);
        Actor A = AI.Next();
        if (A)
            return level.IsPointInMap(A.Pos);
        else
            return false;
    }
    static play void ForceRespawn(int PlayerNum)
    {
        let player = Players[PlayerNum].mo.player;
        if (player)
        {
            player.cls = NULL;
            player.playerstate = PST_REBORN;
            if (player.mo.special1 > 2)
            {
                player.mo.special1 = 0;
            }
        }
    }
    static bool CheckForBadMap()
    {
        if (level.MapName == "E1M8")
        {
            // Phobos Anomaly
            // Tags: 666 (Floor_LowerToLowest)
            return true;
        }

        else if (level.MapName == "E2M8")
        {
            // Tower of Babel
            // Tags: None, level ends
            return true;
        }
        else if (level.MapName == "E3M8")
        {
            // Dis
            // Tags: None, level ends
            return true;
        }
        else if (level.MapName == "E4M6")
        {
            // Against Thee Wickedly
            // Tags: 666 (Door_Open)
            return true;
        }
        else if (level.MapName == "E4M8")
        {
            // Unto the Cruel
            // Tags: 666 (Floor_LowerToLowest)
            return true;
        }
        else if (level.MapName == "MAP07")
        {
            // Dead Simple
            // Tags: 666 (Floor_LowerToLowest), 667 (Floor_RaiseByTexture)
            return true;
        }
        else if (level.MapName == "LEVEL07")
        {
            // Baron's Banquet
            // Tags: 666 (Floor_LowerToLowest), 667 (Floor_RaiseByTexture)
            return true;
        }

        return false;
    }
    static void SetHUDPreset(int Preset)
    {
        // Preset options:
        // 1 = Default
        // 2 = Wide

        CVar i;

        if (Preset == 1)
        {
            i = CVar.FindCVar('drpg_hud_height');
            i.ResetToDefault();
            i = CVar.FindCVar('drpg_hud_width');
            i.ResetToDefault();
            i = CVar.FindCVar('drpg_stats_y');
            i.ResetToDefault();
            i = CVar.FindCVar('drpg_stats_x');
            i.ResetToDefault();
            i = CVar.FindCVar('drpg_skill_y');
            i.ResetToDefault();
            i = CVar.FindCVar('drpg_skill_x');
            i.ResetToDefault();
            i = CVar.FindCVar('drpg_stateffect_y');
            i.ResetToDefault();
            i = CVar.FindCVar('drpg_stateffect_x');
            i.ResetToDefault();
            i = CVar.FindCVar('drpg_credits_y');
            i.ResetToDefault();
            i = CVar.FindCVar('drpg_credits_x');
            i.ResetToDefault();
            i = CVar.FindCVar('drpg_combo_y');
            i.ResetToDefault();
            i = CVar.FindCVar('drpg_combo_x');
            i.ResetToDefault();
            i = CVar.FindCVar('drpg_stim_y');
            i.ResetToDefault();
            i = CVar.FindCVar('drpg_stim_x');
            i.ResetToDefault();
            i = CVar.FindCVar('drpg_aug_y');
            i.ResetToDefault();
            i = CVar.FindCVar('drpg_aug_x');
            i.ResetToDefault();
            i = CVar.FindCVar('drpg_mission_x');
            i.ResetToDefault();
            i = CVar.FindCVar('drpg_mission_y');
            i.ResetToDefault();
            i = CVar.FindCVar('drpg_auratimer_x');
            i.ResetToDefault();
            i = CVar.FindCVar('drpg_auratimer_y');
            i.ResetToDefault();
            i = CVar.FindCVar('drpg_powerup_y');
            i.ResetToDefault();
            i = CVar.FindCVar('drpg_powerup_x');
            i.ResetToDefault();
            i = CVar.FindCVar('drpg_event_y');
            i.ResetToDefault();
            i = CVar.FindCVar('drpg_event_x');
            i.ResetToDefault();
            i = CVar.FindCVar('drpg_drla_y');
            i.ResetToDefault();
            i = CVar.FindCVar('drpg_drla_x');
            i.ResetToDefault();
            i = CVar.FindCVar('drpg_ld_y');
            i.ResetToDefault();
            i = CVar.FindCVar('drpg_ld_x');
            i.ResetToDefault();
            i = CVar.FindCVar('drpg_coopview_y');
            i.ResetToDefault();
            i = CVar.FindCVar('drpg_coopview_x');
            i.ResetToDefault();
            i = CVar.FindCVar('drpg_multiplayer_y');
            i.ResetToDefault();
            i = CVar.FindCVar('drpg_multiplayer_x');
            i.ResetToDefault();
            i = CVar.FindCVar('drpg_turret_y');
            i.ResetToDefault();
            i = CVar.FindCVar('drpg_turret_x');
            i.ResetToDefault();
        }
        else if (Preset == 2)
        {
            i = CVar.FindCVar('drpg_hud_height');
            i.SetInt(480);
            i = CVar.FindCVar('drpg_hud_width');
            i.SetInt(680);
            i = CVar.FindCVar('drpg_stats_y');
            i.SetInt(295);
            i = CVar.FindCVar('drpg_stats_x');
            i.SetInt(645);
            i = CVar.FindCVar('drpg_skill_y');
            i.SetInt(460);
            i = CVar.FindCVar('drpg_skill_x');
            i.SetInt(490);
            i = CVar.FindCVar('drpg_stateffect_y');
            i.SetInt(100);
            i = CVar.FindCVar('drpg_stateffect_x');
            i.SetInt(-100);
            i = CVar.FindCVar('drpg_credits_y');
            i.SetInt(15);
            i = CVar.FindCVar('drpg_credits_x');
            i.SetInt(-70);
            i = CVar.FindCVar('drpg_combo_y');
            i.SetInt(300);
            i = CVar.FindCVar('drpg_combo_x');
            i.SetInt(-100);
            i = CVar.FindCVar('drpg_stim_y');
            i.SetInt(100);
            i = CVar.FindCVar('drpg_stim_x');
            i.SetInt(640);
            i = CVar.FindCVar('drpg_aug_y');
            i.SetInt(415);
            i = CVar.FindCVar('drpg_aug_x');
            i.SetInt(700);
            i = CVar.FindCVar('drpg_mission_x');
            i.SetInt(320);
            i = CVar.FindCVar('drpg_mission_y');
            i.SetInt(420);
            i = CVar.FindCVar('drpg_auratimer_x');
            i.SetInt(-50);
            i = CVar.FindCVar('drpg_auratimer_y');
            i.SetInt(415);
            i = CVar.FindCVar('drpg_powerup_y');
            i.SetInt(30);
            i = CVar.FindCVar('drpg_powerup_x');
            i.SetInt(660);
            i = CVar.FindCVar('drpg_event_y');
            i.SetInt(15);
            i = CVar.FindCVar('drpg_event_x');
            i.SetInt(235);
            i = CVar.FindCVar('drpg_drla_y');
            i.SetInt(30);
            i = CVar.FindCVar('drpg_drla_x');
            i.SetInt(500);
            i = CVar.FindCVar('drpg_ld_y');
            i.SetInt(20);
            i = CVar.FindCVar('drpg_ld_x');
            i.SetInt(335);
            i = CVar.FindCVar('drpg_coopview_y');
            i.SetInt(0);
            i = CVar.FindCVar('drpg_coopview_x');
            i.SetInt(0);
            i = CVar.FindCVar('drpg_multiplayer_y');
            i.SetInt(8);
            i = CVar.FindCVar('drpg_multiplayer_x');
            i.SetInt(80);
            i = CVar.FindCVar('drpg_turret_y');
            i.SetInt(285);
            i = CVar.FindCVar('drpg_turret_x');
            i.SetInt(-75);
        }
    }
    static void ResetToDefaults()
    {
        CVar i;

        // XP System
        i = CVar.FindCVar('drpg_scalexp');
        i.ResetToDefault();
        i = CVar.FindCVar('drpg_scalecomboxp');
        i.ResetToDefault();
        i = CVar.FindCVar('drpg_xp_curve');
        i.ResetToDefault();
        i = CVar.FindCVar('drpg_xp_health_awareness');
        i.ResetToDefault();
        i = CVar.FindCVar('drpg_xp_summon_percent');
        i.ResetToDefault();

        // Monster Difficulty
        i = CVar.FindCVar('drpg_monster_level_cap');
        i.ResetToDefault();
        i = CVar.FindCVar('drpg_monster_stat_cap');
        i.ResetToDefault();
        i = CVar.FindCVar('drpg_monster_levels');
        i.ResetToDefault();
        i = CVar.FindCVar('drpg_monster_level_weight');
        i.ResetToDefault();
        i = CVar.FindCVar('drpg_monster_map_weight');
        i.ResetToDefault();
        i = CVar.FindCVar('drpg_monster_random_min_mult');
        i.ResetToDefault();
        i = CVar.FindCVar('drpg_monster_random_max_mult');
        i.ResetToDefault();
        i = CVar.FindCVar('drpg_aura_curve');
        i.ResetToDefault();
        i = CVar.FindCVar('drpg_monster_specialize');
        i.ResetToDefault();
        i = CVar.FindCVar('drpg_monster_shadows');
        i.ResetToDefault();
        i = CVar.FindCVar('drpg_monster_vitality_effect');
        i.ResetToDefault();
        i = CVar.FindCVar('drpg_monster_vitality_boost');
        i.ResetToDefault();
        i = CVar.FindCVar('drpg_monster_red_missileevenmore');
        i.ResetToDefault();
        i = CVar.FindCVar('drpg_monster_red_missilemore');
        i.ResetToDefault();
        i = CVar.FindCVar('drpg_monster_red_nofear');
        i.ResetToDefault();
        i = CVar.FindCVar('drpg_monster_red_retaliation');
        i.ResetToDefault();
        i = CVar.FindCVar('drpg_monster_green_dontblast');
        i.ResetToDefault();
        i = CVar.FindCVar('drpg_monster_green_dontrip');
        i.ResetToDefault();
        i = CVar.FindCVar('drpg_monster_green_nopain');
        i.ResetToDefault();
        i = CVar.FindCVar('drpg_monster_green_noradiusdmg');
        i.ResetToDefault();
        i = CVar.FindCVar('drpg_monster_green_reflective');
        i.ResetToDefault();
        i = CVar.FindCVar('drpg_monster_green_shieldreflect');
        i.ResetToDefault();
        i = CVar.FindCVar('drpg_monster_white_bright');
        i.ResetToDefault();
        i = CVar.FindCVar('drpg_monster_white_lookallaround');
        i.ResetToDefault();
        i = CVar.FindCVar('drpg_monster_white_masslevel');
        i.ResetToDefault();
        i = CVar.FindCVar('drpg_monster_white_noinfighting');
        i.ResetToDefault();
        i = CVar.FindCVar('drpg_monster_white_notarget');
        i.ResetToDefault();
        i = CVar.FindCVar('drpg_monster_white_notargetswitch');
        i.ResetToDefault();
        i = CVar.FindCVar('drpg_monster_pink_dontdrain');
        i.ResetToDefault();
        i = CVar.FindCVar('drpg_monster_pink_resurrect');
        i.ResetToDefault();
        i = CVar.FindCVar('drpg_monster_blue_epdrain');
        i.ResetToDefault();
        i = CVar.FindCVar('drpg_monster_blue_notimefreeze');
        i.ResetToDefault();
        i = CVar.FindCVar('drpg_monster_blue_seeinvisible');
        i.ResetToDefault();
        i = CVar.FindCVar('drpg_monster_purple_massheal');
        i.ResetToDefault();
        i = CVar.FindCVar('drpg_monster_orange_alwaysfast');
        i.ResetToDefault();
        i = CVar.FindCVar('drpg_monster_orange_jumpdown');
        i.ResetToDefault();
        i = CVar.FindCVar('drpg_monster_orange_notelestomp');
        i.ResetToDefault();
        i = CVar.FindCVar('drpg_monster_orange_quicktoretaliate');
        i.ResetToDefault();
        i = CVar.FindCVar('drpg_monster_orange_teleport');
        i.ResetToDefault();
        i = CVar.FindCVar('drpg_monster_darkblue_ammodrain');
        i.ResetToDefault();
        i = CVar.FindCVar('drpg_monster_yellow_moneydrain');
        i.ResetToDefault();

        // Game Difficulty
        i = CVar.FindCVar('drpg_skill_costscale');
        i.ResetToDefault();
        i = CVar.FindCVar('drpg_skill_keepauras');
        i.ResetToDefault();
        i = CVar.FindCVar('drpg_skill_costcooldown');
        i.ResetToDefault();
        i = CVar.FindCVar('drpg_shield_reset');
        i.ResetToDefault();
        i = CVar.FindCVar('drpg_levelup_heal');
        i.ResetToDefault();
        i = CVar.FindCVar('drpg_toxicity_penalties');
        i.ResetToDefault();
        i = CVar.FindCVar('drpg_mission_monsterkillscount');
        i.ResetToDefault();
        i = CVar.FindCVar('drpg_invulnerability_plus');
        i.ResetToDefault();
        i = CVar.FindCVar('drpg_combo_damage');
        i.ResetToDefault();
        i = CVar.FindCVar('drpg_monster_mapweight');
        i.ResetToDefault();
        i = CVar.FindCVar('drpg_monster_population');
        i.ResetToDefault();
        i = CVar.FindCVar('drpg_monster_limit');
        i.ResetToDefault();
        i = CVar.FindCVar('drpg_monster_adaptive_spawns');
        i.ResetToDefault();
        i = CVar.FindCVar('drpg_aurasteal_rate');
        i.ResetToDefault();
        i = CVar.FindCVar('drpg_aurasteal_amount');
        i.ResetToDefault();
        i = CVar.FindCVar('drpg_aura_removeondeath');
        i.ResetToDefault();
        i = CVar.FindCVar('drpg_minibosses');
        i.ResetToDefault();
        i = CVar.FindCVar('drpg_reinforcements');
        i.ResetToDefault();

        // Loot Generation
        i = CVar.FindCVar('drpg_loot_system');
        i.ResetToDefault();
        i = CVar.FindCVar('drpg_lootgen_factor');
        i.ResetToDefault();
        i = CVar.FindCVar('drpg_loot_rcm');
        i.ResetToDefault();
        i = CVar.FindCVar('drpg_loot_crate_spc');
        i.ResetToDefault();
        i = CVar.FindCVar('drpg_uac_premium');
        i.ResetToDefault();

        // Map Events
        i = CVar.FindCVar('drpg_mapevent_chance');
        i.ResetToDefault();
        i = CVar.FindCVar('drpg_mapevent_eventtime');
        i.ResetToDefault();
        i = CVar.FindCVar('drpg_default_par_seconds');
        i.ResetToDefault();
        i = CVar.FindCVar('drpg_mapevent_megaboss');
        i.ResetToDefault();
        i = CVar.FindCVar('drpg_mapevent_toxichazard');
        i.ResetToDefault();
        i = CVar.FindCVar('drpg_mapevent_nuclearbomb');
        i.ResetToDefault();
        i = CVar.FindCVar('drpg_mapevent_lowpower');
        i.ResetToDefault();
        i = CVar.FindCVar('drpg_mapevent_allauras');
        i.ResetToDefault();
        i = CVar.FindCVar('drpg_mapevent_onemonster');
        i.ResetToDefault();
        i = CVar.FindCVar('drpg_mapevent_hellunleashed');
        i.ResetToDefault();
        i = CVar.FindCVar('drpg_mapevent_harmonizedauras');
        i.ResetToDefault();
        i = CVar.FindCVar('drpg_mapevent_teleportcracks');
        i.ResetToDefault();
        i = CVar.FindCVar('drpg_mapevent_doomsday');
        i.ResetToDefault();
        i = CVar.FindCVar('drpg_mapevent_acidrain');
        i.ResetToDefault();
        i = CVar.FindCVar('drpg_mapevent_darkzone');
        i.ResetToDefault();
        i = CVar.FindCVar('drpg_mapevent_feedingfrenzy');
        i.ResetToDefault();
        i = CVar.FindCVar('drpg_mapevent_overmind');
        i.ResetToDefault();
        i = CVar.FindCVar('drpg_mapevent_rainbows');
        i.ResetToDefault();
        i = CVar.FindCVar('drpg_mapevent_skill_hell');
        i.ResetToDefault();
        i = CVar.FindCVar('drpg_mapevent_skill_armageddon');
        i.ResetToDefault();
        i = CVar.FindCVar('drpg_mapevent_sinstorm');
        i.ResetToDefault();

        // Stat System
        i = CVar.FindCVar('drpg_module_spawnfactor');
        i.ResetToDefault();
        i = CVar.FindCVar('drpg_module_levelfactor');
        i.ResetToDefault();
        i = CVar.FindCVar('drpg_module_statfactor');
        i.ResetToDefault();
        i = CVar.FindCVar('drpg_module_skillfactor');
        i.ResetToDefault();
        i = CVar.FindCVar('drpg_hard_stat_cap');
        i.ResetToDefault();
        i = CVar.FindCVar('drpg_soft_stat_cap_mult');
        i.ResetToDefault();
        i = CVar.FindCVar('drpg_auto_spend');
        i.ResetToDefault();
        i = CVar.FindCVar('drpg_auto_spend_pref');
        i.ResetToDefault();
        i = CVar.FindCVar('drpg_stat_weaponspeed');
        i.ResetToDefault();
        i = CVar.FindCVar('drpg_maxspeed');
        i.ResetToDefault();
        i = CVar.FindCVar('drpg_maxjump');
        i.ResetToDefault();
        i = CVar.FindCVar('drpg_inv_capacity');
        i.ResetToDefault();
        i = CVar.FindCVar('drpg_overdrive_limit');
        i.ResetToDefault();

        // Natural Stat Leveling
        i = CVar.FindCVar('drpg_levelup_natural');
        i.ResetToDefault();
        i = CVar.FindCVar('drpg_allow_spec');
        i.ResetToDefault();
        i = CVar.FindCVar('drpg_character_spec');
        i.ResetToDefault();
        i = CVar.FindCVar('drpg_strength_scalexp');
        i.ResetToDefault();
        i = CVar.FindCVar('drpg_defense_scalexp');
        i.ResetToDefault();
        i = CVar.FindCVar('drpg_vitality_scalexp');
        i.ResetToDefault();
        i = CVar.FindCVar('drpg_energy_scalexp');
        i.ResetToDefault();
        i = CVar.FindCVar('drpg_regeneration_scalexp');
        i.ResetToDefault();
        i = CVar.FindCVar('drpg_agility_scalexp');
        i.ResetToDefault();
        i = CVar.FindCVar('drpg_capacity_scalexp');
        i.ResetToDefault();
        i = CVar.FindCVar('drpg_luck_scalexp');
        i.ResetToDefault();

        // Auto Healing
        i = CVar.FindCVar('drpg_auto_heal');
        i.ResetToDefault();
        i = CVar.FindCVar('drpg_auto_heal_percent');
        i.ResetToDefault();
        i = CVar.FindCVar('drpg_auto_heal_order');
        i.ResetToDefault();

        // Damage, Module Numbers & Health Bars
        i = CVar.FindCVar('drpg_popoffs');
        i.ResetToDefault();
        i = CVar.FindCVar('drpg_damagenumbers');
        i.ResetToDefault();
        i = CVar.FindCVar('drpg_damagenumbers_fade');
        i.ResetToDefault();
        i = CVar.FindCVar('drpg_modulenumbers');
        i.ResetToDefault();
        i = CVar.FindCVar('drpg_damagenumbers_hud');
        i.ResetToDefault();
        i = CVar.FindCVar('drpg_healthbars');
        i.ResetToDefault();
        i = CVar.FindCVar('drpg_healthbars_plain');
        i.ResetToDefault();
        i = CVar.FindCVar('drpg_healthbars_names');
        i.ResetToDefault();
        i = CVar.FindCVar('drpg_healthbars_width');
        i.ResetToDefault();
        i = CVar.FindCVar('drpg_healthbars_height');
        i.ResetToDefault();
        i = CVar.FindCVar('drpg_healthbars_x');
        i.ResetToDefault();
        i = CVar.FindCVar('drpg_healthbars_y');
        i.ResetToDefault();

        // Stims
        i = CVar.FindCVar('drpg_stim_time_multiplier');
        i.ResetToDefault();

        // Multiplayer
        i = CVar.FindCVar('drpg_multi_takexp');
        i.ResetToDefault();
        i = CVar.FindCVar('drpg_multi_takexp_percent');
        i.ResetToDefault();
        i = CVar.FindCVar('drpg_multi_dropcredits');
        i.ResetToDefault();
        i = CVar.FindCVar('drpg_multi_dropcredits_percent');
        i.ResetToDefault();
        i = CVar.FindCVar('drpg_multi_dropinv');
        i.ResetToDefault();
        i = CVar.FindCVar('drpg_multi_restoreep');
        i.ResetToDefault();
        i = CVar.FindCVar('drpg_multi_restoreammo');
        i.ResetToDefault();
        i = CVar.FindCVar('drpg_multi_sharecredits');
        i.ResetToDefault();
        i = CVar.FindCVar('drpg_multi_revives');
        i.ResetToDefault();

// Shop
        i = CVar.FindCVar('drpg_shoptype');
        i.ResetToDefault();
        i = CVar.FindCVar('drpg_shopspecial_type');
        i.ResetToDefault();
        i = CVar.FindCVar('drpg_shopspecial_category');
        i.ResetToDefault();
        i = CVar.FindCVar('drpg_shopspecial_time');
        i.ResetToDefault();
        i = CVar.FindCVar('drpg_shopspecial_min');
        i.ResetToDefault();
        i = CVar.FindCVar('drpg_shopspecial_max');
        i.ResetToDefault();
        i = CVar.FindCVar('drpg_shopspecial_discount');
        i.ResetToDefault();
        i = CVar.FindCVar('drpg_pickup_behavior');
        i.ResetToDefault();
        i = CVar.FindCVar('drpg_autosell_lockerfallback');
        i.ResetToDefault();

// Turret
        i = CVar.FindCVar('drpg_turret_aggression');
        i.ResetToDefault();
        i = CVar.FindCVar('drpg_turret_movespeed');
        i.ResetToDefault();
        i = CVar.FindCVar('drpg_turret_playerdist');
        i.ResetToDefault();
        i = CVar.FindCVar('drpg_turret_enemydist');
        i.ResetToDefault();
        i = CVar.FindCVar('drpg_turret_pursuittime');
        i.ResetToDefault();
        i = CVar.FindCVar('drpg_turret_telewaittime');
        i.ResetToDefault();

// Menus
        i = CVar.FindCVar('drpg_menudim');
        i.ResetToDefault();
        i = CVar.FindCVar('drpg_menuhelp');
        i.ResetToDefault();
        i = CVar.FindCVar('drpg_menu_hideskills');
        i.ResetToDefault();
        i = CVar.FindCVar('drpg_menu_repeat');
        i.ResetToDefault();
        i = CVar.FindCVar('drpg_menu_width');
        i.ResetToDefault();
        i = CVar.FindCVar('drpg_menu_height');
        i.ResetToDefault();
        i = CVar.FindCVar('drpg_menu_background_border');
        i.ResetToDefault();

// HUD
        i = CVar.FindCVar('drpg_hud_preview');
        i.ResetToDefault();
        i = CVar.FindCVar('drpg_hud_width');
        i.ResetToDefault();
        i = CVar.FindCVar('drpg_hud_height');
        i.ResetToDefault();
        i = CVar.FindCVar('drpg_stats_x');
        i.ResetToDefault();
        i = CVar.FindCVar('drpg_stats_y');
        i.ResetToDefault();
        i = CVar.FindCVar('drpg_skill_x');
        i.ResetToDefault();
        i = CVar.FindCVar('drpg_skill_y');
        i.ResetToDefault();
        i = CVar.FindCVar('drpg_stateffect_x');
        i.ResetToDefault();
        i = CVar.FindCVar('drpg_stateffect_y');
        i.ResetToDefault();
        i = CVar.FindCVar('drpg_credits_x');
        i.ResetToDefault();
        i = CVar.FindCVar('drpg_credits_y');
        i.ResetToDefault();
        i = CVar.FindCVar('drpg_combo_x');
        i.ResetToDefault();
        i = CVar.FindCVar('drpg_combo_y');
        i.ResetToDefault();
        i = CVar.FindCVar('drpg_stim_x');
        i.ResetToDefault();
        i = CVar.FindCVar('drpg_stim_y');
        i.ResetToDefault();
        i = CVar.FindCVar('drpg_aug_x');
        i.ResetToDefault();
        i = CVar.FindCVar('drpg_aug_y');
        i.ResetToDefault();
        i = CVar.FindCVar('drpg_mission_x');
        i.ResetToDefault();
        i = CVar.FindCVar('drpg_mission_y');
        i.ResetToDefault();
        i = CVar.FindCVar('drpg_auratimer_x');
        i.ResetToDefault();
        i = CVar.FindCVar('drpg_auratimer_y');
        i.ResetToDefault();
        i = CVar.FindCVar('drpg_powerup_x');
        i.ResetToDefault();
        i = CVar.FindCVar('drpg_powerup_y');
        i.ResetToDefault();
        i = CVar.FindCVar('drpg_event_x');
        i.ResetToDefault();
        i = CVar.FindCVar('drpg_event_y');
        i.ResetToDefault();
        i = CVar.FindCVar('drpg_drla_x');
        i.ResetToDefault();
        i = CVar.FindCVar('drpg_drla_y');
        i.ResetToDefault();
        i = CVar.FindCVar('drpg_ld_x');
        i.ResetToDefault();
        i = CVar.FindCVar('drpg_ld_y');
        i.ResetToDefault();
        i = CVar.FindCVar('drpg_coopview_x');
        i.ResetToDefault();
        i = CVar.FindCVar('drpg_coopview_y');
        i.ResetToDefault();
        i = CVar.FindCVar('drpg_multiplayer_hud');
        i.ResetToDefault();
        i = CVar.FindCVar('drpg_multiplayer_x');
        i.ResetToDefault();
        i = CVar.FindCVar('drpg_multiplayer_y');
        i.ResetToDefault();
        i = CVar.FindCVar('drpg_turret_x');
        i.ResetToDefault();
        i = CVar.FindCVar('drpg_turret_y');
        i.ResetToDefault();
        i = CVar.FindCVar('drpg_skill_wheelspeed');
        i.ResetToDefault();
        i = CVar.FindCVar('drpg_stats_alwaysshow');
        i.ResetToDefault();
        i = CVar.FindCVar('drpg_aug_alwaysshow');
        i.ResetToDefault();
        i = CVar.FindCVar('drpg_drla_hud_staminabar');
        i.ResetToDefault();

// Misc
        i = CVar.FindCVar('drpg_debug');
        i.ResetToDefault();
        i = CVar.FindCVar('drpg_debug_gui');
        i.ResetToDefault();
        i = CVar.FindCVar('drpg_debug_drla');
        i.ResetToDefault();
        i = CVar.FindCVar('drpg_tips');
        i.ResetToDefault();
        i = CVar.FindCVar('drpg_regen_movement');
        i.ResetToDefault();
        i = CVar.FindCVar('drpg_move_aug_battery_regen');
        i.ResetToDefault();
        i = CVar.FindCVar('drpg_autosave');
        i.ResetToDefault();
        i = CVar.FindCVar('drpg_monster_dropdist');
        i.ResetToDefault();
        i = CVar.FindCVar('drpg_transport_on_new_map');
        i.ResetToDefault();
        i = CVar.FindCVar('drpg_shotguns');
        i.ResetToDefault();
        i = CVar.FindCVar('drpg_augs_autoreactivate');
        i.ResetToDefault();
        i = CVar.FindCVar('drpg_give_quickinventory');
        i.ResetToDefault();

// Performance
        i = CVar.FindCVar('drpg_toaster');
        i.ResetToDefault();
        i = CVar.FindCVar('drpg_simple_auras');
        i.ResetToDefault();
        i = CVar.FindCVar('drpg_auras_drawdistance');
        i.ResetToDefault();
        i = CVar.FindCVar('drpg_popoffs_drawdistance');
        i.ResetToDefault();
        i = CVar.FindCVar('drpg_virtual_credits');
        i.ResetToDefault();

// Stored Character
        /*        i = CVar.FindCVar('drpg_char_data_len');
                i.ResetToDefault();
                i = CVar.FindCVar('drpg_char_data_0');
                i.ResetToDefault();
                i = CVar.FindCVar('drpg_char_data_1');
                i.ResetToDefault();
                i = CVar.FindCVar('drpg_char_data_2');
                i.ResetToDefault();
                i = CVar.FindCVar('drpg_char_data_3');
                i.ResetToDefault();
                i = CVar.FindCVar('drpg_char_data_4');
                i.ResetToDefault();
                i = CVar.FindCVar('drpg_char_data_5');
                i.ResetToDefault();
                i = CVar.FindCVar('drpg_char_data_6');
                i.ResetToDefault();
                i = CVar.FindCVar('drpg_char_data_7');
                i.ResetToDefault();
                i = CVar.FindCVar('drpg_char_data_8');
                i.ResetToDefault();
                i = CVar.FindCVar('drpg_char_data_9');
                i.ResetToDefault();
                i = CVar.FindCVar('drpg_char_data_10');
                i.ResetToDefault();
                i = CVar.FindCVar('drpg_char_data_11');
                i.ResetToDefault();
                i = CVar.FindCVar('drpg_char_data_12');
                i.ResetToDefault();
                i = CVar.FindCVar('drpg_char_data_13');
                i.ResetToDefault();
                i = CVar.FindCVar('drpg_char_data_14');
                i.ResetToDefault();
                i = CVar.FindCVar('drpg_char_data_15');
                i.ResetToDefault();*/

// --------------------------------------------------
// Starting Options
//

// Outpost Start
        i = CVar.FindCVar('drpg_startmap');
        i.ResetToDefault();
        i = CVar.FindCVar('drpg_addstartmap');
        i.ResetToDefault();

// Funds
        i = CVar.FindCVar('drpg_start_credits');
        i.ResetToDefault();
        i = CVar.FindCVar('drpg_start_modules');
        i.ResetToDefault();
        i = CVar.FindCVar('drpg_start_turretparts');
        i.ResetToDefault();

// Level/Rank
        i = CVar.FindCVar('drpg_start_level');
        i.ResetToDefault();
        i = CVar.FindCVar('drpg_start_rank');
        i.ResetToDefault();

// Stats
        i = CVar.FindCVar('drpg_start_strength');
        i.ResetToDefault();
        i = CVar.FindCVar('drpg_start_defense');
        i.ResetToDefault();
        i = CVar.FindCVar('drpg_start_vitality');
        i.ResetToDefault();
        i = CVar.FindCVar('drpg_start_energy');
        i.ResetToDefault();
        i = CVar.FindCVar('drpg_start_regeneration');
        i.ResetToDefault();
        i = CVar.FindCVar('drpg_start_agility');
        i.ResetToDefault();
        i = CVar.FindCVar('drpg_start_capacity');
        i.ResetToDefault();
        i = CVar.FindCVar('drpg_start_luck');
        i.ResetToDefault();

// Weapons/Ammo/Armor
        i = CVar.FindCVar('drpg_start_weapon_pistol');
        i.ResetToDefault();
        i = CVar.FindCVar('drpg_start_weapon_shotgun');
        i.ResetToDefault();
        i = CVar.FindCVar('drpg_start_weapon_ssg');
        i.ResetToDefault();
        i = CVar.FindCVar('drpg_start_weapon_chaingun');
        i.ResetToDefault();
        i = CVar.FindCVar('drpg_start_weapon_rocketlauncher');
        i.ResetToDefault();
        i = CVar.FindCVar('drpg_start_weapon_plasmarifle');
        i.ResetToDefault();
        i = CVar.FindCVar('drpg_start_weapon_bfg');
        i.ResetToDefault();
        i = CVar.FindCVar('drpg_start_weapon_chainsaw');
        i.ResetToDefault();
        i = CVar.FindCVar('drpg_start_ammo_bullet');
        i.ResetToDefault();
        i = CVar.FindCVar('drpg_start_ammo_shell');
        i.ResetToDefault();
        i = CVar.FindCVar('drpg_start_ammo_rocket');
        i.ResetToDefault();
        i = CVar.FindCVar('drpg_start_ammo_cell');
        i.ResetToDefault();
        i = CVar.FindCVar('drpg_start_armor');
        i.ResetToDefault();
        i = CVar.FindCVar('drpg_start_medkit');
        i.ResetToDefault();

// Augs
        i = CVar.FindCVar('drpg_start_aug_amount');
        i.ResetToDefault();
        i = CVar.FindCVar('drpg_start_aug_canisters');
        i.ResetToDefault();
        i = CVar.FindCVar('drpg_start_aug_upgrades');
        i.ResetToDefault();
        i = CVar.FindCVar('drpg_start_aug_slots');
        i.ResetToDefault();

// Shield
        i = CVar.FindCVar('drpg_start_shield_type');
        i.ResetToDefault();
        i = CVar.FindCVar('drpg_start_shield_type_acc');
        i.ResetToDefault();
        i = CVar.FindCVar('drpg_start_shield_amount_body');
        i.ResetToDefault();
        i = CVar.FindCVar('drpg_start_shield_amount_battery');
        i.ResetToDefault();
        i = CVar.FindCVar('drpg_start_shield_amount_capacitor');
        i.ResetToDefault();
        i = CVar.FindCVar('drpg_start_shield_amount_acc');
        i.ResetToDefault();

// Stims
        i = CVar.FindCVar('drpg_start_stim_injectors');
        i.ResetToDefault();
        i = CVar.FindCVar('drpg_start_stim_vials');
        i.ResetToDefault();
        i = CVar.FindCVar('drpg_start_stim_boosters');
        i.ResetToDefault();
        i = CVar.FindCVar('drpg_start_stim_powerups');
        i.ResetToDefault();

// Bonuses
        i = CVar.FindCVar('drpg_start_bonus_goldchips');
        i.ResetToDefault();
        i = CVar.FindCVar('drpg_start_bonus_platchips');
        i.ResetToDefault();
        i = CVar.FindCVar('drpg_start_bonus_shopcard');
        i.ResetToDefault();

// DRLA
        i = CVar.FindCVar('drpg_start_drla_weapon_type');
        i.ResetToDefault();
        i = CVar.FindCVar('drpg_start_drla_weapon_amount');
        i.ResetToDefault();
        i = CVar.FindCVar('drpg_start_drla_armor');
        i.ResetToDefault();
        i = CVar.FindCVar('drpg_start_drla_boots');
        i.ResetToDefault();
        i = CVar.FindCVar('drpg_start_drla_modpacks');
        i.ResetToDefault();
        i = CVar.FindCVar('drpg_start_drla_modpacks_amount');
        i.ResetToDefault();

// --------------------------------------------------
// Skill Wheel
//
        i = CVar.FindCVar('drpg_skillwheel_category_1');
        i.ResetToDefault();
        i = CVar.FindCVar('drpg_skillwheel_index_1');
        i.ResetToDefault();
        i = CVar.FindCVar('drpg_skillwheel_category_2');
        i.ResetToDefault();
        i = CVar.FindCVar('drpg_skillwheel_index_2');
        i.ResetToDefault();
        i = CVar.FindCVar('drpg_skillwheel_category_3');
        i.ResetToDefault();
        i = CVar.FindCVar('drpg_skillwheel_index_3');
        i.ResetToDefault();
        i = CVar.FindCVar('drpg_skillwheel_category_4');
        i.ResetToDefault();
        i = CVar.FindCVar('drpg_skillwheel_index_4');
        i.ResetToDefault();
        i = CVar.FindCVar('drpg_skillwheel_category_5');
        i.ResetToDefault();
        i = CVar.FindCVar('drpg_skillwheel_index_5');
        i.ResetToDefault();
        i = CVar.FindCVar('drpg_skillwheel_category_6');
        i.ResetToDefault();
        i = CVar.FindCVar('drpg_skillwheel_index_6');
        i.ResetToDefault();
        i = CVar.FindCVar('drpg_skillwheel_category_7');
        i.ResetToDefault();
        i = CVar.FindCVar('drpg_skillwheel_index_7');
        i.ResetToDefault();
        i = CVar.FindCVar('drpg_skillwheel_category_8');
        i.ResetToDefault();
        i = CVar.FindCVar('drpg_skillwheel_index_8');
        i.ResetToDefault();
    }
}