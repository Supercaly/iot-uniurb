#include "DHT11_sensor.h"

#include <Arduino.h>
#include <DHT.h>
#include <WString.h>

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
  float currentT = 0.0, currentH = 0.0, maxT = 0.0, minT = 0.0, maxH = 0.0, minH = 0.0;

  app_traceln("DHT11_Sensor::measure: reading sensor values");
  _temperature = 0.0;
  _humidity    = 0.0;
  for (int i = 0; i < SENSOR_AVG_WINDOW; ++i) {
    currentT = _dht.readTemperature();
    currentH = _dht.readHumidity();

    if (currentT < minT) {
      minT = currentT;
    }
    if (currentT > maxT) {
      maxT = currentT;
    }
    if (currentH < minH) {
      minH = currentH;
    }
    if (currentH > maxH) {
      maxH = currentH;
    }

    _temperature += currentT;
    _humidity += currentH;
    delay(SENSOR_AVG_DELAY_MS);
  }
  _temperature -= minT;
  _temperature -= maxT;
  _humidity -= minH;
  _humidity -= maxH;

  _temperature /= (SENSOR_AVG_WINDOW - 2);
  _humidity /= (SENSOR_AVG_WINDOW - 2);

  // Remove temperature offset from data
  app_traceln("DHT11_Sensor::measure: using offset of "
              + String(Preference.get_temperature_offset()) + " for real temperature of "
              + String(_temperature));
  _temperature -= Preference.get_temperature_offset();

#ifdef PRINT_SENSORS_ON_READ
  app_infoln("DHT11 temperature: " + String(_temperature));
  app_infoln("DHT11 humidity: " + String(_humidity));
#endif // PRINT_SENSORS_ON_READ

  return true;
}
