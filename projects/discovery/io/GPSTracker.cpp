#include "GPSTracker.h"
#include <stdlib.h>
#include <string.h>

GPSTracker::GPSTracker(GPSI2C& gps) :
    _gps(gps),
    _thread(GPSTracker::thread_func, this),
    _positionSet(false)
{
}

bool GPSTracker::position(GPSTracker::Position *position)
{
  bool result;

  _mutex.lock();
  if (_positionSet) {
    memcpy(position, &_position, sizeof(GPSTracker::Position));
    _positionSet = false;
    result = true;
  } else {
    result = false;
  }
  _mutex.unlock();

  return result;
}

void GPSTracker::thread()
{
  char buf[256], chr; // needs to be that big otherwise mdm isn't working
  int ret, len, n;
  double altitude, latitude, longitude;

  while (true) {
    ret = _gps.getMessage(buf, sizeof(buf));
    if (ret <= 0) {
      Thread::wait(100);
      continue;
    }

    len = LENGTH(ret);
    if ((PROTOCOL(ret) != GPSParser::NMEA) || (len <= 6))
        continue;

    // we're only interested in fixed GPS positions
    // we are not interested in invalid data
    if ((strncmp("$GPGGA", buf, 6) != 0) ||
        (!_gps.getNmeaItem(6, buf, len, n, 10)) || (n == 0))
        continue;

    // get altitude, latitude and longitude
    if ((!_gps.getNmeaAngle(2, buf, len, latitude)) ||
        (!_gps.getNmeaAngle(4, buf, len, longitude)) ||
        (!_gps.getNmeaItem(9, buf, len, altitude)) ||
        (!_gps.getNmeaItem(10, buf, len, chr)) ||
        (chr != 'M'))
        continue;

    _mutex.lock();
    _position.altitude = altitude;
    _position.latitude = latitude;
    _position.longitude = longitude;
    _positionSet = true;
    _mutex.unlock();
  }
}

void GPSTracker::thread_func(void const *arg)
{
  GPSTracker *that;
  that = (GPSTracker*)arg;
  that->thread();
}
