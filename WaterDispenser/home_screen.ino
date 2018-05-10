
//***************************************************************
//
//  Name:       homescreenPushButtonHandler
//
//  Function:   Push button handler for home screen
//
//  Inputs:     HOMEPBPressed - Push button pressed
//
//  Outputs     None
//
//  Changelog:  05/06/2018 - NVG: Created routine
//
//***************************************************************

void homescreenPushButtonHandler(uint8_t HOMEPBPressed)
{
    switch (HOMEPBPressed)
    {
        case PB_MODE:
        {
            // Set foreground app to main menu
            mainSetForegroundApp(APP_MAIN_MENU);

            // Clear LCD
            lcdClearLCD();

            // Set index to select
            mainmenuHighlightItemOnMainMenu(MAINMenuIndexSelected, MENU_NONE);

            // Print main menu
            mainmenuPrintMainMenu(MENU_SET_SPRINKLE);
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
