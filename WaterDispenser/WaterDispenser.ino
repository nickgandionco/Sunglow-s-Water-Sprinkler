
//***********************
// INCLUDES
//***********************

#include <elapsedMillis.h>

//***********************
// DEFINITIONS
//***********************

#define PIN_MEGA_SDA        20
#define PIN_MEGA_SCL        21

#define PB_MODE_PIN         2
#define PB_UP_PIN           3
#define PB_DOWN_PIN         18
#define PB_BACK_PIN         19

#define PIN_SPRINKLER       4
#define PIN_BUZZER          5

#define BACKLIGHT_TIMEOUT   60000

#define DEBOUNCE_TIME       500

#define DEBUG_MODE

//***********************
// STRUCTURES
//***********************

// Sprinkle time parameters
typedef struct
{
    bool IsActive;
    uint8_t Hour;
    uint8_t Minute;
    uint8_t SprinkleDuration;  
} stypeSprinkleTimeParamters;

//***********************
// ENUMERATIONS
//***********************

// Push buttons
typedef enum
{
    PB_MODE,
    PB_UP,
    PB_DOWN,
    PB_BACK,    
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

// Menu directions
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
    APP_SPRINKLE,
    APP_CUSTOM,
} etypeApplications;

// LCD backlight states
typedef enum
{
   LCD_BACKLIGHT_OFF,
   LCD_BACKLIGHT_ON
} etypeLCDBacklightStates;

// Sprinkler states
typedef enum
{
   SPRINKLER_OFF,
   SPRINKLER_ON,
} etypeSPRSprinklerStates;

//***********************
// GLOBAL VARIABLES
//***********************

char SETTINGSItems[2][19] =
{
    "Buzzer",
    "Backlight",
};

elapsedMillis   TIMERBacklight;
elapsedMillis   TIMERDebounce;
elapsedMillis   SPRSprinklerTimer;

uint8_t MAINForegroundApp;
uint8_t MAINMenuIndexSelected       = MENU_ITEM_1;

stypeSprinkleTimeParamters SprinkleSetting1;
stypeSprinkleTimeParamters SprinkleSetting2;
stypeSprinkleTimeParamters SprinkleSetting3;

volatile byte MAINPBPress           = PB_RELEASED;
volatile byte MAINBacklightStatus   = LCD_BACKLIGHT_ON;
volatile uint8_t MAINButtonPressed;

bool SPRStatus = SPRINKLER_OFF;

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
#ifdef DEBUG_MODE
    Serial.begin(9600);
#endif
    // Initialize LCD
    lcdInit();

    // Initialize real time clock
    rtcInit();

    // Initialize push buttons and interrupt
    mainInitializePBAndInterrupts();

    // Set foreground app to homescreen on bootup
    MAINForegroundApp = APP_HOMESCREEN;

    // Initialize sprinkler settings
    sprInit();

    // Print home screen
    homescreenPrintHomeScreen();
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
        rtcPrintDateAndTime();
    }

    // Wait for push button for backlight handling
    mainPBtHandlerForBacklight();

    // Check and turn on if it is time to sprinkle
    sprCheckSprinkle();

    // If sprinkler is on, check if we need to turn it off
    if (SPRStatus == SPRINKLER_ON)
    {
        sprCheckIfSprinklerTimerExpire();
    }

    Serial.println(digitalRead(PIN_SPRINKLER));
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
    pinMode(PB_BACK_PIN, INPUT_PULLUP);

    // Initialize sprinkler pin as output
    pinMode(PIN_SPRINKLER, OUTPUT);

    // Initialize buzzer pin as output
    pinMode(PIN_BUZZER, OUTPUT);

    // Initialize all interrupt
    attachInterrupt(digitalPinToInterrupt(PB_MODE_PIN), mainModeDepression, FALLING);
    attachInterrupt(digitalPinToInterrupt(PB_UP_PIN), mainUpDepression, FALLING);
    attachInterrupt(digitalPinToInterrupt(PB_DOWN_PIN), mainDownDepression, FALLING);
    attachInterrupt(digitalPinToInterrupt(PB_BACK_PIN), mainSelectDepression, FALLING);
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
        // Set flag to indicate that backlight should be OFF
        MAINBacklightStatus = LCD_BACKLIGHT_ON;

        // Reset timer
        TIMERBacklight = 0;

        // Reset debounce timer
        TIMERDebounce = 0;

        // Reset push button depression tracker
        MAINPBPress = PB_RELEASED;

        mainPushButtonHandler();
    }

    // Turn LCD backlight on/off
    lcdSetBacklight(MAINBacklightStatus);
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
    if (DEBOUNCE_TIME <= TIMERDebounce)
    {
        // Indicate that there is a button depression
        MAINPBPress = PB_PRESSED;

        // Indicate that MODE button is pressed
        MAINButtonPressed = PB_MODE;
    }
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
    if (DEBOUNCE_TIME <= TIMERDebounce)
    {
        // Indicate that there is a button depression
        MAINPBPress = PB_PRESSED;

        // Indicate that UP button is pressed
        MAINButtonPressed = PB_UP;
    }
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
    if (DEBOUNCE_TIME <= TIMERDebounce)
    {
        // Indicate that there is a button depression
        MAINPBPress = PB_PRESSED;

        // Indicate that DOWN button is pressed
        MAINButtonPressed = PB_DOWN;
    }
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
    if (DEBOUNCE_TIME <= TIMERDebounce)
    {
        // Indicate that there is a button depression
        MAINPBPress = PB_PRESSED;

        // Indicate that SELECT button is pressed
        MAINButtonPressed = PB_BACK;
    }
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
            homescreenPushButtonHandler(MAINButtonPressed);
        }
        break;

        case APP_MAIN_MENU:
        {
            mainmenuPushButtonHandler(MAINButtonPressed);
        }
        break;

        case APP_SPRINKLE:
        {
            sprMenuBHandler(MAINButtonPressed);
        }
        break;
    }

    // Reset button depression indicator
    MAINButtonPressed = 0;
}

//***************************************************************
//
//  Name:       mainSetForegroundApp
//
//  Function:   Sets foreground application
//
//  Inputs:     MAINForeground  - Foreground application
//
//  Outputs     None
//
//  Changelog:  05/07/2018 - NVG: Created routine
//
//***************************************************************

void mainSetForegroundApp(etypeApplications MAINForeground)
{
    MAINForegroundApp = MAINForeground;
}
