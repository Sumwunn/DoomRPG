#include "Defs.h"
#include "RPG.h"

#include "ItemData.h"
#include "Map.h"
#include "Menu.h"
#include "Minigame.h"
#include "Shield.h"
#include "Shop.h"
#include "Stats.h"
#include "Utils.h"

// Global Shop Card Rank
RPGGlobal int GlobalShopCard;

// Shop Special
RPGGlobal bool        ShopSpecialBought;
RPGGlobal int         ShopSpecialTimer;
RPGGlobal ItemInfoPtr ShopSpecialItem;

RPGMap int ShopSpotID = 1000;

// Open the Shop
NamedScript KeyBind void OpenShop(bool OpenLocker)
{
    // If you're dead, return
    if (GetActorProperty(0, APROP_Health) <= 0) return;

    // If you're in an Outpost menu, return
    if (Player.OutpostMenu > 0) return;

    // If you're in any minigames, return
    if (Player.InMinigame) return;

    // If you're looking inside a crate, return
    if (Player.CrateOpen) return;

    // If settings say no
    if (!CurrentLevel->UACBase && !GetCVar("drpg_shoptype"))
    {
        ActivatorSound("menu/invalid", 127);
        SetFont("BIGFONT");
        PrintError("This is only accessible within the UAC Outpost");
        return;
    }

    // Close the main menu if it's open
    Player.InMenu = false;

    // Sanity check for pressing use while the shop is open in front of the counter in the Outpost
    if (Player.InShop && CheckInput(BT_USE, KEY_HELD, false, PlayerNumber())) return;

    if (Player.InShop)
    {
        ActivatorSound("menu/leave", 127);
        SetPlayerProperty(0, 0, PROP_TOTALLYFROZEN);
        Player.InShop = false;
    }
    else
    {
        ActivatorSound("menu/shop", 127);
        Player.InShop = true;
        if (OpenLocker)
            Player.LockerMode = true;
    }
}

// Auto-Sell & Auto-Store
NamedScript void UpdateShopAutoList()
{
    for (int i = 0; i < ItemCategories; i++)
    {
        for (int j = 0; j < ItemMax[i]; j++)
        {
            // Remove AT_NONE items that exist in the Auto-Sell or Auto-Store arrays
            if (Player.ItemAutoMode[i][j] == AT_NONE)
            {
                ItemInfoPtr Item = &ItemData[i][j];

                zsDynArrayUtils("Auto-Sell", 4, (int)Item, PlayerNumber());
                zsDynArrayUtils("Auto-Store", 4, (int)Item, PlayerNumber());
            }

            // Auto-Sell
            if (Player.ItemAutoMode[i][j] == AT_SELL)
            {
                ItemInfoPtr Item = &ItemData[i][j];

                // Get array size
                int arraySize = zsDynArrayUtils("Auto-Sell", 3, NULL, PlayerNumber());

                if (DebugLog)
                {
                    Log("\CdDEBUG: \C-Sell List Population: %i", arraySize);
                    Log("\CdDEBUG: \C-Adding %S to auto-sell @ %p", ItemData[i][j].Name, Item);
                }

                // Add item to array
                zsDynArrayUtils("Auto-Sell", 1, (int)Item, PlayerNumber());
            }

            // Auto-Store
            else if (Player.ItemAutoMode[i][j] == AT_STORE)
            {
                ItemInfoPtr Item = &ItemData[i][j];

                // Get array size
                int arraySize = zsDynArrayUtils("Auto-Store", 3, NULL, PlayerNumber());

                if (DebugLog)
                {
                    Log("\CdDEBUG: \C-Store List Population: %i", arraySize);
                    Log("\CdDEBUG: \C-Adding %S to auto-store @ %p", ItemData[i][j].Name, Item);
                }

                // Add item to array
                zsDynArrayUtils("Auto-Store", 1, (int)Item, PlayerNumber());
            }

            // Commented out for console spam
            //if (DebugLog)
            //    Log("\CdDEBUG: \C-Completed Item #%i, %S", ItemData[i][j].Name);
        }
    }

    if (DebugLog)
        Log("\CdDEBUG: \C-Completed AutoUpdateShopList");
}

void ShopItemTryAutoDeposit(ItemInfoPtr Item)
{
    bool deposited = false;

    // If Shop/Locker anywhere is off
    if (!CurrentLevel->UACBase && !GetCVar("drpg_shoptype")) return;

    // Special handling for DRLA weapons since you can only keep one weapon with mods in the locker
    if (CompatMode == COMPAT_DRLA && Item->Category == 0 && Player.Locker[Item->Category][Item->Index] > 0) return;

    while (CheckInventory(Item->Actor) > 0 && (Player.EP >= LOCKER_EPRATE || CurrentLevel->UACBase))
    {
        DepositItem(Item->Category, Item->Index, false, true);
        deposited = true;
    }

    if (deposited)
        ActivatorSound("shop/autostore", 127);
}

