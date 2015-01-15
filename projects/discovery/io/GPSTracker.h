#pragma once

#include <stddef.h>
#include "GPS.h"
#include "rtos.h"

/**
 * A GPS tracker class providing access to the current position.
 */
class GPSTracker
{
public:
  /**
   * Initialize a new GPSTracker object.
   * @param gps a previously initialized instance of the GPSI2C class
   */
  GPSTracker(GPSI2C&);

  typedef struct {
    double altitude;  // altitude  meters
    double latitude;  // latitude  degrees
    double longitude; // longitude degrees
  } Position;

  /**
   * Retrieves and invalidates the current position.
   * @param position a pointer of type Position where the current position is written to
   * @return true on success, false otherwise
   */
  bool position(Position*);

protected:
  void thread();
  static void thread_func(void const*);

private:
  GPSI2C _gps;
  Thread _thread;
  Mutex _mutex;
  Position _position;
  bool _positionSet;
};
