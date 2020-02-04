
//***********************
// ENUMERATIONS
//***********************

// Sprinkle menu item index
typedef enum
{
    SPR_MENU_ITEM_1 = 1,
    SPR_MENU_ITEM_2 = 2,
    SPR_MENU_ITEM_3 = 3,
} etypeSprinkleMenuItemIndex;

//***********************
// GLOBAL VARIABLES
//***********************

char SPROnOff[2][20] =
{
    "OFF",
    "ON ",
};

char SPRIdentifiers[4][2] =
{
    "0.",
    "1.",
    "2.",
    "3."
};

char SPRHour[2];
char SPRMinute[2];
char SPRDuration[2];

uint8_t SPRSelectedMenu = SPR_MENU_ITEM_1;

uint32_t SPRTimeToSprinkle;

//***************************************************************
//
//  Name:       sprInit
//
//  Function:   Sets initial parameters for sprinkle settings
//
//  Inputs:     None
//
//  Outputs     None
//
//  Changelog:  05/10/2018 - NVG: Created routine
//
//***************************************************************

void sprInit()
{
    // Initial settings for sprinkle 1
    SprinkleSetting1 =
    {
        .IsActive           = SPRINKLER_ON,
        .Hour               = 00,
        .Minute             = 00,
        .SprinkleDuration   = 5,
    };

    // Initial settings for sprinkle 2
    SprinkleSetting2 =
    {
        .IsActive           = SPRINKLER_ON,
        .Hour               = 12,
        .Minute             = 5,
        .SprinkleDuration   = 10,
    };

    // Initial settings for sprinkle 3
    SprinkleSetting3 =
    {
        .IsActive           = SPRINKLER_OFF,
        .Hour               = 18,
        .Minute             = 30,
        .SprinkleDuration   = 20,
    };
}

//***************************************************************
//
//  Name:       sprCheckSprinkle
//
//  Function:   Checks if it is time to turn on sprinkler
//
//  Inputs:     None
//
//  Outputs     None
//
//  Changelog:  05/10/2018 - NVG: Created routine
//
//***************************************************************

uint8_t sprCheckSprinkle()
{
    uint8_t SPRLoopIndex;
    stypeSprinkleTimeParamters SPRSprinklerToCheck;

    // Get current hour and minute
    uint8_t SPRCurrentHour = rtcGetRTCData(RTC_HOUR);
    uint8_t SPRCurrentMinute = rtcGetRTCData(RTC_MINUTE);

    for (SPRLoopIndex = 1; SPRLoopIndex < 4; SPRLoopIndex++)
    {
        if(SPRLoopIndex == 1)  SPRSprinklerToCheck = SprinkleSetting1;
        else if(SPRLoopIndex == 2)  SPRSprinklerToCheck = SprinkleSetting2;
        else if(SPRLoopIndex == 3)  SPRSprinklerToCheck = SprinkleSetting3;

        // Check if it is time to sprinkle
        if ((SPRCurrentHour == SPRSprinklerToCheck.Hour) &&
            (SPRCurrentMinute == SPRSprinklerToCheck.Minute) &&
            (SPRSprinklerToCheck.IsActive))
        {
            // Turn sprinkler on for set period
            sprTurnOnSprinkler(SPRSprinklerToCheck.SprinkleDuration);   
        }
    }

}

//***************************************************************
//
//  Name:       sprTurnOnSprinkler
//
//  Function:   Turns on sprinkler for a set period of time
//
//  Inputs:     SPRTime - Period to turn on sprinkler (mins)
//
//  Outputs     None
//
//  Changelog:  05/10/2018 - NVG: Created routine
//
//***************************************************************

void sprTurnOnSprinkler(uint32_t SPRTime)
{
    if (SPRStatus == SPRINKLER_OFF)
    {
        // Set flag
        SPRStatus = SPRINKLER_ON;

        // Set sprinkle time in minutes
        SPRTimeToSprinkle = ((SPRTime*1000)*60);

        // Reset timer
        SPRSprinklerTimer = 0;

        // Turn on sprinkler
        digitalWrite(PIN_SPRINKLER, HIGH);

#ifdef DEBUG_MODE
        Serial.println(SPRTimeToSprinkle);
#endif

    }
}

//***************************************************************
//
//  Name:       sprCheckIfSprinklerTimerExpire
//
//  Function:   Checks if we need to turn of sprinkler
//
//  Inputs:     None
//
//  Outputs     None
//
//  Changelog:  05/10/2018 - NVG: Created routine
//
//***************************************************************

void sprCheckIfSprinklerTimerExpire()
{

#ifdef DEBUG_MODE
    Serial.println(SPRSprinklerTimer);
#endif

    // Check if timer has exceeded set time
    if (SPRTimeToSprinkle <= SPRSprinklerTimer)
    {
        // Turn off sprinkler
        digitalWrite(PIN_SPRINKLER, LOW);

        // Reset flag
        SPRStatus = SPRINKLER_OFF;
    }
}

//***************************************************************
//
//  Name:       sprPrintMenu
//
//  Function:   Prints sprinkle menu
//
//  Inputs:     SPRItemToSelect - Item selected
//
//  Outputs     None
//
//  Changelog:  05/10/2018 - NVG: Created routine
//
//***************************************************************

