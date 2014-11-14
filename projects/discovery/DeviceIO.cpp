/**
 ******************************************************************************
 * @file    DeviceIO.cpp
 * @author  Richard Osterloh (richard.osterloh@gmail.com)
 * @version V1.0.0
 * @date    31-October-2014
 * @brief   Device hardware abstractions
 ******************************************************************************
 * Device hardware abstractions
 ******************************************************************************
 */
#include "DeviceIO.h"

DeviceIO::DeviceIO(void) :
    _userButton(USER_BUTTON), // PA_0
    _led1(LED1), // PB_4
    _led2(LED2), // PA_5
    _analog1(A1), // PA_1
    _temperatureSensor(PA_4, AM2302),
    _display(PB_5, PB_4, PB_3, PA_15, PB_11, PA_8, PB_10, PB_2) // mosi, miso, sclk, cs, cd, busy, pwr, reset
{
  _led1 = 1;
  _led2 = 1;

  _display.splashScreen();
}

bool DeviceIO::userButtonPressed()
{
    return _userButton;
}

void DeviceIO::setLED(LEDTypdef led, IOTypdef state)
{
    switch(state) {
      case ON:
        led == GREEN ? _led1 = 1 : _led2 = 1;
        break;
      case OFF:
        led == GREEN ? _led1 = 0 : _led2 = 0;
        break;
      case TOGGLE:
        led == GREEN ? _led1 = !_led1 : _led2 = !_led2;
        break;
    }
}

AnalogIn& DeviceIO::analog1()
{
    return _analog1;
}

DHT& DeviceIO::temperatureSensor()
{
    return _temperatureSensor;
}

void DeviceIO::displayPrint(const char *line1, const char *line2, const char *line3)
{
    _display.cls();
    _display.stringAtLine(1, line1);
    if(line2 != NULL) {
        _display.stringAtLine(2, line2);
        if(line3 != NULL) {
            _display.stringAtLine(3, line3);
        }
    }
    _display.refresh();
}
