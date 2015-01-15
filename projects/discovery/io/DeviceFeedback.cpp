#include "DeviceFeedback.h"
#include <stdlib.h>
#include <string.h>

#define MSG_BEEP_SUCCESS 1
#define MSG_BEEP_FAILURE 2
#define MSG_CLOSE_RELAY 3
#define MSG_OPEN_RELAY 4

DeviceFeedback::DeviceFeedback(PwmOut speaker) :
    _speaker(speaker),
    _thread(DeviceFeedback::thread_func, this)
{
}

void DeviceFeedback::beepSuccess()
{
  sendMessage(MSG_BEEP_SUCCESS);
}

void DeviceFeedback::beepFailure()
{
  sendMessage(MSG_BEEP_FAILURE);
}

void DeviceFeedback::closeRelay()
{
  sendMessage(MSG_CLOSE_RELAY);
}

void DeviceFeedback::openRelay()
{
  sendMessage(MSG_OPEN_RELAY);
}

void DeviceFeedback::sendMessage(uint8_t msg)
{
  uint8_t *msgPtr;

  msgPtr = _mail.alloc();
  *msgPtr = msg;
  _mail.put(msgPtr);
}

void DeviceFeedback::thread()
{
  osEvent evt; uint8_t *msg;
  bool relayState = false;

  while (true) {
    if ((evt = _mail.get(1000)).status == osEventMail) {
      msg = (uint8_t*)evt.value.p;
      switch (*msg) {
        case MSG_BEEP_SUCCESS:
          for (float i=2000.0; i<10000.0; i+=2000.0) {
            _speaker.period(1.0/i);
            _speaker = 0.5;
            Thread::wait(200);
            _speaker = 0.0;
            Thread::wait(50);
          }
          break;
        case MSG_BEEP_FAILURE:
          for (float i=10000.0; i>2000.0; i-=2000.0) {
            _speaker.period(1.0/i);
            _speaker = 0.5;
            Thread::wait(200);
            _speaker = 0.0;
            Thread::wait(50);
          }
          break;
        case MSG_CLOSE_RELAY:
          if (!relayState) {
            relayState = true;
            for (float i=2000.0; i<10000.0; i+=100) {
              _speaker.period(1.0/i);
              _speaker = 0.5;
              Thread::wait(20);
            }
            _speaker = 0.0;
          }
          break;
        case MSG_OPEN_RELAY:
          if (relayState) {
            relayState = false;
            for (float i=10000.0; i>2000.0; i-=100) {
              _speaker.period(1.0/i);
              _speaker = 0.5;
              Thread::wait(20);
            }
            _speaker = 0.0;
          }
          break;
      }
      _mail.free(msg);
    }
    
    if (relayState) {
      _speaker.period(1.0/10000);
      _speaker = 0.5;
      Thread::wait(20);
      _speaker = 0.0;
    }
  }
}

void DeviceFeedback::thread_func(void const *arg)
{
  DeviceFeedback *that;
  that = (DeviceFeedback*)arg;
  that->thread();
}
