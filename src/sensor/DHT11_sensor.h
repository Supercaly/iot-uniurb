#ifndef DHT11_SENSOR_H
#define DHT11_SENSOR_H

#include "AbstractSensor.h"

#include <DHT.h>

// Delay in milliseconds for the sensor init process.
#define DHT11_INIT_DELAY_MS 300

class DHT11_Sensor : public AbstractSensor {
public:
  DHT11_Sensor();

  bool init() override;
  bool measure() override;

  /*
   * Return the last measured temperature.
   */
  float get_temperature() const {
    return _temperature;
  };

  /*
   * Return the last measured humidity.
   */
  float get_humidity() const {
    return _humidity;
  }

private:
  DHT _dht;
  float _temperature;
  float _humidity;
};

/*
 * Global instance of DHT11_Sensor.
 */
extern DHT11_Sensor DHT11Sensor;

#endif  // DHT11_SENSOR_H
