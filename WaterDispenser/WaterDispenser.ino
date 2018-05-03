#include <LCD.h>
#include <LiquidCrystal.h>
#include <LiquidCrystal_I2C.h>
#include <Wire.h>

#define PIN_MEGA_SDA    20
#define PIN_MEGA_SCL    21

#define LCD_WIDTH       20
#define LCD_HEIGHT      4

#define LCD_BACKLIGHT_PIN  3

//Set the pins on the I2C chip used for LCD connections
//(ADDR,EN,R/W,RS,D4,D5,D6,D7)
LiquidCrystal_I2C lcd(0x3F,2,1,0,4,5,6,7);

void setup()
{
   // Initialize LCD
   lcd.begin (LCD_WIDTH,LCD_HEIGHT);

   // Initialize LCD BACKLIGHT
   lcd.setBacklightPin(LCD_BACKLIGHT_PIN,POSITIVE);

   // Set backlight to high initially
   lcd.setBacklight(HIGH);
}

void loop()
{

}
