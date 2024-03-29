#include "ENS160_sensor.h"

#include <Adafruit_AHTX0.h>
#include <Arduino.h>
#include <ScioSense_ENS160.h>

#include "../board_preference.h"
#include "../config.h"
#include "../log.h"

ENS160_Sensor ENS160Sensor;

ENS160_Sensor::ENS160_Sensor() : _ens(ENS160_I2CADDR_1) {}

bool ENS160_Sensor::on_init() {
  app_traceln("ENS160_Sensor::on_init: initializing sensor");

  if (!_ens.begin()) {
    app_errorln("ENS160_Sensor::on_init: ENS160 sensor not found");
    return false;
  }
  if (!_ens.available()) {
    app_errorln("ENS160_Sensor::on_init: sensor not available");
    return false;
  }
  if (!_aht.begin()) {
    app_errorln("ENS160_Sensor::on_init: AHT2x sensor not found");
    return false;
  }

  _ens.setMode(ENS160_OPMODE_STD);
  app_traceln("ENS160_Sensor::on_init: sensor initialized");
  return true;
}

bool ENS160_Sensor::on_measure() {
  app_traceln("ENS160_Sensor::on_measure: reading sensor values");
  sensors_event_t temp, hum;

  _ens.measure(true);
  _eco2 = (float)_ens.geteCO2();
  _tvoc = (float)_ens.getTVOC();

  _aht.getEvent(&hum, &temp);
  _temperature = temp.temperature;
  _humidity    = hum.relative_humidity;

  // Remove offset from data
  SensorOffsets so;
  Preference.get_sensor_offsets(&so);
  app_debugln("ENS160_Sensor::on_measure: using offset of " + String(so.temperature)
              + " for real temperature of" + String(_temperature));
  _temperature += so.temperature;
  app_debugln("ENS160_Sensor::on_measure: using offset of " + String(so.humidity)
              + " for real humidity of" + String(_humidity));
  _humidity += so.humidity;
  app_debugln("ENS160_Sensor::on_measure: using offset of " + String(so.eco2) + " for real eCO2 of"
              + String(_eco2));
  _eco2 += so.eco2;
  app_debugln("ENS160_Sensor::on_measure: using offset of " + String(so.tvoc) + " for real TVOC of"
              + String(_tvoc));
  _tvoc += so.tvoc;

#ifdef PRINT_SENSORS_ON_READ
  app_infoln("ENS160+AHT2X eCO2: " + String(_eco2));
  app_infoln("ENS160+AHT2X TVOC: " + String(_tvoc));
  app_infoln("ENS160+AHT2X temperature: " + String(_temperature));
  app_infoln("ENS160+AHT2X humidity: " + String(_humidity));
#endif // PRINT_SENSORS_ON_READ

  return true;
}

void ENS160_Sensor::print_info(sensor_print_cb_t p) {
  p("ENS160+AHT2X eCO2: " + String(_eco2));
  p("ENS160+AHT2X TVOC: " + String(_tvoc));
  p("ENS160+AHT2X temperature: " + String(_temperature));
  p("ENS160+AHT2X humidity: " + String(_humidity));
}

void ENS160_Sensor::to_influx(Point *p) {
  p->addField(INFLUXDB_FIELD_ENS160_TEMPERATURE, _temperature);
  p->addField(INFLUXDB_FIELD_ENS160_HUMIDITY, _humidity);
  p->addField(INFLUXDB_FIELD_ENS160_ECO2, _eco2);
  p->addField(INFLUXDB_FIELD_ENS160_TVOC, _tvoc);
}
