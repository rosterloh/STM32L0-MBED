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

#ifndef DEVICEIO_H
#define DEVICEIO_H

#include "mbed.h"
#include "GDE021A1.h"

class DeviceIO
{
public:
    DeviceIO(void);

    bool userButtonPressed();
    void greenLED(int state);
    void redLED(int state);
    AnalogIn& analog1();
    //void displayPrint(const char*, const char* = NULL, const char* = NULL);

private:
    DigitalIn _userButton;
    DigitalOut _led1;
    DigitalOut _led2;
    AnalogIn _analog1;
    GDE021A1 _display;
};

#endif
