#include "ConfigurationProperties.h"
#include <stdlib.h>
#include <stdio.h>
#include <string.h>

ConfigurationProperties::ConfigurationProperties(DeviceConfiguration& deviceConfiguration) :
    _deviceConfiguration(deviceConfiguration)
{
}

bool ConfigurationProperties::resetConfiguration()
{
  return (_deviceConfiguration.clear(), _deviceConfiguration.set("interval", CONFIGURATION_PROPERTY_INTERVAL));
}

bool ConfigurationProperties::validateProperties()
{
  return (readInterval() > 0);
}

int ConfigurationProperties::readInterval()
{
  const char *prop; int res, ln;

  if ((prop = _deviceConfiguration.get("interval")) == NULL)
    prop = CONFIGURATION_PROPERTY_INTERVAL;

  ln = -1;
  if ((sscanf(prop, "%d%n", &res, &ln) != 1) || (ln != strlen(prop)))
    return 0;

  return res;
}
