
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
char RTCDaysOfTheWeek[7][12] = {"Su", "Mo", "Tu", "We", "Th", "Fr", "Sa"};

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
    uint8_t RTCStartDateXCoordinate = 8;
    uint8_t RTCStartTimeXCoordinate = 0;
    // Get date and time
    RTCCurrentDateTime = rtc.now();

    // Print current time 
    rtcPrintDateTimeData(RTC_HOUR, RTCStartTimeXCoordinate,0);
    lcdPrintCharArray(RTCStartTimeXCoordinate+2, 0, ":");
    rtcPrintDateTimeData(RTC_MINUTE, RTCStartTimeXCoordinate+3,0);
    lcdPrintCharArray(RTCStartTimeXCoordinate+5, 0, ":");
    rtcPrintDateTimeData(RTC_SECOND, RTCStartTimeXCoordinate+6, 0);

    // Print current date
    lcdPrintCharArray(18, 0, RTCDaysOfTheWeek[RTCCurrentDateTime.dayOfTheWeek()]);
    lcdPrintCharArray(RTCStartDateXCoordinate, 0, " ");
    rtcPrintDateTimeData(RTC_DAY, RTCStartDateXCoordinate + 1, 0);
    lcdPrintCharArray(RTCStartDateXCoordinate + 3, 0, "/");
    rtcPrintDateTimeData(RTC_MONTH, RTCStartDateXCoordinate + 4, 0);
    lcdPrintCharArray(RTCStartDateXCoordinate + 6, 0, "/");
    rtcPrintDateTimeData(RTC_YEAR, RTCStartDateXCoordinate + 7, 0);
}

//***************************************************************
//
//  Name:       rtcGetRTCData
//
//  Function:   Gets RTC Paremeter
//
//  Inputs:     RTCParam - parameter being asked for
//
//  Outputs:    Parameter being asked for
//
//  Changelog:  05/05/2018 - NVG: Created routine
//
//***************************************************************

uint8_t rtcGetRTCData(uint8_t RTCParam)
{
    uint8_t RTCData;

    // Deterermine which parameter is being asked for
    switch (RTCParam)
    {
        case RTC_SECOND:
        {
            return RTCCurrentDateTime.second();
        }
        break;

        case RTC_MINUTE:
        {
            return RTCCurrentDateTime.minute();
        }
        break;

        case RTC_HOUR:
        {
            return RTCCurrentDateTime.hour();
        }
        break;

        case RTC_DAY_OF_WEEK:
        {
            return RTCCurrentDateTime.dayOfTheWeek();
        }
        break;

        case RTC_DAY:
        {
            return RTCCurrentDateTime.day();
        }
        break;

        case RTC_MONTH:
        {
            return RTCCurrentDateTime.month();
        }
        break;

        case RTC_YEAR:
        {
            // Get 16bit year data
            uint16_t RTCYearData = RTCCurrentDateTime.year();

            // Get year data after year 2000
            RTCData = RTCYearData - 2000;

            return RTCData;
        }
        break;
    }
}
