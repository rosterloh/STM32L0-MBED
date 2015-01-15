#pragma once

#include "GPSTracker.h"

class LocationUpdate
{
public:
  LocationUpdate(long&, GPSTracker&);

  bool init();
  bool run();

private:
  bool _init;
  long& _deviceId;
  GPSTracker& _gpsTracker;
};
