#pragma once

#include <stddef.h>
#include "MDM.h"
#include "DeviceIO.h"
//#include "RtosSmartRest.h"
//#include "SmartRestTemplate.h"
#include "DeviceInfo.h"
//#include "DeviceMemory.h"
//#include "DeviceBootstrap.h"
//#include "DeviceIntegration.h"
#include "DeviceConfiguration.h"
#include "ConfigurationProperties.h"
//#include "ConfigurationSynchronization.h"
#include "SignalQualityMeasurement.h"
//#include "TemperatureMeasurement.h"
//#include "AccelerationMeasurement.h"
//#include "AnalogMeasurement.h"
#include "LocationUpdate.h"
//#include "OperationSupport.h"

//#define MBED_AGENT_HOST "developer.cumulocity.com"
//#define MBED_AGENT_PORT 80
//#define MBED_AGENT_DEVICE_IDENTIFIER "com_cumulocity_MbedAgent_1.5.1"

class MbedAgent
{
public:
  MbedAgent(DeviceIO&, MDMSerial&, DeviceInfo&/*, DeviceMemory&*/);

  bool init();
  bool run();

protected:
  void loop();

private:
  DeviceIO& _io;
  MDMSerial& _mdm;
  DeviceInfo& _deviceInfo;
  //DeviceMemory& _deviceMemory;
  DeviceConfiguration _deviceConfiguration;
  ConfigurationProperties _configurationProperties;
  //RtosSmartRest _client;
  //SmartRestTemplate _tpl;
  //DeviceBootstrap _bootstrap;
  //DeviceIntegration _integration;
  //ConfigurationSynchronization _configurationSynchronization;
  SignalQualityMeasurement _signalQualityMeasurement;
  //TemperatureMeasurement _temperatureMeasurement;
  //AccelerationMeasurement _accelerationMeasurement;
  //AnalogMeasurement _analogMeasurement;
  LocationUpdate _locationUpdate;
  //OperationSupport _operationSupport;
  long _deviceId;
};
