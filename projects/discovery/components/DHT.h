#ifndef MBED_DHT_H
#define MBED_DHT_H

#include "mbed.h"

enum eType{
    DHT11     = 11,
    DHT22     = 22,
} ;

class DHT {

public:

#define DHTLIB_OK                0
#define DHTLIB_ERROR_CHECKSUM   -1
#define DHTLIB_ERROR_TIMEOUT    -2

    DHT(PinName pin,int DHTtype);
    ~DHT();
    uint8_t bits[5];
    int readData(void);
    int ReadHumidity(void);
    int ReadTemperature(void);
    int humidity;
    int temperature;
    Timer tmr;

private:
    PinName _pin;
    int _DHTtype;
    int DHT_data[6];
};

#endif
