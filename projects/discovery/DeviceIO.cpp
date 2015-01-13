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
//#include "stdio.h"

DeviceIO::DeviceIO(void) :
    _userButton(PA_0),
    _led1(PB_4),
    _led2(PA_5),
    //_analog1(PA_1),
    _temperatureSensor(PA_4, DHT22), // PA_4, AM2302
    _display(PB_5, PB_3, PA_15, PB_11, PA_8, PB_10, PB_2) // mosi, sclk, cs, cd, busy, pwr, reset
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
/*
AnalogIn& DeviceIO::analog1()
{
    return _analog1;
}
*/
DHT& DeviceIO::temperatureSensor()
{
    return _temperatureSensor;
}

void DeviceIO::displayTemperature(void)
{
    char line1[15];
    char line2[15];

    int err = _temperatureSensor.readData();

    if(err == 0) {
      int t1 = _temperatureSensor.ReadTemperature();
      int t2 = t1 % 10;
      int h1 = _temperatureSensor.ReadHumidity();
      int h2 = h1 % 10;
      sprintf(line1, "Temp: %d.%d C", t1/10, t2);
      sprintf(line2, "Hum:  %d.%d %%", h1/10, h2);
    } else {
      sprintf(line1, "ERROR");
      sprintf(line2, "%d", err);
    }
    displayPrint(line1, line2);
}

void DeviceIO::displayPrint(const char *line1, const char *line2, const char *line3)
{
    _display.cls();
    _display.stringAtLine(3, (uint8_t*)line1);
    if(line2 != NULL) {
        _display.stringAtLine(2, (uint8_t*)line2);
        if(line3 != NULL) {
            _display.stringAtLine(1, (uint8_t*)line3);
        }
    }
    _display.refresh();
}
