//***********************
// DEFINITIONS
//***********************

#define SET_MAX_HOUR        23
#define SET_MAX_MINUTE      55
#define SET_MAX_DURATIOTN   60

#define SET_MIN_HOUR_MIN    0
#define SET_MIN_DURATIOTN   5

//***********************
// GLOBAL VARIABLES
//***********************

uint8_t SETParamSelected = SPRINKLER_ACTIVE;

etypeSprinkleMenuItemIndex SETSprinkleToSet;

//***************************************************************
//
//  Name:       setSprinklerToSet
//
//  Function:   Sets which sprinkler "object" to set
//
//  Inputs:     SETSprinklerSelected - Sprinkler to set
//
//  Outputs     None
//
//  Changelog:  02/05/2020 - NVG: Created routine
//
//***************************************************************

void setSprinklerToSet(etypeSprinkleMenuItemIndex SETSprinklerSelected)
{
    SETSprinkleToSet = SETSprinklerSelected;
}

//***************************************************************
//
//  Name:       setPrintMenu
//
//  Function:   Prints header menu
//
//  Inputs:     SETMenuToDisplay - Header menu to print
//
//  Outputs     None
//
//  Changelog:  02/05/2020 - NVG: Created routine
//
//***************************************************************

void setPrintMenu(etypeSETSprinklerParams SETMenuToDisplay)
{
    switch (SETMenuToDisplay)
    {
        case SPRINKLER_ACTIVE:
            lcdPrintCharArray(3, 0, "TOGGLE ON/OFF");
        break;

        case SPRINKLER_HOUR:
            lcdPrintCharArray(6, 0, "SET HOUR");
        break;

        case SPRINKLER_MINUTE:
            lcdPrintCharArray(5, 0, "SET MINUTE");
        break;

        case SPRINKLER_DURATION:
            lcdPrintCharArray(4, 0, "SET DURATION");
        break;
    }
}

//***************************************************************
//
//  Name:       setDisplayParameter
//
//  Function:   Displays parameter. This is used on application entry
//
//  Inputs:     SETMenuToDisplay - Parameter to display
//
//  Outputs     None
//
//  Changelog:  02/05/2020 - NVG: Created routine
//
//***************************************************************

void setDisplayParameter(etypeSETSprinklerParams SETMenuToDisplay)
{
    stypeSprinkleTimeParamters SETSprinklerItem;

    if (SETSprinkleToSet == SPR_MENU_ITEM_1)       SETSprinklerItem = SprinkleSetting1;
    else if (SETSprinkleToSet == SPR_MENU_ITEM_2)  SETSprinklerItem = SprinkleSetting2;
    else if (SETSprinkleToSet == SPR_MENU_ITEM_3)  SETSprinklerItem = SprinkleSetting3;

    switch(SETMenuToDisplay)
    {
        case SPRINKLER_ACTIVE:
            lcdPrintCharArray(9, 3, SETTINGSOnOff[SETSprinklerItem.IsActive]);
        break;

        case SPRINKLER_HOUR:
            lcdPrintInt(9, 3, SETSprinklerItem.Hour);
        break;

        case SPRINKLER_MINUTE:
            lcdPrintInt(9, 3, SETSprinklerItem.Minute);
        break;

        case SPRINKLER_DURATION:
            lcdPrintInt(9, 3, SETSprinklerItem.SprinkleDuration);
        break;
    }
}

//***************************************************************
//
//  Name:       setIncrementAndUpdateSetting
//
//  Function:   Increment paramter specified
//
//  Inputs:     SETParamSelected - Parameter to increment
//
//  Outputs     None
//
//  Changelog:  02/05/2020 - NVG: Created routine
//
//***************************************************************

void setIncrementAndUpdateSetting(uint8_t SETParamSelected)
{
    stypeSprinkleTimeParamters *SETSprinklerItem;

    if (SETSprinkleToSet == SPR_MENU_ITEM_1)       SETSprinklerItem = &SprinkleSetting1;
    else if (SETSprinkleToSet == SPR_MENU_ITEM_2)  SETSprinklerItem = &SprinkleSetting2;
    else if (SETSprinkleToSet == SPR_MENU_ITEM_3)  SETSprinklerItem = &SprinkleSetting3;

    switch (SETParamSelected)
    {
        case SPRINKLER_ACTIVE:
        {
            // Clear display
            lcdPrintCharArray(9, 3, "   ");

            // Toggle on/off and update display
            SETSprinklerItem->IsActive ^= 1;
            lcdPrintCharArray(9, 3, SETTINGSOnOff[SETSprinklerItem->IsActive]);
        }
        break;

        case SPRINKLER_HOUR:
        {
            // Clear display
            lcdPrintCharArray(9, 3, "   ");

            // Make sure hour setting is always
            // witihin range
            if (SETSprinklerItem->Hour < SET_MAX_HOUR)
                SETSprinklerItem->Hour++;
            else
                SETSprinklerItem->Hour = SET_MIN_HOUR_MIN;

            // Update display
            lcdPrintInt(9, 3, SETSprinklerItem->Hour);
        }
        break;

        case SPRINKLER_MINUTE:
        {
            // Clear display
            lcdPrintCharArray(9, 3, "   ");

            // Make sure minute setting is always
            // witihin range
            if (SETSprinklerItem->Minute < SET_MAX_MINUTE)
                SETSprinklerItem->Minute += 5;
            else
                SETSprinklerItem->Minute = SET_MIN_HOUR_MIN;

            // Update display
            lcdPrintInt(9, 3, SETSprinklerItem->Minute);
        }
        break;

        case SPRINKLER_DURATION:
        {
            // Clear display
            lcdPrintCharArray(9, 3, "   ");

            // Make sure duration setting is always
            // witihin range
            if (SETSprinklerItem->SprinkleDuration < SET_MAX_DURATIOTN)
                SETSprinklerItem->SprinkleDuration += 5;
            else
                SETSprinklerItem->SprinkleDuration = SET_MIN_DURATIOTN;

            // Update display
            lcdPrintInt(9, 3, SETSprinklerItem->SprinkleDuration);
        }
        break;
    }
}

