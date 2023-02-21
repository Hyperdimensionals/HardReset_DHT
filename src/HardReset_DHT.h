/*!
 *  @file HardReset_DHT.h
 *
 *  Resets DHT (or other) sensor by cycling the on/off state of a specified VCC pin.
 *
 *  Written by Brendan Krueger
 *
 *  MIT license, all text above must be included in any redistribution
 */

#ifndef HardReset_DHT_h
#define HardReset_DHT_h

#include "Arduino.h"

/*! 
 *  @brief  Reset of DHT (or other) sensor by cycling the state of a specified VCC pin.
 */
class HardReset {
  public:
  HardReset(uint8_t VCC_pin, uint16_t off_interval = 2100, uint8_t max_tries = 0);
  void begin();
  void resetVCC(float reading);
  uint8_t resets; // Keeps track of reset attempts.
                  // This shows total resets since the library was intiated,
                  // successful or not successful.
 private:
  uint8_t _VCC_pin, _try_count, _max_tries;
  uint16_t _off_interval = 2100;
  uint32_t _ms_last_reset = 0;
  bool _VCC_on = true;
};

#endif
