#include "DHT11_sensor.h"

#include <Arduino.h>
#include <DHT.h>

#include "../board_preference.h"
#include "../config.h"
#include "../log.h"

DHT11_Sensor DHT11Sensor;

DHT11_Sensor::DHT11_Sensor() : _dht(DHT11_PIN, DHT11), _temperature(0.0), _humidity(0.0) {}

bool DHT11_Sensor::on_init() {
  app_traceln("DHT11_Sensor::on_init: initializing sensor");
  _dht.begin();
  delay(DHT11_INIT_DELAY_MS);
  app_traceln("DHT11_Sensor::on_init: sensor initialized");
  return true;
}

bool DHT11_Sensor::on_measure() {
  app_traceln("DHT11_Sensor::on_measure: reading sensor values");
  _temperature = _dht.readTemperature();
  _humidity    = _dht.readHumidity();

  // Remove offset from data
  SensorOffsets so;
  Preference.get_sensor_offsets(&so);
  app_debugln("DHT11_Sensor::on_measure: using offset of " + String(so.temperature)
              + " for real temperature of " + String(_temperature));
  _temperature += so.temperature;
  app_debugln("DHT11_Sensor::on_measure: using offset of " + String(so.humidity)
              + " for real humidity of " + String(_humidity));
  _humidity += so.humidity;

#ifdef PRINT_SENSORS_ON_READ
  app_infoln("DHT11 temperature: " + String(_temperature));
  app_infoln("DHT11 humidity: " + String(_humidity));
#endif // PRINT_SENSORS_ON_READ

  return true;
}

void DHT11_Sensor::print_info(sensor_print_cb_t p) {
  p("DHT11 Temperature: " + String(_temperature));
  p("DHT11 Humidity: " + String(_humidity));
}

void DHT11_Sensor::to_influx(Point *p) {
  p->addField(INFLUXDB_FIELD_DHT11_TEMPERATURE, _temperature);
  p->addField(INFLUXDB_FIELD_DHT11_HUMIDITY, _humidity);
}