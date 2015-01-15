#pragma once

#include <stddef.h>
#include "DeviceConfiguration.h"

#define CONFIGURATION_PROPERTY_INTERVAL "300"

class ConfigurationProperties
{
public:
  ConfigurationProperties(DeviceConfiguration&);

  bool resetConfiguration();
  bool validateProperties();

  int readInterval();

private:
  DeviceConfiguration& _deviceConfiguration;
};
