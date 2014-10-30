/*
 * This is the original mbed example
 */

#include "mbed.h"

DigitalOut led1(LED1);
Serial pc(USBTX, USBRX);

int main() {
  int i = 0;
  while(1) {
    led1 = 1;
    wait(0.3);
    led1 = 0;
    wait(0.3);
    pc.printf("i: %i\n\r", i);
    ++i;
  }
}