NamedScript void ShopItemAutoHandler()
{
    bool ItemsCurrent = false;
    bool ButtonHeld = false;
    // These are actually too big for the script auto handler to allocate properly, so they need to be static-scope here.
    RPGMap static int Items[ITEM_CATEGORIES][ITEM_MAX];
    RPGMap static int PrevItems[ITEM_CATEGORIES][ITEM_MAX];

    UpdateShopAutoList();

    while (true)
    {
        // Auto-Sell/Store doesn't need to run constantly like run-pickup does.
        if (!GetActivatorCVar("drpg_pickup_behavior")) Delay(5);

        // Auto-Sell
        bool CanSellItems = Player.RankLevel > 0 || CurrentLevel->UACBase;
        bool UseAutoDepositFallback = GetActivatorCVar("drpg_autosell_lockerfallback");
        int sellArraySize = zsDynArrayUtils("Auto-Sell", 3, NULL, PlayerNumber());
        int storeArraySize = zsDynArrayUtils("Auto-Store", 3, NULL, PlayerNumber());

        // Auto-Sell
        if (CanSellItems || UseAutoDepositFallback)
            for (int i = 0; i < sellArraySize; i++) // Process each item in Auto-Sell array for selling
            {
                ItemInfoPtr Item = (ItemInfoPtr)zsDynArrayUtils("Auto-Sell", 2, i, PlayerNumber()); // Get Item from array, where i is the array position
                int Quantity = CheckInventory(Item->Actor);

                // Keep
                if (Player.ItemKeep[Item->Category][Item->Index])
                    Quantity--;

                if (Quantity > 0 && !(Player.Mission.Active && Player.Mission.Type == MT_COLLECT && !StrCmp(Player.Mission.Item->Actor, Item->Actor)))
                {
                    if (CanSellItems)
                        SellItem(Item->Actor, Quantity, true);
                    else if (UseAutoDepositFallback)
                        ShopItemTryAutoDeposit(Item);
                }
            }

        // Auto-Store
        if (Player.EP >= LOCKER_EPRATE || CurrentLevel->UACBase)
            for (int i = 0; i < storeArraySize; i++)
            {
                ItemInfoPtr Item = (ItemInfoPtr)zsDynArrayUtils("Auto-Store", 2, i, PlayerNumber()); // Get Item from array

                if (CheckInventory(Item->Actor) > 0)
                    ShopItemTryAutoDeposit(Item);
            }

        // Run-pickup.
        if (GetActivatorCVar("drpg_pickup_behavior"))
        {
            // Get input.
            ButtonHeld = CheckInput(BT_SPEED, KEY_HELD, false, PlayerNumber());

            // For post-checks
            if (ButtonHeld)
                for (int i = 0; i < ItemCategories; i++)
                    for (int j = 0; j < ItemMax[i]; j++)
                        Items[i][j] = CheckInventory(ItemData[i][j].Actor);

            // Run-pickup behavior stuff
            if (ItemsCurrent && ButtonHeld)
            {
                for (int i = 0; i < ItemCategories; i++)
                    for (int j = 0; j < ItemMax[i]; j++)
                    {
                        ItemInfoPtr Item = &ItemData[i][j];

                        // Auto-Sell
                        if (!Player.InShop && GetActivatorCVar("drpg_pickup_behavior") == 1 && Items[i][j] > PrevItems[i][j])
                            if (CheckInventory(Item->Actor) > 0)
                                SellItem(Item->Actor, true, true);

                        // Auto-Store
                        if (!Player.InShop && GetActivatorCVar("drpg_pickup_behavior") == 2 && Items[i][j] > PrevItems[i][j])
                            ShopItemTryAutoDeposit(Item);
                    }
            }

            // Run-pickup has to run constantly or it might miss what the player ran over.
            Delay(1);

            // Prevent using old Items so we don't sell recently picked up stuff.
            if (!ButtonHeld)
                ItemsCurrent = false;

            if (ButtonHeld)
                ItemsCurrent = true;
            for (int i = 0; i < ItemCategories; i++)
                for (int j = 0; j < ItemMax[i]; j++)
                    PrevItems[i][j] = Items[i][j];
        }
    }
}

