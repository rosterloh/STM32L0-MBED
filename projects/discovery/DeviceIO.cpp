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
    _resetButton(USER_BUTTON), // PA_0
    _led1(LED1), // PB_4
    _led2(LED2), // PA_5
    _analog1(A1), // PA_1
    _display(PB_5, PB_4, PB_3, PA_15, PB_11, PA_8, PB_10, PB_2) // mosi, miso, sclk, cs,  cd, busy, pwr, reset
{
}

bool DeviceIO::userButtonPressed()
{
    return _userButton;
}

void DeviceIO::greenLED(int state)
{
    if(state) {
      _led1 = 1;
    } else {
      _led1 = 0;
    }
}

void DeviceIO::redLED(int state)
{
    if(state) {
      _led2 = 1;
    } else {
      _led2 = 0;
    }
}

AnalogIn& DeviceIO::analog1()
{
    return _analog1;
}
/*
void DeviceIO::displayPrint(const char *line1, const char *line2, const char *line3)
{
    printf(GRE "io::lcdPrint" DEF "\r\n");
    _display.cls();
    _display.locate(0, 0);

    _display.printf("%s\n", line1);
    printf(GRE "> " CYA "%s\r\n" DEF, line1);

    if (line2 != NULL) {
        _display.printf("%s\n", line2);
        printf(GRE "> " CYA "%s\r\n" DEF, line2);

        if (line3 != NULL) {
            _display.printf("%s\n", line3);
            printf(GRE "> " CYA "%s\r\n" DEF, line3);
        }
    }
}
*/
