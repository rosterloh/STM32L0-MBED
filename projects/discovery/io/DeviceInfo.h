#pragma once

#include <stddef.h>
#include <stdint.h>
#include "MDM.h"

class DeviceInfo
{
public:
  DeviceInfo(MDMSerial& mdm, MDMParser::DevStatus& devStatus);

  typedef struct {
    int rssi;   // RSSI in dBm
    int ber;    // BER in %
  } SignalQuality;

  const char * imsi();
  const char * imei();
  const char * cellId();
  const char * iccid();
  SignalQuality * signalQuality();

protected:
  bool refreshNetStatus();

private:
  MDMSerial& _mdm;
  MDMParser::DevStatus _devStatus;
  MDMParser::NetStatus _netStatus;
  char _cellId[9];
  SignalQuality _signalQuality;
};
