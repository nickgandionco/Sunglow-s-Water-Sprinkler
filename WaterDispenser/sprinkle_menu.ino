
//***********************
// ENUMERATIONS
//***********************

// Sprinkler states
typedef enum
{
   SPRINKLER_OFF,
   SPRINKLER_ON,
} etypeSPRSprinklerStates;

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
        .Hour               = 15,
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
//  Name:       sprPrintMenu
//
//  Function:   Prints sprinkle menu
//
//  Inputs:     None
//
//  Outputs     None
//
//  Changelog:  05/10/2018 - NVG: Created routine
//
//***************************************************************

void sprPrintMenu()
{
    uint8_t SPRLoopIndex;
    stypeSprinkleTimeParamters SPRSettingToDisplay;

    lcdClearLCD();

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

void sprMenuBHandler(uint8_t SPRPBPressed)
{
    switch (SPRPBPressed)
    {
        case PB_MODE:
        {

        }
        break;

        case PB_UP:
        {

        }
        break;

        case PB_DOWN:
        {

        }
        break;

        case PB_BACK:
        {

        }
        break;
    }
}