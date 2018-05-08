//***********************
// INCLUDES
//***********************

#include <Wire.h>
#include "C:\Program Files (x86)\Arduino\libraries\LiquidCrystal_I2C\LCD.h"
#include <LiquidCrystal_I2C.h>

//***********************
// DEFINITIONS
//***********************

#define LCD_WIDTH           20
#define LCD_HEIGHT          4

#define LCD_BACKLIGHT_PIN   3

//***********************
// GLOBAL VARIABLES
//***********************

//Set the pins on the I2C chip used for LCD connections (ADDR,EN,R/W,RS,D4,D5,D6,D7)
LiquidCrystal_I2C lcd(0x3F,2,1,0,4,5,6,7);

//***************************************************************
//
//  Name:       lcdInit
//
//  Function:   Initializes LCD
//
//  Inputs:     None
//
//  Outputs     None
//
//  Changelog:  05/05/2018 - NVG: Created routine
//
//***************************************************************

void lcdInit()
{
    // Initialize LCD
    lcd.begin (LCD_WIDTH,LCD_HEIGHT);

    // Initialize LCD backlight
    lcd.setBacklightPin(LCD_BACKLIGHT_PIN,POSITIVE);

    // Set backlight to high initially
    lcd.setBacklight(1);
}

//***************************************************************
//
//  Name:       lcdPrintCharArray
//
//  Function:   Prints character arrays
//
//  Inputs:     LCDX            - X coordinate to print
//              LCDY            - Y coordinate to print
//              LCDDataToPrint  - Data to print
//
//  Outputs     None
//
//  Changelog:  05/08/2018 - NVG: Created routine
//
//***************************************************************

void lcdPrintCharArray(uint8_t LCDX, uint8_t LCDY, char LCDDataToPrint[])
{
    lcd.setCursor(LCDX, LCDY);
    lcd.print(LCDDataToPrint);
}

//***************************************************************
//
//  Name:       lcdPrintInt
//
//  Function:   Prints integer
//
//  Inputs:     LCDX            - X coordinate to print
//              LCDY            - Y coordinate to print
//              LCDDataToPrint  - Data to print
//
//  Outputs     None
//
//  Changelog:  05/08/2018 - NVG: Created routine
//
//***************************************************************

void lcdPrintInt(uint8_t LCDX, uint8_t LCDY, uint8_t LCDDataToPrint)
{
    lcd.setCursor(LCDX, LCDY);
    lcd.print(LCDDataToPrint);
}

//***************************************************************
//
//  Name:       lcdClearLCD
//
//  Function:   Clears entire LCD
//
//  Inputs:     None
//
//  Outputs     None
//
//  Changelog:  05/08/2018 - NVG: Created routine
//
//***************************************************************

void lcdClearLCD()
{
    lcd.clear();
}

//***************************************************************
//
//  Name:       lcdSetBacklight
//
//  Function:   Turn backlight on or off
//
//  Inputs:     LCDBacklightStatus - Status of backlight (ON/OFF)
//
//  Outputs     None
//
//  Changelog:  05/08/2018 - NVG: Created routine
//
//***************************************************************

void lcdSetBacklight(uint8_t LCDBacklightStatus)
{
    lcd.setBacklight(LCDBacklightStatus);
}