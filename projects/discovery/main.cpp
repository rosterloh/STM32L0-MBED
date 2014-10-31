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
#include "DeviceInfo.h"

int main() {
  led1 = 1; led2 = 1;
  ticker.attach(&doTick, 2.0); // the address of the function to be attached (doTick) and the interval (2 seconds)

  // spin in a main loop. ticker will interrupt it to call doTick
  while(1) {
    led2 = !led2;
    wait(0.5);
  }
}
