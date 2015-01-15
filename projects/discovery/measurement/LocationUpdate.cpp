#include "LocationUpdate.h"

LocationUpdate::LocationUpdate(long& deviceId, GPSTracker& gpsTracker) :
    _deviceId(deviceId),
    _gpsTracker(gpsTracker)
{
  _init = false;
}

bool LocationUpdate::init()
{
  if (_init)
    return false;

  // Add any init steps here

  _init = true;
  return true;
}

bool LocationUpdate::run()
{
  GPSTracker::Position position;

  if (!_gpsTracker.position(&position)) {
    puts("No GPS data available.");
    return true;
  }

  //int devId(_deviceId);
  //double altitude(position.altitude, 2);
  //double latitude(position.latitude, 6);
  //double longitude(position.longitude, 6);

  puts("Sending GPS measurement.");

  return true;
}
