/**
 ******************************************************************************
 * @file    DeviceIO.h
 * @author  Richard Osterloh (richard.osterloh@gmail.com)
 * @version V1.0.0
 * @date    31-October-2014
 * @brief   Device hardware abstractions
 ******************************************************************************
 * Device hardware abstractions
 ******************************************************************************
 */
#pragma once

#include "mbed.h"
#include "DHT.h"
//#include "GDE021A1.h"
#include "GPS.h"
#include "GPSTracker.h"
//#include DeviceFeedback.h

/**
 * @brief  IO state definition
 */
typedef enum
{
  ON       = 0x01,    /*!< Output high   */
  OFF      = 0x02,    /*!< Output low    */
  TOGGLE   = 0x03     /*!< Output toggle */
} IOTypdef;

/**
 * @brief  LED definition
 */
typedef enum
{
  GREEN    = 0x01,    /*!< Green LED = LED1 = PB_4 */
  RED      = 0x02     /*!< Red LED   = LED2 = PA_5 */
} LEDTypdef;

class DeviceIO
{
public:
    DeviceIO(GPSI2C&);

    bool userButtonPressed();
    GPSTracker& gpsTracker();
    //DeviceFeedback& deviceFeedback();
    void setLED(LEDTypdef led, IOTypdef state);
    //AnalogIn& analog1();
    DHT& temperatureSensor();
    //void displayTemperature(void);
    //void displayPrint(const char*, const char* = NULL, const char* = NULL);

private:
    DigitalIn _userButton;
    DigitalOut _led1;
    DigitalOut _led2;
    //AnalogIn _analog1;
    DHT _temperatureSensor;
    //GDE021A1 _display;
    GPSTracker _gpsTracker;
    //DeviceFeedback _deviceFeedback;
    //int _displayWrites;
};