void ShopLoop()
{
    ItemInfoPtr ItemPtr = &ItemData[Player.ShopPage][Player.ShopIndex];
    int Cost = ItemPtr->Price - ItemPtr->Price * Player.ShopDiscount / 100;
    int Rank = ItemPtr->Rank;
    int SellPrice;
    int Color;

    // Freeze the Player
    SetPlayerProperty(0, 1, PROP_TOTALLYFROZEN);

    // Get the sell price
    SellPrice = GetSellPrice(NULL, 1);

    // Set the HUD Size
    SetHudSize(GetActivatorCVar("drpg_menu_width"), GetActivatorCVar("drpg_menu_height"), true);

    // Draw cords
    fixed X = GetActivatorCVar("drpg_menu_x");
    fixed Y = GetActivatorCVar("drpg_menu_y");

    // Draw Border
    // These are pushed back a bit so the border doesn't overlap anything
    if (GetActivatorCVar("drpg_menu_background_border"))
        DrawBorder("Bor", -1, 8, X + (-5.0), Y + (-3.0), 470, 470);

    // Switch to Locker mode if Shop Anywhere CVAR is set to Locker
    if (GetCVar("drpg_shoptype") == 1 && !CurrentLevel->UACBase)
        Player.LockerMode = true;

    // Setup dynamic Medikit refill pricing
    ItemData[2][0].Price = Player.MedkitMax * 3;

    // Compatibility Handling - DoomRL Arsenal Extended
    if (CompatModeEx == COMPAT_DRLAX)
    {
        // If the player's class is Phase Sisters, replace the Phase Device to a Phase Sisters Device
        if (PlayerClass(PlayerNumber()) == 9)
        {
            ItemData[4][24].Actor = "RLPhaseSistersDevice";
            ItemData[4][25].Actor = "RLPhaseSistersHomingDevice";
        }
        else
        {
            ItemData[4][24].Actor = "RLPhaseDevice";
            ItemData[4][25].Actor = "RLHomingPhaseDevice";
        }
    }

    SetFont("BIGFONT");
    if (Player.LockerMode)
    {
        // Title and Page
        HudMessage("\ChLocker \C-- %S\C- \Cd(%d/%d)", ItemCategoryNames[Player.ShopPage], Player.ShopPage + 1, ItemCategories);
        EndHudMessage(HUDMSG_PLAIN, 0, "White", X + 24.1, Y + 24.0, 0.05);

        // EP Cost/Inventory Count
        if (GetCVar("drpg_inv_capacity"))
        {
            HudMessage("%S%d EP \C-(Inventory: %d/%d)", (CurrentLevel->UACBase ? "\Cc" : "\Cn"), LOCKER_EPRATE, Player.InvItems, CheckInventoryMax());
            EndHudMessage(HUDMSG_PLAIN, 0, "White", X + 24.1, Y + 38.0, 0.05);
        }
        else
        {
            HudMessage("%S%d EP", (CurrentLevel->UACBase ? "\Cc" : "\Cn"), LOCKER_EPRATE);
            EndHudMessage(HUDMSG_PLAIN, 0, "Gold", X + 24.1, Y + 38.0, 0.05);
        }
    }
    else
    {
        // Title and Page
        HudMessage("\CfShop \C-- %S\C- \Cd(%d/%d)", ItemCategoryNames[Player.ShopPage], Player.ShopPage + 1, ItemCategories);
        EndHudMessage(HUDMSG_PLAIN, 0, "White", X + 24.1, Y + 24.0, 0.05);

        // Price
        if (ItemCategoryFlags[Player.ShopPage] & CF_NOBUY && ItemCategoryFlags[Player.ShopPage] & CF_NOSELL) // No Buying or Selling
        {
            HudMessage("(Discount: %d%%)", Player.ShopDiscount);
            EndHudMessage(HUDMSG_PLAIN, 0, "Gold", X + 24.1, Y + 38.0, 0.05);
        }
        else if (ItemCategoryFlags[Player.ShopPage] & CF_NOBUY || Rank == -1) // No Buying
        {
            HudMessage("\Ci%d C \Ck(Discount: %d%%)", SellPrice, Player.ShopDiscount);
            EndHudMessage(HUDMSG_PLAIN, 0, "Gold", X + 24.1, Y + 38.0, 0.05);
        }
        else if (ItemCategoryFlags[Player.ShopPage] & CF_NOSELL) // No Selling
        {
            HudMessage("\Cj%d C \Ck(Discount: %d%%)", Cost, Player.ShopDiscount);
            EndHudMessage(HUDMSG_PLAIN, 0, "Gold", X + 24.1, Y + 38.0, 0.05);
        }
        else // Normal
        {
            HudMessage("\Cj%d C \Ci(%d C) \Ck(Discount: %d%%)", Cost, SellPrice, Player.ShopDiscount);
            EndHudMessage(HUDMSG_PLAIN, 0, "Gold", X + 24.1, Y + 38.0, 0.05);
        }
    }

    // Item Grid
    DrawItemGrid(X, Y);

    // Draw Shop Card
    if (Player.ShopCard > 0)
    {
        str CardSprite;

        SetFont("BIGFONT");
        HudMessage("+%d%%", Player.ShopCard * 5);
        EndHudMessage(HUDMSG_PLAIN, 0, (CurrentLevel->UACBase ? "Yellow" : "Gray"), X + 436.4, Y + 364.0, 0.05);
        if (ItemRanksRemoved)
        {
            SetFont("SMALLFONT");
            HudMessage("No Rank\nLimits");
            EndHudMessage(HUDMSG_PLAIN, 0, "Cyan", X + 433.4, Y + 382.0, 0.05);
        }

        switch(Player.ShopCard)
        {
        case 1:
            CardSprite = "UCRDA0";
            break;
        case 2:
            CardSprite = "UCRDB0";
            break;
        case 3:
            CardSprite = "UCRDC0";
            break;
        case 4:
            CardSprite = "UCRDD0";
            break;
        case 5:
            CardSprite = "UCRDE0";
            break;
        }

        if (CurrentLevel->UACBase)
            PrintSprite(CardSprite, 0, X + 458.0, Y + 384.0, 0.05);
        else
            PrintSpritePulse(CardSprite, 0, X + 458.0, Y + 384.0, 0.5, 64.0, 0.5);
    }

    // Check Input
    if (CheckInput(BT_FORWARD, KEY_REPEAT, false, PlayerNumber()))
    {
        ActivatorSound("menu/move", 127);
        if (CheckInput(BT_SPEED, KEY_HELD, false, PlayerNumber()))
            Player.ShopIndex -= 54;
        else
            Player.ShopIndex -= 9;
        if (Player.ShopIndex < 0) Player.ShopIndex = 0;
    };
    if (CheckInput(BT_BACK, KEY_REPEAT, false, PlayerNumber()))
    {
        ActivatorSound("menu/move", 127);
        if (CheckInput(BT_SPEED, KEY_HELD, false, PlayerNumber()))
            Player.ShopIndex += 54;
        else
            Player.ShopIndex += 9;
        if (Player.ShopIndex > ItemMax[Player.ShopPage] - 1) Player.ShopIndex = ItemMax[Player.ShopPage] - 1;
    }
    if (CheckInput(BT_MOVELEFT, KEY_REPEAT, false, PlayerNumber()))
        if (CheckInput(BT_SPEED, KEY_HELD, false, PlayerNumber()))
        {
            ActivatorSound("menu/move", 127);
            Player.ShopPage--;
            Player.ShopIndex = 0;
            if (Player.ShopPage < 0) Player.ShopPage = (DebugLog ? ItemCategories : ItemCategories - 1);
        }
        else
        {
            ActivatorSound("menu/move", 127);
            Player.ShopIndex--;
            if (Player.ShopIndex < 0) Player.ShopIndex = 0;
        }
    if (CheckInput(BT_MOVERIGHT, KEY_REPEAT, false, PlayerNumber()))
        if (CheckInput(BT_SPEED, KEY_HELD, false, PlayerNumber()))
        {
            ActivatorSound("menu/move", 127);
            Player.ShopPage++;
            Player.ShopIndex = 0;
            if (Player.ShopPage > (DebugLog ? ItemCategories : ItemCategories - 1)) Player.ShopPage = 0;
        }
        else
        {
            ActivatorSound("menu/move", 127);
            Player.ShopIndex++;
            if (Player.ShopIndex > ItemMax[Player.ShopPage] - 1) Player.ShopIndex = ItemMax[Player.ShopPage] - 1;
        }
    if (CheckInput(BT_USE, KEY_PRESSED, false, PlayerNumber()) && !Player.MenuBlock)
        if (CheckInput(BT_SPEED, KEY_HELD, false, PlayerNumber()))
        {
            if (Player.LockerMode)
                WithdrawItem(Player.ShopPage, Player.ShopIndex);
            else
                SellItem(ItemPtr->Actor, 1, false);
        }
        else
        {
            if (Player.LockerMode)
                DepositItem(Player.ShopPage, Player.ShopIndex, false, false);
            else if (Player.ItemAutoMode[Player.ShopPage][Player.ShopIndex] != AT_SELL)
                BuyItem(ItemPtr->Actor);
        }

    if (CheckInput(BT_JUMP, KEY_PRESSED, false, PlayerNumber()) && (GetCVar("drpg_shoptype") == 2 || CurrentLevel->UACBase))
    {
        Player.LockerMode = !Player.LockerMode;
        ActivatorSound("menu/move", 127);
    }
    if (CheckInput(BT_ZOOM, KEY_PRESSED, false, PlayerNumber()))
        if (!(ItemCategoryFlags[Player.ShopPage] & CF_NODROP) && CheckInventory(ItemPtr->Actor) > 0 && !(CompatMode == COMPAT_DRLA && (Player.ShopPage == 0 || Player.ShopPage == 3 || Player.ShopPage == 8 || Player.ShopPage == 9)))
        {
            int Success = DropPlayerItem(ItemPtr->Actor);
            if (Success)
                TakeInventory(ItemPtr->Actor, 1);
            else
            {
                PrintError("You cannot drop this item here");
                ActivatorSound("menu/error", 127);
            }
        }
    if (CheckInput(BT_ATTACK, KEY_PRESSED, false, PlayerNumber()))
    {
        if (CheckInput(BT_SPEED, KEY_HELD, false, PlayerNumber()))
        {
            Player.ItemKeep[Player.ShopPage][Player.ShopIndex] = !Player.ItemKeep[Player.ShopPage][Player.ShopIndex];
            ActivatorSound("menu/move", 127);
        }
        else
        {
            if (Player.LockerMode)
            {
                if (Player.ItemAutoMode[Player.ShopPage][Player.ShopIndex] != AT_STORE)
                    Player.ItemAutoMode[Player.ShopPage][Player.ShopIndex] = AT_STORE;
                else
                    Player.ItemAutoMode[Player.ShopPage][Player.ShopIndex] = AT_NONE;
                ActivatorSound("menu/move", 127);
            }
            else if (!(ItemCategoryFlags[Player.ShopPage] & CF_NOSELL))
            {
                if (Player.ItemAutoMode[Player.ShopPage][Player.ShopIndex] != AT_SELL)
                    Player.ItemAutoMode[Player.ShopPage][Player.ShopIndex] = AT_SELL;
                else
                    Player.ItemAutoMode[Player.ShopPage][Player.ShopIndex] = AT_NONE;
                ActivatorSound("menu/move", 127);
            }
        }

        UpdateShopAutoList();
    }
    if (CheckInput(BT_ALTATTACK, KEY_ONLYPRESSED, false, PlayerNumber()) && Player.LockerMode)
    {
        if (CurrentLevel->UACBase)
        {
            for (int i = 0; i < ItemMax[Player.ShopPage]; i++)
                while (Player.Locker[Player.ShopPage][i] > 0)
                    if (WithdrawItem(Player.ShopPage, i) == 0)
                        break;
        }
        else
        {
            PrintError("You can only bulk withdraw while in the Outpost");
            ActivatorSound("menu/error", 127);
        }
    }

    // Reset the menu block
    Player.MenuBlock = false;
}

