#pragma once

#include <stddef.h>
#include <stdint.h>

#define DEVICE_CONFIGURATION_SIZE 8

class DeviceConfiguration
{
public:
  DeviceConfiguration();
  ~DeviceConfiguration();

  bool read(const char*);
  bool write(char*, size_t);

  bool set(const char*, const char*);
  const char * get(const char*);
  bool unset(const char*);
  bool has(const char*);
  void clear();

protected:
  struct KeyValue {
    char *key;
    char *value;
  };

  KeyValue * search(const char*);

private:
  KeyValue _items[DEVICE_CONFIGURATION_SIZE];
};