//***************************************************************
//
//  Name:       setDecrementAndUpdateSetting
//
//  Function:   Decrement paramter specified
//
//  Inputs:     SETParamSelected - Parameter to decrement
//
//  Outputs     None
//
//  Changelog:  02/05/2020 - NVG: Created routine
//
//***************************************************************

void setDecrementAndUpdateSetting(uint8_t SETParamSelected)
{
    stypeSprinkleTimeParamters *SETSprinklerItem;

    if (SETSprinkleToSet == SPR_MENU_ITEM_1)       SETSprinklerItem = &SprinkleSetting1;
    else if (SETSprinkleToSet == SPR_MENU_ITEM_2)  SETSprinklerItem = &SprinkleSetting2;
    else if (SETSprinkleToSet == SPR_MENU_ITEM_3)  SETSprinklerItem = &SprinkleSetting3;

    switch (SETParamSelected)
    {
        case SPRINKLER_ACTIVE:
        {
            // Clear display
            lcdPrintCharArray(9, 3, "   ");

            // Toggle on/off and update display
            SETSprinklerItem->IsActive ^= 1;
            lcdPrintCharArray(9, 3, SETTINGSOnOff[SETSprinklerItem->IsActive]);
        }
        break;

        case SPRINKLER_HOUR:
        {
            // Clear display
            lcdPrintCharArray(9, 3, "   ");

            // Make sure hour setting is always
            // witihin range
            if (SETSprinklerItem->Hour > SET_MIN_HOUR_MIN)
                SETSprinklerItem->Hour--;
            else
                SETSprinklerItem->Hour = SET_MAX_HOUR;

            // Update display
            lcdPrintInt(9, 3, SETSprinklerItem->Hour);
        }
        break;

        case SPRINKLER_MINUTE:
        {
            // Clear display
            lcdPrintCharArray(9, 3, "   ");

            // Make sure minute setting is always
            // witihin range
            if (SETSprinklerItem->Minute > SET_MIN_HOUR_MIN)
                SETSprinklerItem->Minute -= 5;
            else
                SETSprinklerItem->Minute = SET_MAX_MINUTE;

            // Update display
            lcdPrintInt(9, 3, SETSprinklerItem->Minute);
        }
        break;

        case SPRINKLER_DURATION:
        {
            // Clear display
            lcdPrintCharArray(9, 3, "   ");

            // Make sure duration setting is always
            // witihin range
            if (SETSprinklerItem->SprinkleDuration > SET_MIN_DURATIOTN)
                SETSprinklerItem->SprinkleDuration -= 5;
            else
                SETSprinklerItem->SprinkleDuration = SET_MAX_DURATIOTN;

            // Update display
            lcdPrintInt(9, 3, SETSprinklerItem->SprinkleDuration);
        }
        break;
    }
}

//***************************************************************
//
//  Name:       setMenuBHandler
//
//  Function:   Push button handler for sprinkle menu
//
//  Inputs:     SETPBPressed - Push button pressed
//
//  Outputs     None
//
//  Changelog:  02/04/2020 - NVG: Created routine
//
//***************************************************************

void setMenuBHandler(etypePushButtons SETPBPressed)
{
    switch (SETPBPressed)
    {
        case PB_MODE:
        {
            if (SETParamSelected != SPRINKLER_DURATION)
            {
                // Increment parameter to be set
                SETParamSelected++;

                // Clear LCD
                lcdClearLCD();

                // Print menu
                setPrintMenu(SETParamSelected);

                // Display parameter
                setDisplayParameter(SETParamSelected);
            }
            else
            {
                // Clear LCD
                lcdClearLCD();

                // Set sprinkle app to be foreground app
                mainSetForegroundApp(APP_SPRINKLE);

                // Print Sprinkle menu
                sprPrintMenu(MENU_ITEM_1);

                // We reached the end. Reset.`
                SETParamSelected = SPRINKLER_ACTIVE;
            }
        }
        break;

        case PB_UP:
        {
            // Decrement and update setting
            setDecrementAndUpdateSetting(SETParamSelected);
            
        }
        break;

        case PB_DOWN:
        {
            // Increment and update setting
            setIncrementAndUpdateSetting(SETParamSelected);
        }
        break;

        case PB_BACK:
        {
            // Clear LCD
            lcdClearLCD();

            // Set sprinkle app to be foreground app
            mainSetForegroundApp(APP_SPRINKLE);

            // Print Sprinkle menu
            sprPrintMenu(MENU_ITEM_1);

            SETParamSelected = SPRINKLER_ACTIVE;
        }
        break;
    }
}