#pragma once

#include <stddef.h>
#include "mbed.h"
#include "rtos.h"

class DeviceFeedback
{
public:
  DeviceFeedback(PwmOut speaker);

  void beepSuccess();
  void beepFailure();
  void closeRelay();
  void openRelay();

protected:
  void sendMessage(uint8_t);
  void thread();
  static void thread_func(void const*);

private:
  PwmOut _speaker;
  Thread _thread;
  Mail<uint8_t, 16> _mail;
};
