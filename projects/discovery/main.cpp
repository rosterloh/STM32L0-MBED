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
#include "rtos.h"
#include "DeviceIO.h"

DeviceIO io;

void led2_thread(void const *args) {
    while (true) {
        io.setLED(GREEN, TOGGLE);
        io.displayTemperature();
        Thread::wait(10000);
    }
}

int main() {
  //io.displayTemperature();
  Thread thread(led2_thread);

  while(1) {
    io.setLED(RED, TOGGLE);
    Thread::wait(500);
  }

}
