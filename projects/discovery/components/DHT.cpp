#include "DHT.h"
#include "mbed.h"

#define DHT_DATA_BIT_COUNT 41

DHT::DHT(PinName pin,int DHTtype) {
    _pin = pin;
    _DHTtype = DHTtype;
}

DHT::~DHT() {
}

int DHT::readData() {
    Timer tmr;
    DigitalInOut data_pin(_pin);
    // BUFFER TO RECEIVE
    //uint8_t bits[5];
    uint8_t cnt = 7;
    uint8_t idx = 0;

    tmr.stop();
    tmr.reset();

    // EMPTY BUFFER
    for(int i=0; i< 5; i++) bits[i] = 0;

    // REQUEST SAMPLE
    data_pin.output();
    data_pin.write(0);
    wait_ms(18);
    data_pin.write(1);
    wait_us(40);
    data_pin.input();

    // ACKNOWLEDGE or TIMEOUT
    unsigned int loopCnt = 10000;

    while(!data_pin.read())if(!loopCnt--)return DHTLIB_ERROR_TIMEOUT;

    loopCnt = 10000;

    while(data_pin.read())if(!loopCnt--)return DHTLIB_ERROR_TIMEOUT;

    // READ OUTPUT - 40 BITS => 5 BYTES or TIMEOUT
    for(int i=0; i<40; i++){

        loopCnt = 10000;

        while(!data_pin.read()) {
          if(loopCnt-- == 0) {
            return DHTLIB_ERROR_TIMEOUT;
          }
        }

        //unsigned long t = micros();
        tmr.start();

        loopCnt = 10000;

        while(data_pin.read()) {
          if(!loopCnt--) {
            return DHTLIB_ERROR_TIMEOUT;
          }
        }

        if(tmr.read_us() > 40) bits[idx] |= (1 << cnt);

        tmr.stop();
        tmr.reset();

        if(cnt == 0){   // next byte?

            cnt = 7;    // restart at MSB
            idx++;      // next byte!

        }else cnt--;

    }

    uint8_t sum = bits[0] + bits[1] + bits[2] + bits[3];

    if(bits[4] != sum) return DHTLIB_ERROR_CHECKSUM;

    return DHTLIB_OK;
}

int DHT::ReadTemperature() {
    switch (_DHTtype) {
        case DHT11:
            temperature = bits[2];
            return int(temperature);
        case DHT22:
            temperature = bits[2] & 0x7F;
            temperature *= 256;
            temperature += bits[3];
            //temperature /= 10;
            if (bits[2] & 0x80) {
              temperature *= -1;
            }
            return int(temperature);
    }
    return 0;
}

int DHT::ReadHumidity() {
    switch (_DHTtype) {
        case DHT11:
            humidity = bits[0];
            return int(humidity);
        case DHT22:
            humidity = bits[0];
            humidity *= 256;
            humidity += bits[1];
            //humidity /= 10;
            return int(humidity);
    }
    return 0;
}
