/** Simple blinky example to show basic timing is working
 *
 * If you want to use basic waits remove USE_TICKER define
 *
 */

#include "mbed.h"

#define USE_TICKER

DigitalOut led1(LED1);
DigitalOut led2(LED2);
Serial pc(USBTX, USBRX);
#ifdef USE_TICKER
Ticker ticker;

void doTick() {
    led1 = !led1;
}

int main() {
  led1 = 1; led2 = 1;
  ticker.attach(&doTick, 2.0); // the address of the function to be attached (doTick) and the interval (2 seconds)

  // spin in a main loop. ticker will interrupt it to call doTick
  while(1) {
    led2 = !led2;
    wait(0.5);
  }
}
#else
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
#endif