void sprPrintMenu(uint8_t SPRItemToSelect)
{
    uint8_t SPRLoopIndex;
    stypeSprinkleTimeParamters SPRSettingToDisplay;

    // Clear LCD
    lcdClearLCD();

    // Print header
    lcdPrintCharArray(3, 0, "SPRINKLE TIMES");

    // Print indicator
    lcdPrintCharArray(0, SPRItemToSelect+1, "*");

    // Print all sprinkle menu
    for (SPRLoopIndex = 1; SPRLoopIndex < 4; SPRLoopIndex++)
    {
        if(SPRLoopIndex == 1)  SPRSettingToDisplay = SprinkleSetting1;
        else if(SPRLoopIndex == 2)  SPRSettingToDisplay = SprinkleSetting2;
        else if(SPRLoopIndex == 3)  SPRSettingToDisplay = SprinkleSetting3;

        // Print identifier
        lcdPrintCharArray(1, SPRLoopIndex, SPRIdentifiers[SPRLoopIndex]);

        // Print on/off status
        lcdPrintCharArray(3, SPRLoopIndex, SPROnOff[SPRSettingToDisplay.IsActive]);

        // Print hour with no zero suppression
        sprintf(SPRHour, "%02d", SPRSettingToDisplay.Hour);
        lcdPrintCharArray(7, SPRLoopIndex, SPRHour);

        lcdPrintCharArray(9, SPRLoopIndex, ":");

        // Print minute with no zero supression
        sprintf(SPRMinute, "%02d", SPRSettingToDisplay.Minute);
        lcdPrintCharArray(10, SPRLoopIndex, SPRMinute);

        // Print duration with no zero supression
        sprintf(SPRDuration, "%02d", SPRSettingToDisplay.SprinkleDuration);
        lcdPrintCharArray(13, SPRLoopIndex, SPRDuration);

        lcdPrintCharArray(15, SPRLoopIndex, "Mins");

        // Clear stray characters (TODO: Trace stray characters)
        lcdPrintCharArray(6, SPRLoopIndex, " ");
        lcdPrintCharArray(12, SPRLoopIndex, " ");
        lcdPrintCharArray(19, SPRLoopIndex, " ");

        lcdPrintCharArray(0, 3, " ");
    }
}

//***************************************************************
//
//  Name:       sprMenuBHandler
//
//  Function:   Push button handler for sprinkle menu
//
//  Inputs:     SPRPBPressed - Push button pressed
//
//  Outputs     None
//
//  Changelog:  05/10/2018 - NVG: Created routine
//
//***************************************************************

void sprMenuBHandler(etypePushButtons SPRPBPressed)
{
    switch (SPRPBPressed)
    {
        case PB_MODE:
        {

        }
        break;

        case PB_UP:
        {
            // Move highlited item up
            if (SPRSelectedMenu != SPR_MENU_ITEM_1)
            {
                // Decrement selected index
                SPRSelectedMenu--;
                
                // Highlight item
                sprHighlightItemOnMenu(SPRSelectedMenu, MENU_UP);
            }
        }
        break;

        case PB_DOWN:
        {
            

            // Move highlited item down
            if (SPRSelectedMenu != SPR_MENU_ITEM_3)
            {
                // Increment selected index
                SPRSelectedMenu++;

                // Highlight item
                sprHighlightItemOnMenu(SPRSelectedMenu, MENU_DOWN);
            }
        }
        break;

        case PB_BACK:
        {
            // Set foreground app to main menu
            mainSetForegroundApp(APP_MAIN_MENU);

            // Clear LCD
            lcdClearLCD();

            // Set index to select
            mainmenuHighlightItemOnMainMenu(MENU_ITEM_1, MENU_NONE);

            // Print main menu
            mainmenuPrintMainMenu(MENU_SET_SPRINKLE);

            // Reset selected item
            SPRSelectedMenu = SPR_MENU_ITEM_1;
        }
        break;
    }
}

//***************************************************************
//
//  Name:       sprHighlightItemOnMenu
//
//  Function:   Highlights an item on the sprinkle menu
//
//  Inputs:     SPRSelectedItem - Item to be highlighted
//              SPRDirection    - Direction of selection
//
//  Outputs     None
//
//  Changelog:  05/10/2018 - NVG: Created routine
//
//***************************************************************

void sprHighlightItemOnMenu(uint8_t SPRSelectedItem, uint8_t SPRDirection)
{
    uint8_t SPRCurrentSelection = SPRSelectedItem;
    uint8_t SPRPreviousSelection;

    if (SPRDirection != MENU_NONE)
    {
        // Find out previous selection so we can erase indicator
        if (SPRDirection == MENU_DOWN)
        {
            SPRPreviousSelection = (--SPRSelectedItem);
        }
        else if (SPRDirection == MENU_UP)
        {
            SPRPreviousSelection = (++SPRSelectedItem);
        }

        lcdPrintCharArray(0, SPRPreviousSelection, " ");
    }

    lcdPrintCharArray(0, SPRCurrentSelection, "*");
}