void BuyItem(str Item)
{
    ItemInfoPtr ItemPtr = &ItemData[Player.ShopPage][Player.ShopIndex];
    int Cost = ItemPtr->Price - ItemPtr->Price * Player.ShopDiscount / 100;
    int Spawned;

    // If you're not the required rank, return
    if (ItemPtr->Rank > Player.RankLevel)
    {
        PrintError(StrParam("You need Rank %d (%S) to buy this item", ItemPtr->Rank, LongRanks[ItemPtr->Rank]));
        ActivatorSound("menu/error", 127);

        return;
    }

    // If the item (or category) is flagged to never be buyable, return
    if (ItemPtr->Rank == -1 || ItemCategoryFlags[Player.ShopPage] & CF_NOBUY)
    {
        PrintError("You cannot buy this item");
        ActivatorSound("menu/error", 127);

        return;
    }

    // If you don't have enough Credits, return
    if (CheckInventory("DRPGCredits") < ItemPtr->Price - ItemPtr->Price * Player.ShopDiscount / 100)
    {
        PrintError("You don't have enough Credits to buy this item");
        ActivatorSound("menu/error", 127);

        return;
    }

    // If the item has no cost, return
    if (Cost == 0)
    {
        ActivatorSound("menu/error", 127);

        return;
    }

    // Spawn item on ShopSpotID if Shop Anywhere CVAR is set to Locker or Closed
    if (CurrentLevel->UACBase && GetCVar("drpg_shoptype") < 2)
        Spawned = SpawnSpotForced(Item, ShopSpotID, 0, 0);
    else
    {
        Spawned = SpawnForced(Item, GetActorX(0), GetActorY(0), GetActorZ(0), 0, 0);
        SetActorVelocity(Player.TID, 0.01, 0.01, 0, true, false);
    }

    if (Spawned > 0)
    {
        ActivatorSound("menu/buy", 127);
        TakeInventory("DRPGCredits", Cost);
    }
    else
    {
        PrintError("Could not teleport the requested item to your location");
        ActivatorSound("menu/error", 127);
    }
}

