
//***********************
// GLOBAL VARIABLES
//***********************

char SPRINKLEOnOff[2][3] =
{
    "OFF",
    "ON",
};

//***************************************************************
//
//  Name:       sprinkleInit
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

void sprinkleInit()
{
    // Initial settings for sprinkle 1
    SprinkleSetting1 =
    {
        .IsActive           = SPRINKLER_ON,
        .Hour               = 15,
        .Minute             = 30,
        .SprinkleDuration   = 5,
    };

    // Initial settings for sprinkle 2
    SprinkleSetting1 =
    {
        .IsActive           = SPRINKLER_ON,
        .Hour               = 12,
        .Minute             = 00,
        .SprinkleDuration   = 10,
    };

    // Initial settings for sprinkle 3
    SprinkleSetting1 =
    {
        .IsActive           = SPRINKLER_OFF,
        .Hour               = 18,
        .Minute             = 30,
        .SprinkleDuration   = 20,
    };
}

//***************************************************************
//
//  Name:       sprinklePrintMenu
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

void sprinklePrintMenu()
{

}