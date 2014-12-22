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
//#include "DeviceIO.h"
//#include "stdio.h"

//DeviceIO io;
//Ticker ticker;
DigitalOut green(PB_4);
DigitalOut red(PA_5);
Serial pc(SERIAL_TX, SERIAL_RX);
/*
void doTick() {
  green = !green;
  //io.setLED(GREEN, TOGGLE);
  pc.printf("Reading Temperature");
  //io.displayTemperature();
}
*/
void led2_thread(void const *args) {
    while (true) {
        //io.setLED(GREEN, TOGGLE);
        green = !green;
        Thread::wait(2000);
    }
}

int main() {
  green = 0;
  red = 0;
  pc.printf("STM32L0 Discovery");
  //ticker.attach(&doTick, 30.0); // the address of the function to be attached (doTick) and the interval (2 seconds)
  //io.displayTemperature();
  Thread thread(led2_thread);

  while(1) {
    red = !red;
    //io.setLED(RED, TOGGLE);
    //wait(0.5);
    Thread::wait(500);
  }

}