int GetSellPrice(str Item, int Amount)
{
    ItemInfoPtr ItemPtr = &ItemData[Player.ShopPage][Player.ShopIndex];
    int SellCost;

    // Just use the current index if item is blank
    if (Item == NULL)
        SellCost = ItemPtr->Price;

    // Iterate to find the item
    for (int i = 0; i < ItemCategories; i++)
        for (int j = 0; j < ItemMax[i]; j++)
        {
            ItemInfoPtr ItemIterPtr = &ItemData[i][j];

            if (ItemIterPtr->Actor == Item)
            {
                SellCost = ItemIterPtr->Price;
                break;
            }
        }

    // 1/20th normal buying price
    SellCost /= 20;

    // Multiply by amount
    SellCost *= Amount;

    // Make sure you always get at least 1 Credit from a sale
    if (SellCost == 0) SellCost = 1;

    return SellCost;
}

int SellItem(str Item, int SellAmount, bool AutoSold)
{
    int SellCost;

    // You must be at least Rank 1 or in the Outpost to sell items
    if (Player.RankLevel == 0 && !CurrentLevel->UACBase)
    {
        PrintError("You cannot sell items outside the Outpost until you reach the first rank");
        ActivatorSound("menu/error", 127);

        return 0;
    }

    // If you're on a page that doesn't allow selling or you don't have any of the required item, fail
    if (!AutoSold && (CheckInventory(Item) == 0 || ItemCategoryFlags[Player.ShopPage] & CF_NOSELL))
    {
        PrintError("You cannot sell these items");
        ActivatorSound("menu/error", 127);

        return 0;
    }

    if (SellAmount == 0)
        SellAmount = CheckInventory(Item);

    ItemInfoPtr Info = FindItem(Item);

    // Get Sell Price
    SellCost = GetSellPrice(Item, SellAmount);

    // Sell the Item
    if (CheckInventory(Item) >= SellAmount)
    {
        if (AutoSold)
            ActivatorSound("shop/autosell", 127);
        else
            ActivatorSound("menu/sell", 127);
        TakeInventory(Item, SellAmount);

        // DoomRL Compatibility
        if (CompatMode == COMPAT_DRLA)
            for (int i = 0; i < SellAmount; i++)
                RemoveDRLAItem(Info->Category, Info->Index);

        GiveInventory("DRPGCredits", SellCost);
    }
    else
    {
        PrintError("You are not carrying any of the specified item");
        ActivatorSound("menu/error", 127);
    }

    return SellCost;
}

void DepositItem(int Page, int Index, bool CharSave, bool NoSound)
{
    ItemInfoPtr ItemPtr = &ItemData[Page][Index];
    int *LockerAmount = &Player.Locker[Page][Index];

    // Stop if this item cannot be deposited
    if (ItemCategoryFlags[Page] & CF_NOSTORE) return;

    if ((Player.EP >= LOCKER_EPRATE || CurrentLevel->UACBase || CharSave) && CheckInventory(ItemPtr->Actor) > 0)
    {
        // DoomRL Compatibility
        if (CompatMode == COMPAT_DRLA)
        {
            if (Page == 0) // Weapons
            {
                // You can only store one of a weapon type
                if (*LockerAmount > 0 && !CharSave)
                {
                    PrintError("You can only store one of a weapon type in the Locker");
                    ActivatorSound("menu/error", 127);
                    return;
                }

                // Store the weapons modpack data
                if (!CharSave || (CharSave && (CheckInventory(StrParam("%SModLimit", ItemPtr->Actor)) > Player.WeaponMods[Index].Total || CheckInventory(StrParam("%SDemonArtifacts", ItemPtr->Actor)) > Player.WeaponMods[Index].Artifacts)))
                {
                    Player.WeaponMods[Index].Total = CheckInventory(StrParam("%SModLimit", ItemPtr->Actor));
                    if (ItemPtr->CompatMods & RL_POWER_MOD)
                        Player.WeaponMods[Index].Power = CheckInventory(StrParam("%SPowerMod", ItemPtr->Actor));
                    if (ItemPtr->CompatMods & RL_BULK_MOD)
                        Player.WeaponMods[Index].Bulk = CheckInventory(StrParam("%SBulkMod", ItemPtr->Actor));
                    if (ItemPtr->CompatMods & RL_AGILITY_MOD)
                        Player.WeaponMods[Index].Agility = CheckInventory(StrParam("%SAgilityMod", ItemPtr->Actor));
                    if (ItemPtr->CompatMods & RL_TECH_MOD)
                        Player.WeaponMods[Index].Technical = CheckInventory(StrParam("%STechnicalMod", ItemPtr->Actor));
                    if (ItemPtr->CompatMods & RL_SNIPER_MOD)
                        Player.WeaponMods[Index].Sniper = CheckInventory(StrParam("%SSniperMod", ItemPtr->Actor));
                    if (ItemPtr->CompatMods & RL_FIREST_MOD)
                        Player.WeaponMods[Index].Firestorm = CheckInventory(StrParam("%SFirestormMod", ItemPtr->Actor));
                    if (ItemPtr->CompatMods & RL_NANO_MOD)
                        Player.WeaponMods[Index].Nano = CheckInventory(StrParam("%SNanoMod", ItemPtr->Actor));
                    if (ItemPtr->CompatMods & RL_DEMON_MOD)
                        Player.WeaponMods[Index].Artifacts = CheckInventory(StrParam("%SDemonArtifacts", ItemPtr->Actor));
                }

                // Check DRLA set bonuses
                CheckDRLASetWeapons();
            }

            RemoveDRLAItem(Page, Index);
        }

        TakeInventory(ItemPtr->Actor, 1);
        if (!CurrentLevel->UACBase && !CharSave)
            Player.EP -= LOCKER_EPRATE;
        (*LockerAmount)++;
        if (CharSave && CompatMode == COMPAT_DRLA && Page == 0 && *LockerAmount > 1) // Prevent dumping multiple weapons in the locker in DRLA saves
            *LockerAmount = 1;
        if (!NoSound)
            ActivatorSound("menu/move", 127);
    }
    else if (!NoSound)
    {
        if (Player.EP < LOCKER_EPRATE)
            PrintError("Not enough EP to deposit this item");
        else
            PrintError("You're not carrying any of the specified item");
        ActivatorSound("menu/error", 127);
    }
}

