#include "TemperatureMeasurement.h"

TemperatureMeasurement::TemperatureMeasurement(long& deviceId, DHT& sensor) :
    _deviceId(deviceId),
    _sensor(sensor)
{
  _init = false;
}

bool TemperatureMeasurement::init()
{
  if (_init)
    return false;

  _init = true;
  return true;
}

bool TemperatureMeasurement::run()
{
  int err = _sensor.readData();

  if(err == 0) {
    /*
    int t1 = _sensor.ReadTemperature();
    int t2 = t1 % 10;
    int h1 = _sensor.ReadHumidity();
    int h2 = h1 % 10;
    */
    //float temp = _sensor.ReadTemperature(CELCIUS);
    //float hum = _sensor.ReadHumidity();
  } else {
    puts("ERROR collecting temperature measurement");
    return false;
  }

  return true;
}
