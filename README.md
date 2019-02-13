## Fork overview

This fork is based on Sumwunn's fork of DRPG and focused on tweaking gameplay in a strange and exciting ways.

## New settings

XP based on monster health (drpg_xp_health_awareness)
0-1 (step 0.1), default: 0
0: Vanilla DRPG (XP award is based on monster base maximum health)
0.5: Mean of base maximum health and actual maximum health goes to XP
1: All of the monster's actual maximum health goes to XP

Monster Vitality Effect (drpg_monster_vitality_effect)
1-100 (step 1), default: 9
0: Monster's health is unaffected by vitality
9: Vanilla DRPG (monster with 1000 vitality will have 10 times health of a base maximum)
100: Monster with 1000 vitality will have 101 times health of a base maximum
This option will increase health of all monsters according to their base health. So strong monsters will become even stronger.

Monster Health Boost (drpg_monster_vitality_boost)
0-10 (step 0.1), default: 0
0: Vanilla DRPG (no additional HP boost)
5: Every monster receives additional 5 health per vitality point
10: Every monster receives additional 10 health per vitality point
This option adds same health to every monster (considering their vitality), so it's much more noticeable on weaker monsters.

Invulnerability resists status (drpg_invulnerability_plus)
On-Off, default: Off
On: Makes the player fully immune to status effect when invulnerability is active. 
Off: Vanilla DRPG (player could receive Curse and some other status effects when invulnerable)

Movement Recharger efficiency % (drpg_move_aug_battery_regen)
0-100 (step 10), default: 0
0: Vanilla DRPG (battery stops regenerating if the player is moving)
50: Battery regenerates only half as fast if the player is moving
100: Battery regeneration doesn't consider player movement

Skill Overdrive % Limit (drpg_overdrive_limit)
0-1000 (step 1), default: 1000
0: Disables overdrive completely
500: Player with 100 max energy cannot go below 500 EP 
1000: Vanilla DRPG (unlimited overdrive)
Limits energy which could be used for skill overdrive to prevent OP summons or stuff like that in early game.  

Plain Health Bars (drpg_healthbars_plain)
On-Off, default: Off
On: Makes healthbars single-layer.
Off: Vanilla DRPG (multi-layer healthbars)
Default behaviour (one healthbar layer per every 100% of base health) breaks for monsters which have more than 1000% of base health. So this setting is essential for higher than default vitality options.