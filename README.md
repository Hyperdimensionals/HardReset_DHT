# DHT Hard Reset Library

## Description

This Arduino library cycles the state of a specified pin on and off (HIGH/LOW) if it recieves NAN (Not A Number) for a specified amount of time. This was written for use with the DHT.h library, to reset the power of a DHT22 sensor when it fails to read. The reset is done by running the sensor's VCC power through a specified Digital/Analog pin instead of through an arduino's power pins.

This library was conceived because my DHT22 sensors would occasionally inexplicably stop reading,and unplugging and plugging them back in brought them back online without having to reset the arduino.

This is a 'brute force' method that doesn't solve any potential issues with code or hardware that may be causing the failures. But this code may be a useful redunancy.

Note: Arduino Digital/Analog pins have very small maximum currents. If using this library with a device besides a DHT sensor, make sure its electrical current is within the Arduino pin's current limits.


The initializer takes 3 arguments:
- VCC_pin: the arduino pin you'd like to power the DHT sensor from.
- off_interval: The amount of time in milliseconds the power will be cycled off. Amounts less than 2001 will be ignored.
- max_tries: number of times reset will be attempted. If reset is successful, tries reset. Set to 0 for infinite.
For instance:
```Cpp
HardReset DHT_hardreset(DHTPIN_power, 4000, 5);
```

## Public Functions

- begin() - Sets VCC pin to an output and sets its state to high
- resetVCC(sensor_reading) - If sensor_reading is NAN, toggles the given VCC power pin on and off.

## Example

```Cpp
#include "DHT.h"  // DHT22 Library
#define DHTTYPE DHT22
#define DHTPIN 3 
DHT dht(DHTPIN, DHTTYPE); 

#include <HardReset_DHT.h>
#define DHTPIN_power 4  // Pin to power DHT22 from

HardReset DHT_hardreset(DHTPIN_power, 4000, 5);
// Define power pin, set reset interval to 4000 ms, and try 5
// times each time reset fails to bring back the sensor

void setup() {
  Serial.begin(9600); 
  
  dht.begin();

  // hard reset's begin() sets DHT power pin to an output 
  // and sets state of that pin to HIGH.
  DHT_hardreset.begin();
}

void loop() {
  delay(2000);

  float DHT22temp;
  DHT22temp = dht.readTemperature();
  // After the DHT sensor is read above, the hard reset library takes the reading
  // and if it is NAN (Not a Number), resets the DHT's power pin:
  DHT_hardreset.resetVCC(DHT22temp);

  Serial.print("Current Temperature Reading: ");
  Serial.println(DHT22temp);
  Serial.print("Number of resets attempted: ");
  Serial.println(DHT_hardreset.resets); 
  //                           /\   
  // Note that this shows total resets since the library was intiated,
  // successful or not successful.
}
```

## Contributing

## Credits

The author and maintainer of this library is Brendan Krueger <>

## License

This library is licensed under [MIT license](https://opensource.org/licenses/MIT).
