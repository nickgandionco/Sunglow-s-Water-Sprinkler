
//***********************
// INCLUDES
//***********************

#include <RTClib.h>
#include <elapsedMillis.h>

//***********************
// DEFINITIONS
//***********************

#define PIN_MEGA_SDA        20
#define PIN_MEGA_SCL        21

#define PB_MODE_PIN        2
#define PB_UP_PIN          3
#define PB_DOWN_PIN        18
#define PB_SELECT_PIN      19

#define BACKLIGHT_TIMEOUT  5000

#define DEBOUNCE_TIME      500 

//***********************
// ENUMERATIONS
//***********************

// Push buttons
typedef enum
{
    PB_MODE,
    PB_UP,
    PB_DOWN,
    PB_SELECT,    
} etypePushButtons;

// Push button states
typedef enum
{
    PB_RELEASED,
    PB_PRESSED
} etypeButtonStates;

// Main menu items
typedef enum
{
    MENU_SET_SPRINKLE,
    MENU_SET_TIME,
    MENU_MANUAL_MODE,
    MENU_CALIBRATE_METER,
    MENU_SETTINGS,
} etypeMainMenuItems;

// Main menu item index
typedef enum
{
    MENU_ITEM_1,
    MENU_ITEM_2,
    MENU_ITEM_3,
    MENU_ITEM_4,
} etypeMainMenuItemIndex;

typedef enum
{
    MENU_NONE,
    MENU_DOWN,
    MENU_UP
} etypeMenuSelectionDirection;

// Settings items
typedef enum
{
    SETTINGS_BUZZER,
    SETTINGS_BACKLIGHT,
};

// Applications
typedef enum
{
    APP_HOMESCREEN,
    APP_MAIN_MENU,
    APP_CUSTOM,
} etypeApplications;

// LCD backlight states
typedef enum
{
   LCD_BACKLIGHT_OFF,
   LCD_BACKLIGHT_ON
} etypeLCDBacklightStates;

//***********************
// GLOBAL VARIABLES
//***********************

char RTCDaysOfTheWeek[7][12] = {"Sun", "Mon", "Tue", "Wed", "Thu", "Fri", "Sat"};
char MENUItems[5][19] =
{
    "Set sprinkle",
    "Set time",
    "Manual mode",
    "Calibrate meter",
    "Settings",
};

char SETTINGSItems[2][19] =
{
    "Buzzer",
    "Backlight",
};

RTC_DS1307      rtc;
DateTime        RTCCurrentDateTime;
elapsedMillis   TIMERBacklight;
elapsedMillis   TIMERDebounce;

uint8_t MAINForegroundApp;
uint8_t MAINMenuIndexSelected       = MENU_ITEM_1;

volatile byte MAINPBPress           = PB_RELEASED;
volatile byte MAINBacklightStatus   = LCD_BACKLIGHT_ON;
volatile uint8_t MAINButtonPressed;


//***************************************************************
//
//  Name:       setup
//
//  Function:   Routine to be run once during startup
//
//  Inputs:     None
//
//  Outputs     None
//
//  Changelog:  05/03/2018 - NVG: Created routine
//
//***************************************************************

void setup()
{
    // Initialize LCD
    lcdInit();

    // Initialize real time clock
    rtcInitializeRTC();

    // Initialize push buttons and interrupt
    mainInitializePBAndInterrupts();

    // Set foreground app to homescreen on bootup
    MAINForegroundApp = APP_HOMESCREEN;
}

//***************************************************************
//
//  Name:       loop
//
//  Function:   Routine to be run on a loop after setup is run
//
//  Inputs:     None
//
//  Outputs     None
//
//  Changelog:  04/06/2018 - NVG: Created routine
//
//***************************************************************

void loop()
{
    if (APP_HOMESCREEN == MAINForegroundApp)
    {
        // Print current date and time
        lcdPrintCharArrayDateAndTime();
    }

    // Wait for push button for backlight handling
    mainPBtHandlerForBacklight();
}

//***************************************************************
//
//  Name:       mainInitializePBAndInterrupts
//
//  Function:   Initializes push buttons and interrupt
//
//  Inputs:     None
//
//  Outputs     None
//
//  Changelog:  05/05/2018 - NVG: Created routine
//
//***************************************************************

void mainInitializePBAndInterrupts()
{
    // Initialize push buttons
    pinMode(PB_MODE_PIN, INPUT_PULLUP);
    pinMode(PB_UP_PIN, INPUT_PULLUP);
    pinMode(PB_DOWN_PIN, INPUT_PULLUP);
    pinMode(PB_SELECT_PIN, INPUT_PULLUP);

    // Initialize all interrupt
    attachInterrupt(digitalPinToInterrupt(PB_MODE_PIN), mainModeDepression, FALLING);
    attachInterrupt(digitalPinToInterrupt(PB_UP_PIN), mainUpDepression, FALLING);
    attachInterrupt(digitalPinToInterrupt(PB_DOWN_PIN), mainDownDepression, FALLING);
    attachInterrupt(digitalPinToInterrupt(PB_SELECT_PIN), mainSelectDepression, FALLING);
}

