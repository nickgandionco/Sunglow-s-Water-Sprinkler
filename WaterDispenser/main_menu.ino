
//***********************
// GLOBAL VARIABLES
//***********************

char MENUItems[5][19] =
{
    "Set sprinkle",
    "Set time",
    "Manual mode",
    "Calibrate meter",
    "Settings",
};

//***************************************************************
//
//  Name:       mainmenuPushButtonHandler
//
//  Function:   Handles push button events when in main menu
//
//  Inputs:     MAINPBPressed - Button that is pressed
//
//  Outputs     None
//
//  Changelog:  05/07/2018 - NVG: Created routine
//
//***************************************************************

void mainmenuPushButtonHandler(etypePushButtons MAINPBPressed)
{
    switch (MAINPBPressed)
    {
        case PB_MODE:
        {
            if (MAINMenuIndexSelected ==  MENU_SET_SPRINKLE)
            {
                // Clear LCD
                lcdClearLCD();

                // Set sprinkle app to be foreground app
                mainSetForegroundApp(APP_SPRINKLE);

                // Print Sprinkle menu
                sprPrintMenu(MENU_ITEM_1);
            }
            else if (MAINMenuIndexSelected == MENU_MANUAL_MODE)
            {
                // Turn sprinkler on for 1 minute
                sprTurnOnSprinkler(1);
            }
        }
        break;

        case PB_UP:
        {
            if (MAINMenuIndexSelected != MENU_ITEM_1)
            {
                // Increment menu index to hightlight
                MAINMenuIndexSelected--;

                // Set index to select
                mainmenuHighlightItemOnMainMenu(MAINMenuIndexSelected, MENU_UP);
            }
        }
        break;

        case PB_DOWN:
        {
            if (MAINMenuIndexSelected != MENU_ITEM_4)
            {
                // Increment menu index to hightlight
                MAINMenuIndexSelected++;

                // Set index to select
                mainmenuHighlightItemOnMainMenu(MAINMenuIndexSelected, MENU_DOWN);
            }
        }
        break;

        case PB_BACK:
        {
            // Clear LCD
            lcdClearLCD();

            // Make homescreen foreground application
            mainSetForegroundApp(APP_HOMESCREEN);

            homescreenPrintHomeScreen();

            // Set first item to be highlighted
            MAINMenuIndexSelected = MENU_ITEM_1;
        }
        break;
    }
}


//***************************************************************
//
//  Name:       mainmenuPrintMainMenu
//
//  Function:   Prints main menu
//
//  Inputs:     MAINMENUTopMenu - Index of top menu item
//
//  Outputs     None
//
//  Changelog:  05/07/2018 - NVG: Created routine
//
//***************************************************************

void mainmenuPrintMainMenu(uint8_t MAINMENUTopMenu)
{
    uint8_t MAINMENUMenuToDisplay = MAINMENUTopMenu;
    uint8_t MAINMENULoopIndex;

    // Print four menu items
    for (MAINMENULoopIndex = 0; MAINMENULoopIndex < 4; MAINMENULoopIndex++)
    {
        lcdPrintCharArray(1, MAINMENUMenuToDisplay, MENUItems[MAINMENUMenuToDisplay]);
        
        // Increment menu index
        MAINMENUMenuToDisplay++;
    }
}

//***************************************************************
//
//  Name:       mainmenuHighlightItemOnMainMenu
//
//  Function:   Selects an item in main menu
//
//  Inputs:     MAINMENUSelectedItem  - Index of selected item
//              MAINMENUMenuDirection - Direction of menu (up/down)
//
//  Outputs     None
//
//  Changelog:  05/07/2018 - NVG: Created routine
//
//***************************************************************

void mainmenuHighlightItemOnMainMenu(uint8_t MAINMENUSelectedItem, etypeMenuSelectionDirection MAINMENUMenuDirection)
{
    uint8_t MAINMENUCurrentSelection = MAINMENUSelectedItem;
    uint8_t MAINMENUPreviousSelection;

    if (MAINMENUMenuDirection != MENU_NONE)
    {
        // Find out previous selection so we can erase indicator
        if (MAINMENUMenuDirection == MENU_DOWN)
        {
            MAINMENUPreviousSelection = (--MAINMENUSelectedItem);
        }
        else if (MAINMENUMenuDirection == MENU_UP)
        {
            MAINMENUPreviousSelection = (++MAINMENUSelectedItem);
        }

        lcdPrintCharArray(0, MAINMENUPreviousSelection, " ");
    }

    lcdPrintCharArray(0, MAINMENUCurrentSelection, "*");
}