int WithdrawItem(int Page, int Index)
{
    ItemInfoPtr ItemPtr = &ItemData[Page][Index];
    int *LockerAmount = &Player.Locker[Page][Index];

    // Stop if this item cannot be withdrawn
    if (ItemCategoryFlags[Page] & CF_NOSTORE) return 0;

    // Stop if you're trying to withdraw your final copy while Keep is enabled
    if (Player.ItemKeep[Page][Index] && *LockerAmount <= 1)
        return 0;

    // Spawning
    if ((Player.EP >= LOCKER_EPRATE || CurrentLevel->UACBase) && *LockerAmount > 0)
    {
        if (CompatMode == COMPAT_DRLA && Page == 0) // Weapons
        {
            // Don't let you withdraw the weapon if you're already holding one
            if (CheckInventory(ItemPtr->Actor))
            {
                PrintError("You are already carrying this type of weapon");
                ActivatorSound("menu/error", 127);
                return 0;
            }

            int WeaponTID = UniqueTID();
            if (Player.WeaponMods[Index].Total > 0 || Player.WeaponMods[Index].Artifacts > 0) // Weapon was modded
            {
                // Re-add the mods onto the weapon
                SpawnForced(StrParam("%SPickupModded", ItemPtr->Actor), GetActorX(0), GetActorY(0), GetActorZ(0), WeaponTID, 0);

                // Give Weapon Mods tokens
                GiveActorInventory(WeaponTID, StrParam("%SModLimit", ItemPtr->Actor), Player.WeaponMods[Index].Total);
                if (ItemPtr->CompatMods & RL_POWER_MOD)
                    GiveActorInventory(WeaponTID, StrParam("%SPowerMod", ItemPtr->Actor), Player.WeaponMods[Index].Power);
                if (ItemPtr->CompatMods & RL_BULK_MOD)
                    GiveActorInventory(WeaponTID, StrParam("%SBulkMod", ItemPtr->Actor), Player.WeaponMods[Index].Bulk);
                if (ItemPtr->CompatMods & RL_AGILITY_MOD)
                    GiveActorInventory(WeaponTID, StrParam("%SAgilityMod", ItemPtr->Actor), Player.WeaponMods[Index].Agility);
                if (ItemPtr->CompatMods & RL_TECH_MOD)
                    GiveActorInventory(WeaponTID, StrParam("%STechnicalMod", ItemPtr->Actor), Player.WeaponMods[Index].Technical);
                if (ItemPtr->CompatMods & RL_SNIPER_MOD)
                    GiveActorInventory(WeaponTID, StrParam("%SSniperMod", ItemPtr->Actor), Player.WeaponMods[Index].Sniper);
                if (ItemPtr->CompatMods & RL_FIREST_MOD)
                    GiveActorInventory(WeaponTID, StrParam("%SFirestormMod", ItemPtr->Actor), Player.WeaponMods[Index].Firestorm);
                if (ItemPtr->CompatMods & RL_NANO_MOD)
                    GiveActorInventory(WeaponTID, StrParam("%SNanoMod", ItemPtr->Actor), Player.WeaponMods[Index].Nano);
                if (ItemPtr->CompatMods & RL_DEMON_MOD)
                    GiveActorInventory(WeaponTID, StrParam("%SDemonArtifacts", ItemPtr->Actor), Player.WeaponMods[Index].Artifacts);

                // Wipe Weapon Mods data
                Player.WeaponMods[Index].Total = 0;
                Player.WeaponMods[Index].Power = 0;
                Player.WeaponMods[Index].Bulk = 0;
                Player.WeaponMods[Index].Agility = 0;
                Player.WeaponMods[Index].Technical = 0;
                Player.WeaponMods[Index].Sniper = 0;
                Player.WeaponMods[Index].Firestorm = 0;
                Player.WeaponMods[Index].Nano = 0;
                Player.WeaponMods[Index].Artifacts = 0;
            }
            else
            {
                SpawnForced(ItemPtr->Actor, GetActorX(0), GetActorY(0), GetActorZ(0), 0, 0);
            }
        }
        else if (Page == 1) // Ammo
            GiveInventory(ItemPtr->Actor, 1);
        else // Everything else
            SpawnForced(ItemPtr->Actor, GetActorX(0), GetActorY(0), GetActorZ(0), 0, 0);

        SetActorVelocity(Player.TID, 0.01, 0.01, 0, true, false);
        (*LockerAmount)--;
        if (!CurrentLevel->UACBase)
            Player.EP -= LOCKER_EPRATE;

        ActivatorSound("menu/move", 127);
    }
    else
    {
        if (Player.EP < LOCKER_EPRATE)
            PrintError("Not enough EP to withdraw this item");
        else
            PrintError("Locker does not contain any of the specified item");

        ActivatorSound("menu/error", 127);
    }

    return 1;
}

