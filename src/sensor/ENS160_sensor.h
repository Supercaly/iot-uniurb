#ifndef ENS160_SENSOR_H
#define ENS160_SENSOR_H

#include <Adafruit_AHTX0.h>
#include <Arduino.h>
#include <ScioSense_ENS160.h>

#include "abstract_sensor.h"

class ENS160_Sensor : public AbstractSensor {
  public:
  ENS160_Sensor();

  bool on_init() override;
  bool on_measure() override;

  /*
   * Return the last measured temperature.
   */
  float get_temperature() const { return _temperature; }

  /*
   * Return the last measured humidity.
   */

  float get_humidity() const { return _humidity; }

  /*
   * Return the last measured eCO2.
   */

  float get_eCO2() const { return _eco2; }

  /*
   * Return the last measured TVOC.
   */

  float get_TVOC() const { return _tvoc; }

  private:
  ScioSense_ENS160 _ens;
  Adafruit_AHTX0   _aht;

  float _eco2        = 0;
  float _tvoc        = 0;
  float _temperature = 0.0;
  float _humidity    = 0.0;
};

/*
 * Global instance of ENS160_Sensor.
 */
extern ENS160_Sensor ENS160Sensor;

#endif // ENS160_SENSOR_H