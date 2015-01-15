#include "MbedAgent.h"
#include "rtos.h"

MbedAgent::MbedAgent(DeviceIO& io, MDMSerial& mdm, DeviceInfo& deviceInfo, DeviceMemory& deviceMemory) :
    _io(io),
    _mdm(mdm),
    _deviceInfo(deviceInfo),
    _deviceMemory(deviceMemory),
    _configurationProperties(_deviceConfiguration),
    //_client(MBED_AGENT_HOST, MBED_AGENT_PORT, MBED_AGENT_DEVICE_IDENTIFIER),
    //_bootstrap(_client, _io, _deviceInfo, _deviceMemory),
    //_integration(_client, _tpl, _deviceId, _deviceInfo),
    //_configurationSynchronization(_client, _tpl, _deviceId, _deviceMemory, _deviceConfiguration, _configurationProperties),
    _signalQualityMeasurement(_deviceId, _deviceInfo),
    _temperatureMeasurement(_deviceId, _io.temperatureSensor()),
    //_accelerationMeasurement(_client, _tpl, _deviceId, _io.accelerometer()),
    //_analogMeasurement(_client, _tpl, _deviceId, _io.analog1(), _io.analog2()),
    _locationUpdate(_deviceId, _io.gpsTracker()),
    //_operationSupport(_client, _tpl, _deviceId, _configurationSynchronization, _io),
    _deviceId(0)
{
}

bool MbedAgent::init()
{
  if ((!_signalQualityMeasurement.init()) ||
      (!_temperatureMeasurement.init()) ||
      //(!_accelerationMeasurement.init()) ||
      //(!_analogMeasurement.init()) ||
      (!_locationUpdate.init())) {
      puts("Initialization failed.");
      return false;
  }
  return true;
}

bool MbedAgent::run()
{/*
  // device bootstrapping process
  if (!_bootstrap.setUpCredentials())
    return false;

  Thread::wait(5000);

  _io.lcdPrint("INTEGRATION");
  if (!_integration.integrate()) {
    return false;
  }

  if (!_configurationSynchronization.integrate()) {
    return false;
  }

  char status[60];
  snprintf(status, sizeof(status), "ID: %ld", _deviceId);
  _io.displayPrint("INTEGRATED", status);
  */
  loop();

  return true;
}

void MbedAgent::loop()
{
  Timer timer; int interval;

  timer.start();
  while (true) {
    timer.reset();

    //_configurationSynchronization.run();
    _signalQualityMeasurement.run();
    _temperatureMeasurement.run();
    //_accelerationMeasurement.run();
    //_analogMeasurement.run();
    _locationUpdate.run();
    //_operationSupport.run();

    if ((interval = _configurationProperties.readInterval()) == 0)
      break;

    while (timer.read() < interval) {
        Thread::yield();
    }
  }
}
