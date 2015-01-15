#include "DeviceMemory.h"
#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#define PLATFORM_CREDENTIALS_FILE "001_CREDENTIALS"
#define CONFIGURATION_FILE "002_CONFIGURATION"

DeviceMemory::DeviceMemory(MDMSerial& mdm) :
    _mdm(mdm)
{
}

bool DeviceMemory::loadPlatformCredentials(char *username, char *password, size_t len)
{
  char buffer[len*2+3]; int res, len2;

  if ((res = _mdm.readFile(PLATFORM_CREDENTIALS_FILE, buffer, sizeof(buffer))) < 0)
    return false;

  buffer[(size_t)res] = '\0';
  sscanf(buffer, "%s\n%s\n%n", username, password, &len2);
  return res == len2;
}

bool DeviceMemory::savePlatformCredentials(char *username, char *password, size_t len)
{
  char buffer[len*2+3]; int res;

  res = snprintf(buffer, sizeof(buffer), "%s\n%s\n", username, password);
  if ((res < 0) || (res >= sizeof(buffer)))
    return false;

  resetPlatformCredentials();
  return (res == _mdm.writeFile(PLATFORM_CREDENTIALS_FILE, buffer, res));
}

bool DeviceMemory::resetPlatformCredentials()
{
  return _mdm.delFile(PLATFORM_CREDENTIALS_FILE);
}


bool DeviceMemory::loadConfiguration(char *cfg, size_t len)
{
  int res;

  if ((res = _mdm.readFile(CONFIGURATION_FILE, cfg, len)) < 0)
    return false;

  cfg[(size_t)res] = '\0';
  return true;
}

bool DeviceMemory::saveConfiguration(char *cfg)
{
  size_t len;

  len = strlen(cfg);

  resetConfiguration();
  return (_mdm.writeFile(CONFIGURATION_FILE, cfg, len) == len);
}

bool DeviceMemory::resetConfiguration()
{
  return _mdm.delFile(CONFIGURATION_FILE);
}
