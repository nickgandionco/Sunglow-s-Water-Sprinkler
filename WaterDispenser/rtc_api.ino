
//***********************
// INCLUDES
//***********************

#include <RTClib.h>

//***********************
// ENUMERATIONS
//***********************

typedef enum
{
    RTC_SECOND,
    RTC_MINUTE,
    RTC_HOUR,
    RTC_DAY_OF_WEEK,
    RTC_DAY,
    RTC_MONTH,
    RTC_YEAR,
} etypeRTCData;

//***********************
// GLOBAL VARIABLES
//***********************

RTC_DS1307      rtc;
DateTime        RTCCurrentDateTime;
char RTCDaysOfTheWeek[7][12] = {"Sun", "Mon", "Tue", "Wed", "Thu", "Fri", "Sat"};

//***************************************************************
//
//  Name:       rtcInit()
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

void rtcInit()
{
    // Auto update from computer time
    rtc.adjust(DateTime(F(__DATE__), F(__TIME__)));
}

//***************************************************************
//
//  Name:       rtcPrintHour
//
//  Function:   Prints hour on the LCD
//
//  Inputs:     RTCParam        - RTC Parameter
//              RTCYCoordinate  - Y coordinate to print
//              RTCXCoordinate  - X coordinate to print
//
//  Outputs     None
//
//  Changelog:  05/03/2018 - NVG: Created routine
//
//***************************************************************

void rtcPrintDateTimeData(uint8_t RTCParam, uint8_t RTCYCoordinate, uint8_t RTCXCoordinate)
{
    uint8_t RTCData;

    // Deterermine which parameter is being printed and place it in varaialbe
    switch (RTCParam)
    {
        case RTC_SECOND:
        {
            RTCData = RTCCurrentDateTime.second();
        }
        break;

        case RTC_MINUTE:
        {
            RTCData = RTCCurrentDateTime.minute();
        }
        break;

        case RTC_HOUR:
        {
            RTCData = RTCCurrentDateTime.hour();
        }
        break;

        case RTC_DAY_OF_WEEK:
        {

        }
        break;

        case RTC_DAY:
        {
            RTCData = RTCCurrentDateTime.day();
        }
        break;

        case RTC_MONTH:
        {
            RTCData = RTCCurrentDateTime.month();
        }
        break;

        case RTC_YEAR:
        {
            // Get 16bit year data
            uint16_t RTCYearData = RTCCurrentDateTime.year();

            // Get year data after year 2000
            RTCData = RTCYearData - 2000;
        }
        break;
    }
    

    // Manually add zero if single digits
    if (RTCData < 10)
    {
        // Print without  zero supression
        lcdPrintCharArray(RTCYCoordinate, RTCXCoordinate, "0");
        lcdPrintInt(RTCYCoordinate+1, RTCXCoordinate, RTCData);
    }
    else
    {
        lcdPrintInt(RTCYCoordinate, RTCXCoordinate, RTCData);
    }
}

//***************************************************************
//
//  Name:       rtcPrintDateAndTime
//
//  Function:   Prints date and time in home screen
//
//  Inputs:     None
//
//  Outputs     None
//
//  Changelog:  05/05/2018 - NVG: Created routine
//
//***************************************************************

void rtcPrintDateAndTime()
{
    // Get date and time
    RTCCurrentDateTime = rtc.now();

    // Print current time 
    rtcPrintDateTimeData(RTC_HOUR, 0,1);
    lcdPrintCharArray(2, 1, ":");
    rtcPrintDateTimeData(RTC_MINUTE, 3,1);
    lcdPrintCharArray(5, 1, ":");
    rtcPrintDateTimeData(RTC_SECOND, 6, 1);

    // Print current date
    lcdPrintCharArray(0, 0, RTCDaysOfTheWeek[RTCCurrentDateTime.dayOfTheWeek()]);
    lcdPrintCharArray(4, 0, " ");
    rtcPrintDateTimeData(RTC_DAY, 5, 0);
    lcdPrintCharArray(7, 0, "/");
    rtcPrintDateTimeData(RTC_MONTH, 8, 0);
    lcdPrintCharArray(10, 0, "/");
    rtcPrintDateTimeData(RTC_YEAR, 11, 0);
}
