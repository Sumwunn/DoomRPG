#include "Defs.h"

#include "ItemData.h"
#include "Menu.h"
#include "Minigame.h"
#include "Mission.h"
#include "RPG.h"
#include "Utils.h"

NamedScript void ItemRoulette(bool Rare)
{
    ItemInfoPtr WheelItems[MAX_ROULETTE_ITEMS];
    bool Started = false;
    bool Finished = false;
    bool Spinning = true;
    bool Repick = true;
    int ChipRarity = 0;
    int ChipAmount = 0;
    int ChipDuds = 0;
    int ChipSpeed = 0;
    int ChipTotal = 0;
    int Rarity = 0;
    int Amount = 0;
    int Radius = 0;
    int Selection = 0;
    int ChipIndex = 0;
    fixed Speed = 0;
    fixed Offset = 0;

    // Wait until rewards are sorted
    while (!RewardsInit) Delay(1);

    Player.InMinigame = true;

    int Duds, Index;

    // Set the HUD Size
    SetHudSize(640, 480, false);

    while (!Finished && Player.InMinigame)
    {
        // Draw Border
        if (GetActivatorCVar("drpg_menu_background_border"))
            DrawBorder("Bor", -1, 8, 85.0, 5.0, 470, 470);

        // Header.
        SetFont("BIGFONT");
        HudMessage("ITEM ROULETTE");
        EndHudMessage(HUDMSG_PLAIN, 0, "Gold", 235.1, 25.0, 0.05);

        // Help
        if (GetActivatorCVar("drpg_menuhelp"))
        {
            SetFont("SMALLFONT");
            HudMessage("Navigate/Change: \Cd%jS/%jS/%jS/%jS\C-\nReady/Play: \Cd%jS\C-\nExit: \Cd%jS\C-",
                       "+forward", "+back", "+moveleft", "+moveright", "+use", "drpg_menu");
            EndHudMessage(HUDMSG_PLAIN, 0, "White", 90.1, 460.0, 0.05);
        }

        // Keep menus closed - This is technically a hack and shouldn't be here
        Player.InMenu = false;
        Player.InShop = false;

        // Freeze and Fade
        SetPlayerProperty(0, 1, PROP_TOTALLYFROZEN);
        FadeRange(0, 0, 0, 0.65, 0, 0, 0, 0.0, 0.25);

        // Populate Wheel Items
        if (Repick)
        {
            // Recalculate Rarity and Amount
            Rarity = (Rare ? 4 : 0) + ChipRarity / 2;
            if (Rarity > MAX_DIFFICULTIES - 1)
                Rarity = MAX_DIFFICULTIES - 1;
            Amount = 10 + ChipAmount;
            Radius = 128 + (Amount * 3);

            // Place Items in Wheel
            for (int i = 0; i < Amount; i++)
                WheelItems[i] = GetRewardItem(Rarity);

            // Place Duds
            Duds = 0;
            while (Duds < (Amount / 2) - (int)(((fixed)Amount / 20.0) * (fixed)ChipDuds))
            {
                Index = Random(0, Amount);
                if (WheelItems[Index] != GetBlankItem())
                {
                    WheelItems[Index] = GetBlankItem();
                    Duds++;
                }
            }

            Repick = false;
        }

        // Recalculate Speed and Total
        if (!Started)
        {
            Speed = 12 - ChipSpeed;
            ChipTotal = 1 + ChipRarity + ChipSpeed + ChipAmount + ChipDuds;
        }

        // Wheel Handling
        for (int i = 0; i < Amount; i++)
        {
            fixed Angle = -0.25 + ((1.0 / Amount) * i) + Offset;
            fixed X = 320.0 + (Radius * Cos(Angle)) + WheelItems[i]->Sprite.XOff;
            fixed Y = 240.0 + (Radius * Sin(Angle)) + WheelItems[i]->Sprite.YOff;

            // Draw Icon
            PrintSprite(WheelItems[i]->Sprite.Name, 0, (int)X, (int)Y, 0.05);
            if (StrLen(WheelItems[i]->Sprite.Name) < 0)
                PrintSprite("SprNone", 0, (int)X, (int)Y, 0.05);

            // Determine the current item near the cursor
            if (Distance2D(320, 240 + Radius, X - WheelItems[i]->Sprite.XOff, Y - WheelItems[i]->Sprite.YOff) < 32)
                Selection = i;
        }

        // Draw Chips
        if (!Started)
        {
            SetFont("BIGFONT");
            HudMessage("%d", ChipTotal);
            EndHudMessage(HUDMSG_PLAIN, 0, (Rare ? "White" : "Gold"), 320, 240, 0.05);
            HudMessage("Rarity: %d", ChipRarity);
            EndHudMessage(HUDMSG_PLAIN, 0, (ChipIndex == 0 ? MenuCursorColor : (Rare ? "White" : "Gold")), 320, 272, 0.05);
            HudMessage("Amount: %d", ChipAmount);
            EndHudMessage(HUDMSG_PLAIN, 0, (ChipIndex == 1 ? MenuCursorColor : (Rare ? "White" : "Gold")), 320, 284, 0.05);
            HudMessage("Duds: %d", ChipDuds);
            EndHudMessage(HUDMSG_PLAIN, 0, (ChipIndex == 2 ? MenuCursorColor : (Rare ? "White" : "Gold")), 320, 296, 0.05);
            HudMessage("Speed: %d", ChipSpeed);
            EndHudMessage(HUDMSG_PLAIN, 0, (ChipIndex == 3 ? MenuCursorColor : (Rare ? "White" : "Gold")), 320, 308, 0.05);
            PrintSprite((Rare ? "CHPPA0" : "CHPGA0"), 0, 320 + 16, 240 + 32, 0.05);
        };

        // Draw Name
        SetFont("BIGFONT");
        HudMessage("%S", (WheelItems[Selection] == GetBlankItem() ? "\CaNothing" : WheelItems[Selection]->Name));
        EndHudMessage(HUDMSG_PLAIN, 0, "White", 320, 240 + Radius + 32, 0.05);

        // Draw Box
        if (Started)
            PrintSprite("ItemBoxH", 0, 320, 240 + Radius, 0.05);

        // Sound Loop
        if (Started)
            PlaySound(0, "menu/click", CHAN_BODY, 0.8, true, ATTN_NORM);

        // Input Handling
        if (CheckInput(BT_USE, KEY_PRESSED, false, PlayerNumber()) && Spinning)
        {
            ActivatorSound("menu/move", 127);
            if (!Started)
                Started = true;
            else
                Spinning = false;
        }
        if (CheckInput(BT_FORWARD, KEY_PRESSED, false, PlayerNumber()) && !Started)
        {
            ActivatorSound("menu/move", 127);
            ChipIndex--;
            if (ChipIndex < 0) ChipIndex = 3;
        }
        if (CheckInput(BT_BACK, KEY_PRESSED, false, PlayerNumber()) && !Started)
        {
            ActivatorSound("menu/move", 127);
            ChipIndex++;
            if (ChipIndex > 3) ChipIndex = 0;
        }
        if (CheckInput(BT_MOVELEFT, KEY_PRESSED, false, PlayerNumber()) && !Started)
        {
            switch (ChipIndex)
            {
            case 0: // Rarity
                if (ChipRarity > 0)
                {
                    ActivatorSound("menu/move", 127);
                    ChipRarity--;
                    ChipTotal--;
                    Repick = true;
                }
                break;
            case 1: // Amount
                if (ChipAmount > 0)
                {
                    ActivatorSound("menu/move", 127);
                    ChipAmount--;
                    ChipTotal--;
                    Repick = true;
                }
                break;
            case 2: // Duds
                if (ChipDuds > 0)
                {
                    ActivatorSound("menu/move", 127);
                    ChipDuds--;
                    ChipTotal--;
                    Repick = true;
                }
                break;
            case 3: // Speed
                if (ChipSpeed > 0)
                {
                    ActivatorSound("menu/move", 127);
                    ChipSpeed--;
                    ChipTotal--;
                }
                break;
            }
        }
        if (CheckInput(BT_MOVERIGHT, KEY_PRESSED, false, PlayerNumber()) && !Started && ChipTotal < (Rare ? CheckInventory("DRPGChipPlatinum") : CheckInventory("DRPGChipGold")))
        {
            switch (ChipIndex)
            {
            case 0: // Rarity
                if (ChipRarity < 10)
                {
                    ActivatorSound("menu/move", 127);
                    ChipRarity++;
                    ChipTotal++;
                    Repick = true;
                }
                break;
            case 1: // Amount
                if (ChipAmount < 10)
                {
                    ActivatorSound("menu/move", 127);
                    ChipAmount++;
                    ChipTotal++;
                    Repick = true;
                }
                break;
            case 2: // Duds
                if (ChipDuds < 10)
                {
                    ActivatorSound("menu/move", 127);
                    ChipDuds++;
                    ChipTotal++;
                    Repick = true;
                }
                break;
            case 3: // Speed
                if (ChipSpeed < 10)
                {
                    ActivatorSound("menu/move", 127);
                    ChipSpeed++;
                    ChipTotal++;
                }
                break;
            }
        }

        // Slowly decrease the spin when done
        if (!Spinning)
            Speed -= 0.05;

        // Spin
        Offset += (Speed * 0.005);

        // Finished
        if (!Spinning && Speed <= 0)
        {
            // Draw Name
            SetFont("BIGFONT");
            HudMessage("%S", (WheelItems[Selection] == GetBlankItem() ? "\CaNothing" : WheelItems[Selection]->Name));
            EndHudMessage(HUDMSG_FADEOUT, 0, (WheelItems[Selection] == GetBlankItem() ? "Red" : "Green"), 320, 240 + Radius + 32, 2.0, 1.0);

            // Draw Box
            PrintSpriteFade("ItemBoxR", 0, 320, 240 + Radius, 0.25, 0.75);

            // Draw Icon
            PrintSpriteFade(WheelItems[Selection]->Sprite.Name, 0, 320 + WheelItems[Selection]->Sprite.XOff, 240 + Radius + WheelItems[Selection]->Sprite.YOff, 2.0, 1.0);

            // Give the item if it wasn't a blank
            if (WheelItems[Selection] != GetBlankItem())
            {
                ActivatorSound("transfer/complete", 127);

                // Item
                str ItemActor = WheelItems[Selection]->Actor;

                // Spawn Item and try to pick it up
                SpawnForced(ItemActor, GetActorX(0), GetActorY(0), GetActorZ(0), 0, 0);
                SetActorVelocity(Player.TID, 0.01, 0.01, 0, true, false);
            }
            else
                ActivatorSound("menu/error", 127);

            // Take Chips
            if (Rare)
                TakeInventory("DRPGChipPlatinum", ChipTotal);
            else
                TakeInventory("DRPGChipGold", ChipTotal);

            StopSound(0, CHAN_BODY); // Stop the looping sound
            Finished = true;
        }

        Delay(1);
    }

    SetPlayerProperty(0, 0, PROP_TOTALLYFROZEN);
    Player.InMinigame = false;
}
