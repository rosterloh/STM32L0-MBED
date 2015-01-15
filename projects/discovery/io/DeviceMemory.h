#pragma once

#include <stddef.h>
#include "MDM.h"

/**
 * Device Memory storage handler
 */
class DeviceMemory
{
public:
  DeviceMemory(MDMSerial&);

  /** loads credentials from persistent memory */
  bool loadPlatformCredentials(char*, char*, size_t);

  /** saves credentials to persistent memory */
  bool savePlatformCredentials(char*, char*, size_t);

  /** removes credentials from persistent memory */
  bool resetPlatformCredentials();

  /** loads configuration from persistent memory */
  bool loadConfiguration(char*, size_t);

  /** saves configuration to persistent memory */
  bool saveConfiguration(char*);

  /** removes configuration from persistent memory */
  bool resetConfiguration();

private:
  MDMSerial& _mdm;
};