void DrawItemGrid(fixed X, fixed Y)
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

    for (int i = 0; i < Height; i++)
    {
        // Apply base cords
        BaseX = X;

        for (int j = 0; j < Width; j++)
        {
            int Index = j + (i * Width);

            // Calculate offset
            if (Player.ShopIndex >= Height * Width)
                Index += (Player.ShopIndex / (Height * Width)) * (Height * Width);

            // Stop if we're at the end of the list
            if (Index > ItemMax[Player.ShopPage] - 1) break;

            ItemInfoPtr Item = &ItemData[Player.ShopPage][Index];
            str Icon = Item->Sprite.Name;
            str Name = Item->Name;
            int IconXOff = Item->Sprite.XOff;
            int IconYOff = Item->Sprite.YOff;
            int Held = CheckInventory(Item->Actor);
            int Stored = Player.Locker[Player.ShopPage][Index];
            int Rank = Item->Rank;
            int Price = Item->Price;
            bool Unlocked = !((Rank > 0 && Player.RankLevel < Rank));
            bool CanBuy = !(Rank == -1);
            bool CanAfford = (CheckInventory("DRPGCredits") >= Price - Price * Player.ShopDiscount / 100);

            // Placeholder Icon
            if (StrLen(Icon) == 0)
                Icon = "SprNone";

            // Setup X and Y
            X1 = 26.0 + BaseX;
            Y1 = 50.0 + BaseY;
            BoxX = 48.0 + BaseX;
            BoxY = 72.0 + BaseY;
            IconX = 48.0 + BaseX;
            IconY = 72.0 + BaseY;
            IconX += IconXOff;
            IconY += IconYOff;

            // Amount
            if (Held > 0)
            {
                SetFont("BIGFONT");
                HudMessage("%d", Held);
                EndHudMessage(HUDMSG_PLAIN, 0, "Green", X1 + 44.2, Y1 + 44.2, 0.05);
            }
            if (Stored > 0)
            {
                SetFont("BIGFONT");
                HudMessage("%d", Stored);
                EndHudMessage(HUDMSG_PLAIN, 0, "Yellow", X1 + 44.2, Y1 + 12.2, 0.05);
            }

            // Auto-Sell/Auto-Store Icon
            if (Player.ItemAutoMode[Player.ShopPage][Index] == AT_SELL)
                PrintSpritePulse("SELLICN", 0, X1 - 2.0 + 0.1, Y1 + 0.1, 0.75, 128.0, 0.25, false);
            else if (Player.ItemAutoMode[Player.ShopPage][Index] == AT_STORE)
                PrintSpritePulse("STORICN", 0, X1 - 2.0 + 0.1, Y1 + 0.1, 0.75, 128.0, 0.25, false);

            // Keep Icon
            if (Player.ItemKeep[Player.ShopPage][Index])
                PrintSpritePulse("KEEPICN", 0, X1 + 12.0 + 0.1, Y1 + 0.1, 0.75, 128.0, 0.25, false);

            // Mod Info
            if (CompatMode == COMPAT_DRLA && Player.LockerMode && Player.ShopPage == 0 && Player.Locker[Player.ShopPage][Index] > 0)
            {
                SetFont("SMALLFONT");
                HudMessage("%d", Player.WeaponMods[Index].Power);
                EndHudMessage(HUDMSG_PLAIN, 0, "Red", X1 + 4.0, Y1 + 38.2, 0.05);
                HudMessage("%d", Player.WeaponMods[Index].Bulk);
                EndHudMessage(HUDMSG_PLAIN, 0, "Blue", X1 + 12.0, Y1 + 38.2, 0.05);
                HudMessage("%d", Player.WeaponMods[Index].Agility);
                EndHudMessage(HUDMSG_PLAIN, 0, "Green", X1 + 20.0, Y1 + 38.2, 0.05);
                HudMessage("%d", Player.WeaponMods[Index].Technical);
                EndHudMessage(HUDMSG_PLAIN, 0, "Yellow", X1 + 28.0, Y1 + 38.2, 0.05);
                HudMessage("%d", Player.WeaponMods[Index].Sniper);
                EndHudMessage(HUDMSG_PLAIN, 0, "Purple", X1 + 4.0, Y1 + 46.2, 0.05);
                HudMessage("%d", Player.WeaponMods[Index].Firestorm);
                EndHudMessage(HUDMSG_PLAIN, 0, "Orange", X1 + 12.0, Y1 + 46.2, 0.05);
                HudMessage("%d", Player.WeaponMods[Index].Nano);
                EndHudMessage(HUDMSG_PLAIN, 0, "White", X1 + 20.0, Y1 + 46.2, 0.05);
                HudMessage("%d", Player.WeaponMods[Index].Artifacts);
                EndHudMessage(HUDMSG_PLAIN, 0, "DarkRed", X1 + 28.0, Y1 + 46.2, 0.05);
            }

            // Icon
            SetHudClipRect(X1, Y1, 44, 44);
            if (Player.ShopIndex == Index)
            {
                if (Unlocked || Player.LockerMode)
                    PrintSpritePulse(Icon, 0, IconX, IconY, 0.75, 32.0, 0.25);
                else
                    PrintSpritePulse(Icon, 0, IconX, IconY, 0.25, 32.0, 0.25);
            }
            else
            {
                if (Unlocked || Player.LockerMode)
                    PrintSprite(Icon, 0, IconX, IconY, 0.05);
                else
                    PrintSpriteAlpha(Icon, 0, IconX, IconY, 0.05, 0.25);
            }
            SetHudClipRect(0, 0, 0, 0);

            // Box
            if (Player.ShopIndex == Index)
                if ((!CanAfford || !Unlocked || !CanBuy) && !Player.LockerMode)
                    PrintSprite("ItemBoxR", 0, BoxX, BoxY, 0.05);
                else if (Player.LockerMode)
                    PrintSprite("ItemBoxL", 0, BoxX, BoxY, 0.05);
                else
                    PrintSprite("ItemBoxH", 0, BoxX, BoxY, 0.05);
            PrintSprite("ItemBoxB", 0, BoxX, BoxY, 0.05);

            // Item Name
            if (Player.ShopIndex == Index)
            {
                SetFont("BIGFONT");
                HudMessage("%S", Name);
                EndHudMessage(HUDMSG_PLAIN, 0, ((!Player.LockerMode && (CanAfford || !CanBuy)) || Player.LockerMode ? "White" : "Red"), X + 24.1, Y + 344.1, 0.05);
            }

            // Set Description For Shield Parts
            if (Player.ShopPage == 5 && Player.ShopIndex > 0)
            {
                int ReduceIndex;
                int ShieldPartType;
                str Description = "";
                ShieldPartPtr CurrentPart;
                ShieldAccsPtr CurrentAccessory;

                // Get Current Part Shield
                if (Index <= MAX_BODIES + MAX_BATTERIES + MAX_CAPACITORS)
                {
                    // Capacitors
                    if (Index > MAX_BODIES + MAX_BATTERIES)
                    {
                        ShieldPartType = SHIELDPAGE_CAPACITOR;
                        ReduceIndex = MAX_BODIES + MAX_BATTERIES;
                    }
                    // Batteries
                    else if (Index > MAX_BODIES)
                    {
                        ShieldPartType = SHIELDPAGE_BATTERY;
                        ReduceIndex = MAX_BODIES;
                    }
                    // Bodies
                    else
                        ShieldPartType = SHIELDPAGE_BODY;

                    CurrentPart = &ShieldParts[ShieldPartType][Index - ReduceIndex - 1];
                }
                // Get Current Part Shield For Accessories
                else
                {
                    ReduceIndex = MAX_BODIES + MAX_BATTERIES + MAX_CAPACITORS;
                    CurrentAccessory = &ShieldAccessories[Index - ReduceIndex - 1];
                }

                // Get Description For Bodies, Batteries and Capacitors
                if (Player.ShopIndex == Index && Index <= MAX_BODIES + MAX_BATTERIES + MAX_CAPACITORS)
                {
                    // Name
                    Description = StrParam("\CgPart characteristics:\C-");

                    str Prepend;

                    // Capacity
                    if (CurrentPart->Capacity < 0)
                        Prepend = "\Cc";
                    else
                        Prepend = "\Cj+";
                    if (CurrentPart->Capacity != 0)
                        Description = StrParam("%S\n%S%d Capacity", Description, Prepend, CurrentPart->Capacity);

                    // Charge Rate
                    if (CurrentPart->ChargeRate < 0)
                        Prepend = "\Cc";
                    else
                        Prepend = "\Cj+";
                    if (CurrentPart->ChargeRate != 0)
                        Description = StrParam("%S\n%S%d Charge Rate/Sec", Description, Prepend, CurrentPart->ChargeRate);

                    // Delay Rate
                    if (CurrentPart->DelayRate > 0)
                        Prepend = "\Cc+";
                    else
                        Prepend = "\Cj";
                    if (CurrentPart->DelayRate != 0)
                        Description = StrParam("%S\n%S%.2k Delay", Description, Prepend, CurrentPart->DelayRate);

                }
                // Get Description For Accessories
                else if (Player.ShopIndex == Index)
                {
                    // Name
                    Description = StrParam("\CgAccessory effect:\C-");

                    // Extra Description
                    if (CurrentAccessory->Description != "")
                        Description = StrParam("%S\n%S", Description, CurrentAccessory->Description);
                }

                // View Description
                SetFont("SMALLFONT");
                HudMessage("%S", Description);
                EndHudMessage(HUDMSG_PLAIN, 0, "White", X + 24.1, Y + 363.1, 0.05);
            }

            // Increment X
            BaseX += 48.0;
        }

        // Increment Y
        BaseY += 48.0;
    }
}

void CheckShopCard()
{
    Player.ShopCard = CheckInventory("DRPGUACCard");
    if (CheckInventory("DRPGDiamondUACCard"))
        Player.ShopCard = 5;

//  Disabled this to increase the duration of opening rare items in the store
//  if (Player.ShopCard == 5 && !ItemRanksRemoved)
//      RemoveItemRanks();

    for (int i = 0; i < MAX_PLAYERS; i++)
        if (Players(i).ShopCard > GlobalShopCard)
            GlobalShopCard = Players(i).ShopCard;
}
