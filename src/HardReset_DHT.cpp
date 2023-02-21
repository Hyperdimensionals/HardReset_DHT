/*!
 *  @file HardReset_DHT.cpp
 *
 *  @mainpage Resets DHT (or other) sensor by cycling the on/off state of a specified VCC pin.
 *
 *  @section intro_sec Introduction
 *
 *  This Arduino library cycles the state of a specified pin on and off (HIGH/LOW) 
 *  if it recieves NAN (Not A Number) for a specified amount of time. This was
 *  written for use with the DHT.h library, to reset the power of a DHT22 sensor
 *  when it fails to read. The reset is done by running the sensor's VCC power 
 *  through a specified Digital/Analog pin instead of through an arduino's 
 *  power pins.
 * 
 *  This library was conceived because my DHT22 sensors would occasionally 
 *  inexplicably stop reading,and unplugging and plugging them back in 
 *  brought them back online without having to reset the arduino.
 *
 *  This is a 'brute force' method that doesn't solve any potential issues with
 *  code or hardware that may be causing the failures. 
 *  But this code may be a useful redunancy.
 * 
 *  Note: Arduino Digital/Analog pins have very small maximum currents. 
 *  If using this library with a device besides a DHT sensor, make sure 
 *  its electrical current is within the Arduino pin's current limits.
 *  
 *  @section author Author
 *
 *  Written by Brendan Krueger
 *
 *  @section license License
 *
 *  MIT license, all text above must be included in any redistribution
 */

#include "HardReset_DHT.h"

#define MIN_INTERVAL 2001

/*!
 *  @brief  HardReset constructor.
 *  @param  VCC_pin
 *          Specify sensor's VCC power pin, pin to be disabled and renabled.
 *  @param  off_interval
 *          The amount of time in milliseconds to cycle on/off the power
 *          to VCC pin during reset. Must be greater than 2001, or will default
 *          to MIN_INTERVAL. 2000 ms is the DHT's minimum read time, so the 
 *          minimum interval prevents resets interrupting the DHT library's 
 *          ability to read the sensor.
 *  @param  max_tries
 *          Will attempt reset this many times. 0 for infinite. Tries go back 
 *          to 0 if resetVCC function is passed a real number 
 *          (which would mean the reset was successful).
 */
HardReset::HardReset(uint8_t VCC_pin, uint16_t off_interval, uint8_t max_tries) 
{
  _VCC_pin = VCC_pin;
  if (off_interval > MIN_INTERVAL) {
    _off_interval = off_interval;
  }
  _max_tries = max_tries;

  //Initiate VCC Pin
  pinMode(VCC_pin, OUTPUT);
}

/*!
 *  @brief  Sets VCC pin to an output and sets its state to HIGH
 *          Optional if you don't want state to be HIGH at start as long as 
 *          VCC pin is set to output elseware (also done in constructor).
 */
void HardReset::begin()
{
  pinMode(_VCC_pin, OUTPUT);
  digitalWrite(_VCC_pin, HIGH);
}

/*!
 *  @brief  If reading is NAN, toggles the given VCC power pin on and off.
 *  @param  reading
 *          Sensor reading (float), initiates reset logic if NAN
 */
void HardReset::resetVCC(float reading) 
{
  if (!isnan(reading)) { 
    if (_try_count > 0) {
      _try_count = 0;
    }
    return;
  } else 
  {
    uint32_t current_time = millis();

    if (current_time - _ms_last_reset >= _off_interval) {
      bool tries_available = true;

      if (_max_tries) {
        tries_available = (_try_count < (_max_tries));
      }
      if (tries_available) {
        if (_VCC_on) {
          digitalWrite(_VCC_pin, LOW);
          _VCC_on = false;
        } else {
          digitalWrite(_VCC_pin, HIGH);
          _VCC_on = true;
          _try_count += 1;
          resets += 1;
        }
      }
      _ms_last_reset = current_time;
    }
  }
}
