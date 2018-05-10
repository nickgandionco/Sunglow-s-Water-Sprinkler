
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
        .Minute             = 00,
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

        // Print hour with no zero supression
        if (SPRSettingToDisplay.Hour < 10)
        {
            lcdPrintInt(7, SPRLoopIndex, 0);
            lcdPrintInt(8, SPRLoopIndex, SPRSettingToDisplay.Hour);
        }
        else
        {
            lcdPrintInt(7, SPRLoopIndex, SPRSettingToDisplay.Hour);
        }

        lcdPrintCharArray(9, SPRLoopIndex, ":");

        // Print minute with no zero supression
        if (SPRSettingToDisplay.Minute < 10)
        {
            lcdPrintInt(10, SPRLoopIndex, 0);
            lcdPrintInt(11, SPRLoopIndex, SPRSettingToDisplay.Minute);
        }
        else
        {
            lcdPrintInt(10, SPRLoopIndex, SPRSettingToDisplay.Minute);
        }

        // Print duration with no zero supression
        if (SPRSettingToDisplay.SprinkleDuration < 10)
        {
            lcdPrintInt(13, SPRLoopIndex, 0);
            lcdPrintInt(14, SPRLoopIndex, SPRSettingToDisplay.SprinkleDuration);
        }
        else
        {
            lcdPrintInt(13, SPRLoopIndex, SPRSettingToDisplay.SprinkleDuration);
        }

        lcdPrintCharArray(15, SPRLoopIndex, "Mins");

    }
}