//***************************************************************
//
//  Name:       rtcInitializeRTC
//
//  Function:   Initializes and sets real time clock
//
//  Inputs:     None
//
//  Outputs     None
//
//  Changelog:  05/05/2018 - NVG: Created routine
//
//***************************************************************

void rtcInitializeRTC()
{
    // Auto update from computer time
    rtc.adjust(DateTime(F(__DATE__), F(__TIME__)));
}

//***************************************************************
//
//  Name:       mainPBtHandlerForBacklight
//
//  Function:   Push button handler for backlight tasks
//
//  Inputs:     None
//
//  Outputs     None
//
//  Changelog:  05/05/2018 - NVG: Created routine
//
//***************************************************************

void mainPBtHandlerForBacklight()
{
    // Check if 5 seconds has elapsed
    if ((LCD_BACKLIGHT_ON == MAINBacklightStatus) && (BACKLIGHT_TIMEOUT < TIMERBacklight))
    {
        // Backlight flag is set to OFF
        MAINBacklightStatus = LCD_BACKLIGHT_OFF;
        
        // Reset timer
        TIMERBacklight = 0;
    }

    // If there is a push button depression
    if ((PB_PRESSED == MAINPBPress) && (DEBOUNCE_TIME <= TIMERDebounce))
    {
        // Reset push button depression tracker
        MAINPBPress = PB_RELEASED;

        // Set flag to indicate that backlight should be OFF
        MAINBacklightStatus = LCD_BACKLIGHT_ON;

        // Reset timer
        TIMERBacklight = 0;

        // Reset debounce timer
        TIMERDebounce = 0;

        mainPushButtonHandler();
    }

    // Turn LCD backlight on/off
    lcdSetBacklight(MAINBacklightStatus);
}

//***************************************************************
//
//  Name:       rtcPrintHour
//
//  Function:   Prints hour on the LCD
//
//  Inputs:     RTCYCoordinate - Y coordinate to print
//              RTCXCoordinate - X coordinate to print
//
//  Outputs     None
//
//  Changelog:  05/03/2018 - NVG: Created routine
//
//***************************************************************

void rtcPrintHour(uint8_t RTCYCoordinate, uint8_t RTCXCoordinate)
{
    uint8_t RTCHour = RTCCurrentDateTime.hour();

    // Manually add zero if single digits
    if (RTCHour < 10)
    {
       lcdPrintCharArray(RTCYCoordinate, RTCXCoordinate, "0");
    }

    lcdPrintCharArrayInt(RTCYCoordinate, RTCXCoordinate, RTCHour);
}

//***************************************************************
//
//  Name:       rtcPrintMinute
//
//  Function:   Prints minute on the LCD
//
//  Inputs:     RTCYCoordinate - Y coordinate to print
//              RTCXCoordinate - X coordinate to print
//
//  Outputs     None
//
//  Changelog:  05/03/2018 - NVG: Created routine
//
//***************************************************************

void rtcPrintMinute(uint8_t RTCYCoordinate, uint8_t RTCXCoordinate)
{
    uint8_t RTCMinute = RTCCurrentDateTime.minute();
   
    // Manually add zero if single digits
    if (RTCMinute < 10)
    {
       lcdPrintCharArray(RTCYCoordinate, RTCXCoordinate, "0");
    }

    lcdPrintCharArrayInt(RTCYCoordinate, RTCXCoordinate, RTCMinute);
}

//***************************************************************
//
//  Name:       rtcPrintSecond
//
//  Function:   Prints second on the LCD
//
//  Inputs:     RTCYCoordinate - Y coordinate to print
//              RTCXCoordinate - X coordinate to print
//
//  Outputs     None
//
//  Changelog:  05/03/2018 - NVG: Created routine
//
//***************************************************************

void rtcPrintSecond(uint8_t RTCYCoordinate, uint8_t RTCXCoordinate)
{
    uint8_t RTCSecond = RTCCurrentDateTime.second();

    // Manually add zero if single digits
    if (RTCSecond < 10)
    {
       lcdPrintCharArray(RTCYCoordinate, RTCXCoordinate, "0");
    }

    lcdPrintCharArrayInt(RTCYCoordinate, RTCXCoordinate, RTCSecond);
}

//***************************************************************
//
//  Name:       mainModeDepression
//
//  Function:   Mode push button depression interrupt handler
//
//  Inputs:     None
//
//  Outputs     None
//
//  Changelog:  05/05/2018 - NVG: Created routine
//
//***************************************************************

void mainModeDepression()
{
    // Indicate that there is a button depression
    MAINPBPress = PB_PRESSED;

    // Indicate that MODE button is pressed
    MAINButtonPressed = PB_MODE;
}

//***************************************************************
//
//  Name:       mainUpDepression
//
//  Function:   Up push button depression interrupt handler
//
//  Inputs:     None
//
//  Outputs     None
//
//  Changelog:  05/05/2018 - NVG: Created routine
//
//***************************************************************

