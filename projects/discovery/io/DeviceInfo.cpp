#include "DeviceInfo.h"
#include <stdlib.h>
#include <string.h>

DeviceInfo::DeviceInfo(MDMSerial& mdm, MDMParser::DevStatus& devStatus) :
    _mdm(mdm)
{
  *_cellId = '\0';
  memcpy(&_devStatus, &devStatus, sizeof(MDMParser::DevStatus));
  memset(&_netStatus, 0, sizeof(MDMParser::NetStatus));
  memset(&_signalQuality, 0, sizeof(DeviceInfo::SignalQuality));
}

const char * DeviceInfo::imsi()
{
  return _devStatus.imsi;
}

const char * DeviceInfo::imei()
{
  return _devStatus.imei;
}

const char * DeviceInfo::cellId()
{
  if (!refreshNetStatus())
    return NULL;

  if (snprintf(_cellId, sizeof(_cellId), "%X", _netStatus.ci) < 1)
    return NULL;

  return _cellId;
}

const char * DeviceInfo::iccid()
{
  return _devStatus.ccid;
}

DeviceInfo::SignalQuality * DeviceInfo::signalQuality()
{
  memset(&_signalQuality, 0, sizeof(DeviceInfo::SignalQuality));
  if (!refreshNetStatus())
    return NULL;

  if ((_netStatus.rssi == 0) || (_netStatus.ber == 0))
    return NULL;

  _signalQuality.rssi = _netStatus.rssi;
  _signalQuality.ber = _netStatus.ber;
  return &_signalQuality;
}

bool DeviceInfo::refreshNetStatus()
{
  return _mdm.checkNetStatus(&_netStatus);
}
