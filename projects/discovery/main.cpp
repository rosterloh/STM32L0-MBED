/**
 ******************************************************************************
 * @file    main.cpp
 * @author  Richard Osterloh (richard.osterloh@gmail.com)
 * @version V1.0.0
 * @date    31-October-2014
 * @brief   Project to play around with MBED on the STM32L0 Discovery Board.
 ******************************************************************************
 * Main application configuration.
 ******************************************************************************
 */

#include "mbed.h"
//#include "rtos.h"
#include "DeviceIO.h"
#include <string>

DeviceIO io;
Ticker ticker;

void doTick() {
  io.setLED(GREEN, TOGGLE);

  char line1[15];
  char line2[15];
  printf("Temp: %4.2f C", io.temperatureSensor().ReadTemperature(CELCIUS));
  printf("Hum: %4.2f", io.temperatureSensor().ReadHumidity());
}

int main() {
  ticker.attach(&doTick, 2.0); // the address of the function to be attached (doTick) and the interval (2 seconds)

  while(1) {
    io.setLED(RED, TOGGLE);
    wait(0.5);
  }

}
