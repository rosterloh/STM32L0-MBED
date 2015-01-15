#include "SignalQualityMeasurement.h"

SignalQualityMeasurement::SignalQualityMeasurement(long& deviceId, DeviceInfo& deviceInfo) :
    _deviceId(deviceId),
    _deviceInfo(deviceInfo)
{
  _init = false;
}

bool SignalQualityMeasurement::init()
{
  if (_init)
    return false;

  _init = true;
  return true;
}

bool SignalQualityMeasurement::run()
{
  DeviceInfo::SignalQuality *signalQuality;

  if ((signalQuality = _deviceInfo.signalQuality()) == NULL)
    return false;

  //ComposedRecord record;
  //IntegerValue msgId(104);
  //IntegerValue devId(_deviceId);
  //IntegerValue rssi(signalQuality->rssi);
  //IntegerValue ber(signalQuality->ber);

  return true;
}
