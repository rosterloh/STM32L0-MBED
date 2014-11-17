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
#include "stdio.h"

DeviceIO io;
Ticker ticker;

void doTick() {
  io.setLED(GREEN, TOGGLE);
  io.displayTemperature();
}

int main() {
  ticker.attach(&doTick, 30.0); // the address of the function to be attached (doTick) and the interval (2 seconds)
  //io.displayTemperature();
  while(1) {
    io.setLED(RED, TOGGLE);
    wait(0.5);
  }

}
