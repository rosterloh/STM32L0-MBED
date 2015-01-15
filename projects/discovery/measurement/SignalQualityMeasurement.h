#pragma once

#include "DeviceInfo.h"

class SignalQualityMeasurement
{
public:
  SignalQualityMeasurement(long&, DeviceInfo&);

  bool init();
  bool run();

private:
  bool _init;
  long& _deviceId;
  DeviceInfo& _deviceInfo;
};
