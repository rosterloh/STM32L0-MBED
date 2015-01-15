#include "DeviceConfiguration.h"
#include <stdlib.h>
#include <stdio.h>
#include <string.h>

DeviceConfiguration::DeviceConfiguration()
{
  size_t i;

  for (i = 0; i < DEVICE_CONFIGURATION_SIZE; i++) {
    _items[i].key = NULL;
    _items[i].value = NULL;
  }
}

DeviceConfiguration::~DeviceConfiguration()
{
  clear();
}

bool DeviceConfiguration::read(const char *str)
{
  const char *ptr, *ptr2, *ptr3; size_t i, j, len1, len2;
  DeviceConfiguration::KeyValue items[DEVICE_CONFIGURATION_SIZE];

  for (i = 0; i < DEVICE_CONFIGURATION_SIZE; i++) {
    items[i].key = NULL;
    items[i].value = NULL;
  }

  ptr = str;
  i = 0;
  while ((*ptr != '\0') && (i < DEVICE_CONFIGURATION_SIZE)) {
    if (((ptr2 = strchr(ptr, '=')) == NULL) ||
        ((ptr3 = strchr(ptr2+1, ';')) == NULL))
      goto failure;

    len1 = ptr2-ptr;
    len2 = ptr3-ptr2 - 1;

    if ((memchr(ptr, ';', len1) != NULL) ||
        (memchr(ptr2+1, '=', len2) != NULL))
      goto failure;

    for (j = 0; j < DEVICE_CONFIGURATION_SIZE; j++) {
      if ((items[j].key != NULL) && (strlen(items[j].key) == len1) && (strncmp(items[j].key, ptr, len1) == 0))
        goto failure;
    }

    if ((items[i].key = (char*)malloc(len1+1)) == NULL)
      goto failure;

    if ((items[i].value = (char*)malloc(len2+1)) == NULL) {
      free(items[i].key);
      items[i].key = NULL;
      goto failure;
    }

    strncpy(items[i].key, ptr, len1);
    strncpy(items[i].value, ptr2+1, len2);
    items[i].key[len1] = '\0';
    items[i].value[len2] = '\0';

    i++;
    ptr = ptr3+1;
  }

  if (*ptr != '\0')
    goto failure;

  clear();
  memcpy(_items, items, sizeof(DeviceConfiguration::KeyValue)*DEVICE_CONFIGURATION_SIZE);
  return true;

failure:
  for (i = 0; i < DEVICE_CONFIGURATION_SIZE; i++) {
    if (items[i].key != NULL) {
      free(items[i].key);
      free(items[i].value);
    }
  }

  return false;
}

bool DeviceConfiguration::write(char *buf, size_t len)
{
  char *ptr; size_t i; int ret, ln;

  ptr = buf;
  for (i = 0; i < DEVICE_CONFIGURATION_SIZE; i++) {
    if (_items[i].key == NULL)
      continue;

    ret = snprintf(ptr, len, "%s=%s;%n", _items[i].key, _items[i].value, &ln);
    if ((ret < 0) || (ret >= len))
      return false;

    ptr += ln;
    len -= ln;
  }

  return true;
}

bool DeviceConfiguration::set(const char *key, const char *value)
{
  KeyValue *item; size_t i;

  if ((item = search(key)) == NULL) {
    for (i = 0; (i < DEVICE_CONFIGURATION_SIZE) && (item == NULL); i++) {
      if (_items[i].key == NULL)
        item = &_items[i];
    }
  }

  if (item == NULL)
    return false;

  if ((item->key = (char*)malloc(strlen(key)+1)) == NULL)
    return false;

  if ((item->value = (char*)malloc(strlen(value)+1)) == NULL) {
    free(item->key);
    item->key = NULL;
    return false;
  }

  strcpy(item->key, key);
  strcpy(item->value, value);
  return true;
}

const char * DeviceConfiguration::get(const char *key)
{
  KeyValue *item;

  if ((item = search(key)) == NULL)
    return NULL;

  return item->value;
}

bool DeviceConfiguration::unset(const char *key)
{
  KeyValue *item;

  if ((item = search(key)) == NULL)
    return false;

  free(item->key);
  free(item->value);
  item->key = NULL;
  item->value = NULL;
  return true;
}

bool DeviceConfiguration::has(const char *key)
{
  return (search(key) != NULL);
}

void DeviceConfiguration::clear()
{
  size_t i;

  for (i = 0; i < DEVICE_CONFIGURATION_SIZE; i++) {
    if (_items[i].key != NULL) {
      free(_items[i].key);
      free(_items[i].value);
    }
  }
}

DeviceConfiguration::KeyValue * DeviceConfiguration::search(const char *key)
{
  size_t i;

  for (i = 0; i < DEVICE_CONFIGURATION_SIZE; i++) {
    if (strcmp(key, _items[i].key) == 0)
      return &_items[i];
  }

  return NULL;
}
