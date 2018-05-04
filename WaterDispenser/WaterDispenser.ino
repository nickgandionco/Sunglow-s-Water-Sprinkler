
//***********************
// INCLUDES
//***********************

#include <RTClib.h>
#include <Wire.h>
#include "C:\Program Files (x86)\Arduino\libraries\LiquidCrystal_I2C\LCD.h"
#include <LiquidCrystal_I2C.h>


//***********************
// DEFINITIONS
//***********************

#define PIN_MEGA_SDA    20
#define PIN_MEGA_SCL    21

#define LCD_WIDTH       20
#define LCD_HEIGHT      4

#define LCD_BACKLIGHT_PIN  3


//***********************
// GLOBAL VARIABLES
//***********************

String      SelectOption         = "Select options";
int         SelectOptionChar     = SelectOption.length();
char        daysOfTheWeek[7][12] = {"Sun", "Mon", "Tue", "Wed", "Thu", "Fri", "Sat"};

RTC_DS1307  rtc;
DateTime    RTCCurrentDateTime;

//Set the pins on the I2C chip used for LCD connections (ADDR,EN,R/W,RS,D4,D5,D6,D7)
LiquidCrystal_I2C lcd(0x3F,2,1,0,4,5,6,7);


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
//  Changelog:  04/06/2018 - NVG: Created routine
//
//***************************************************************

void setup()
{
  // Initialize LCD
  lcd.begin (LCD_WIDTH,LCD_HEIGHT);

  // Initialize LCD BACKLIGHT
  lcd.setBacklightPin(LCD_BACKLIGHT_PIN,POSITIVE);

  // Set backlight to high initially
  lcd.setBacklight(HIGH);

  // Auto update from computer time
  rtc.adjust(DateTime(F(__DATE__), F(__TIME__)));
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
    // Get date and time
    RTCCurrentDateTime = rtc.now();

    //int LcdXOffset = ((LCD_WIDTH - SelectOptionChar)/2);

    // Set cursor to (0,0)
    //lcd.setCursor(LcdXOffset, 0);

    // Print text
    //lcd.print(SelectOption);

    // Print current time
    rtcPrintHour(0,1);
    lcd.print(':');
    rtcPrintMinute(3,1);
    lcd.print(':');
    rtcPrintSecond(6, 1);

    lcd.setCursor(0, 0);
    lcd.print(daysOfTheWeek[RTCCurrentDateTime.dayOfTheWeek()]);
    lcd.print(" ");
    lcd.print(RTCCurrentDateTime.day());
    lcd.print('/');
    lcd.print(RTCCurrentDateTime.month());
    lcd.print('/');
    lcd.print(RTCCurrentDateTime.year());
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
//  Changelog:  04/06/2018 - NVG: Created routine
//
//***************************************************************

void rtcPrintHour(int RTCYCoordinate, int RTCXCoordinate)
{
    int RTCHour = RTCCurrentDateTime.hour();

    // Set lcd cursor
    lcd.setCursor(RTCYCoordinate, RTCXCoordinate);
    
    // Manually add zero if single digits
    if (RTCHour < 10)
    {
        lcd.print("0");
    }

    lcd.print(RTCHour);
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
//  Changelog:  04/06/2018 - NVG: Created routine
//
//***************************************************************

void rtcPrintMinute(int RTCYCoordinate, int RTCXCoordinate)
{
    int RTCMinute = RTCCurrentDateTime.minute();

    // Set lcd cursor
    lcd.setCursor(RTCYCoordinate, RTCXCoordinate);
    
    // Manually add zero if single digits
    if (RTCMinute < 10)
    {
        lcd.print("0");
    }

    lcd.print(RTCMinute);
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
//  Changelog:  04/06/2018 - NVG: Created routine
//
//***************************************************************

void rtcPrintSecond(int RTCYCoordinate, int RTCXCoordinate)
{
    int RTCSecond = RTCCurrentDateTime.second();

    // Set lcd cursor
    lcd.setCursor(RTCYCoordinate, RTCXCoordinate);
    
    // Manually add zero if single digits
    if (RTCSecond < 10)
    {
        lcd.print("0");
    }

    lcd.print(RTCSecond);
}