#include "DHT11_sensor.h"

#include <Arduino.h>
#include <DHT.h>

#include "../board_preference.h"
#include "../config.h"
#include "../log.h"

DHT11_Sensor DHT11Sensor;

DHT11_Sensor::DHT11_Sensor() : _dht(DHT11_PIN, DHT11), _temperature(0.0), _humidity(0.0) {}

bool DHT11_Sensor::on_init() {
  _dht.begin();
  delay(DHT11_INIT_DELAY_MS);
  app_traceln("DHT11_Sensor::init: sensor initialized");
  return true;
}

bool DHT11_Sensor::on_measure() {
  app_traceln("DHT11_Sensor::measure: reading sensor values");
  _temperature = _dht.readTemperature();
  _humidity    = _dht.readHumidity();

  // Remove offset from data
  int16_t temp_offset = Preference.get_temperature_offset();
  int16_t hum_offset  = Preference.get_humidity_offset();
  app_traceln("DHT11_Sensor::measure: using offset of " + String(temp_offset)
              + " for real temperature of " + String(_temperature));
  _temperature += temp_offset;
  app_traceln("DHT11_Sensor::measure: using offset of " + String(hum_offset)
              + " for real humidity of " + String(_humidity));
  _humidity += hum_offset;

#ifdef PRINT_SENSORS_ON_READ
  app_infoln("DHT11 temperature: " + String(_temperature));
  app_infoln("DHT11 humidity: " + String(_humidity));
#endif // PRINT_SENSORS_ON_READ

  return true;
}
