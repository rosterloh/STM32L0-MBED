/**
 ******************************************************************************
 * @file    main.cpp
 * @author  Richard Osterloh (richard.osterloh@gmail.com)
 * @version V1.0.0
 * @date    31-October-2014
 * @brief   Project to play around with MBED on the STM32L0 Discovery Board.
 ******************************************************************************
 * Main application configuration.
 ******************************************************************************
 */

#include "mbed.h"
#include "rtos.h"
#include "MDM.h"
#include "GPS.h"
#include "DeviceInfo.h"
#include "DeviceMemory.h"
#include "MbedAgent.h"
#include "GPSTracker.h"

#include "DeviceConfiguration.h"

#include <stdio.h>

/**
* SIM PIN. Null for no pin.
*/
#define SIM_PIN NULL

/**
* SIM GPRS login data. Leave commented out for automatic setting.
*/
//#define SIM_APN ""
//#define SIM_USER ""
//#define SIM_PASS ""

int main() {
  MDMParser::DevStatus devStatus;
  //int res;
  uint8_t status = 0;

  MDMRtos<MDMSerial> mdm;
  GPSI2C gps;

  //mdm.setDebug(4);

  if (!mdm.init(SIM_PIN, &devStatus))
    status = 1;
  else if (!gps.init())
    status = 2;

  DeviceIO io(gps);

  switch (status) {
    case 1:
      io.displayPrint("MODEM INIT FAILURE", "CHECK SIM");
      break;
    case 2:
      io.displayPrint("GPS INIT FAILURE");
      break;
  }

  if (status != 0)
    goto error;

  io.displayPrint("DEVICE INIT");

  io.displayPrint("REGISTER NETWORK", "IMEI", devStatus.imei);

  if (!mdm.registerNet()) {
    io.displayPrint("NETWORK REG ERROR");
    goto error;
  }

  io.displayPrint("JOIN NETWORK");
#ifdef SIM_APN
  if (mdm.join(SIM_APN, SIM_USER, SIM_PASS) == NOIP) {
#else
  if (mdm.join() == NOIP) {
#endif
    io.displayPrint("NETWORK JOIN FAILURE");
    goto error;
  }

  {
    uint8_t tries;
    DeviceInfo deviceInfo(mdm, devStatus);
    DeviceMemory deviceMemory(mdm);

    if (io.userButtonPressed()) {
      if (deviceMemory.resetPlatformCredentials()) {
        //io.deviceFeedback().beepSuccess();
        io.setLED(GREEN, ON);
      } else {
        //io.deviceFeedback().beepFailure();
        io.setLED(RED, ON);
      }
      Thread::wait(1000);
      return 0;
    }

    MbedAgent agent(io, mdm, deviceInfo, deviceMemory);

    io.displayPrint("AGENT INIT");
    if (!agent.init()) {
      io.displayPrint("AGENT INIT FAILURE");
      goto error;
    }

    tries = 3;
    do {
      io.displayPrint("AGENT RUN");
      if (agent.run())
        break;
    } while (--tries > 0);

    if (tries == 0) {
      io.displayPrint("AGENT RUN FAILURE");
      goto error;
    }
  }

  mdm.disconnect();
  return 0;

error:
  mdm.disconnect();
  return 1;
}