void mainUpDepression()
{
    // Indicate that there is a button depression
    MAINPBPress = PB_PRESSED;

    // Indicate that UP button is pressed
    MAINButtonPressed = PB_UP;
}

//***************************************************************
//
//  Name:       mainDownDepression
//
//  Function:   Down push button depresison interrupt handler
//
//  Inputs:     None
//
//  Outputs     None
//
//  Changelog:  05/05/2018 - NVG: Created routine
//
//***************************************************************

void mainDownDepression()
{
    // Indicate that there is a button depression
    MAINPBPress = PB_PRESSED;

    // Indicate that DOWN button is pressed
    MAINButtonPressed = PB_DOWN;
}

//***************************************************************
//
//  Name:       mainSelectDepression
//
//  Function:   Select push button depression interrupt handler
//
//  Inputs:     None
//
//  Outputs     None
//
//  Changelog:  05/05/2018 - NVG: Created routine
//
//***************************************************************

void mainSelectDepression()
{
    // Indicate that there is a button depression
    MAINPBPress = PB_PRESSED;

    // Indicate that SELECT button is pressed
    MAINButtonPressed = PB_SELECT;
}

//***************************************************************
//
//  Name:       mainPushButtonHandler
//
//  Function:   Handles push button depression events
//
//  Inputs:     None
//
//  Outputs     None
//
//  Changelog:  05/06/2018 - NVG: Created routine
//
//***************************************************************

void mainPushButtonHandler()
{
    switch (MAINForegroundApp)
    {
        case APP_HOMESCREEN:
        {
            switch (MAINButtonPressed)
            {
                case PB_MODE:
                {
                    // Set foreground app to main menu
                    MAINForegroundApp = APP_MAIN_MENU;

                    // Clear LCD
                    lcdClearLCD();

                    // Set index to select
                    lcdSelectItemOnMainMenu(MAINMenuIndexSelected, MENU_NONE);

                    // Print main menu
                    lcdPrintCharArrayMainMenu(MENU_SET_SPRINKLE);
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

                case PB_SELECT:
                {

                }
                break;
            }            
        }
        break;

        case APP_MAIN_MENU:
        {
            switch (MAINButtonPressed)
            {
                case PB_MODE:
                {
                    MAINForegroundApp = APP_HOMESCREEN;

                    lcdClearLCD();
                }
                break;

                case PB_UP:
                {
                    if (MAINMenuIndexSelected != MENU_ITEM_1)
                    {
                        // Increment menu index to hightlight
                        MAINMenuIndexSelected--;

                        // Set index to select
                        lcdSelectItemOnMainMenu(MAINMenuIndexSelected, MENU_UP);
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
                        lcdSelectItemOnMainMenu(MAINMenuIndexSelected, MENU_DOWN);
                    }
                }
                break;

                case PB_SELECT:
                {

                }
                break;
            }
        }
        break;
    }

    // Reset button depression indicator
    MAINButtonPressed = 0;
}

//***************************************************************
//
//  Name:       lcdPrintCharArrayMainMenu
//
//  Function:   Prints main menu
//
//  Inputs:     LCDTopMenu - Index of top menu item
//
//  Outputs     None
//
//  Changelog:  05/07/2018 - NVG: Created routine
//
//***************************************************************

void lcdPrintCharArrayMainMenu(uint8_t LCDTopMenu)
{
    uint8_t LCDMenuToDisplay = LCDTopMenu;
    uint8_t LCDLoopIndex;

    // Print four menu items
    for (LCDLoopIndex = 0; LCDLoopIndex < 4; LCDLoopIndex++)
    {
        lcdPrintCharArray(1, LCDMenuToDisplay, MENUItems[LCDMenuToDisplay]);
        
        // Increment menu index
        LCDMenuToDisplay++;
    }
}

//***************************************************************
//
//  Name:       lcdSelectItemOnMainMenu
//
//  Function:   Selects an item in main menu
//
//  Inputs:     LCDSelectedItem  - Index of selected item
//              LCDMenuDirection - Direction of menu (up/down)
//
//  Outputs     None
//
//  Changelog:  05/07/2018 - NVG: Created routine
//
//***************************************************************

void lcdSelectItemOnMainMenu(uint8_t LCDSelectedItem, uint8_t LCDMenuDirection)
{
    uint8_t LCDCurrentSelection = LCDSelectedItem;
    uint8_t LCDPreviousSelection;

    if (LCDMenuDirection != MENU_NONE)
    {
        // Find out previous selection so we can erase indicator
        if (LCDMenuDirection == MENU_DOWN)
        {
            LCDPreviousSelection = (--LCDSelectedItem);
        }
        else if (LCDMenuDirection == MENU_UP)
        {
            LCDPreviousSelection = (++LCDSelectedItem);
        }

        lcdPrintCharArray(0, LCDPreviousSelection, " ");
    }

    lcdPrintCharArray(0, LCDCurrentSelection, "*");
}
