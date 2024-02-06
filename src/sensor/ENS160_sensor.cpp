#include "ENS160_sensor.h"

#include <Adafruit_AHTX0.h>
#include <Arduino.h>
#include <ScioSense_ENS160.h>

#include "../board_preference.h"
#include "../log.h"

ENS160_Sensor ENS160Sensor;

ENS160_Sensor::ENS160_Sensor() : _ens(ENS160_I2CADDR_1) {}

bool ENS160_Sensor::on_init() {
  if (!_ens.begin()) {
    app_errorln("ENS160_Sensor::on_init: sensor not found");
    return false;
  }
  if (!_ens.available()) {
    app_errorln("ENS160_Sensor::on_init: sensor not available");
    return false;
  }
  if (!_aht.begin()) {
    app_errorln("ENS160_Sensor::on_init: sensor not found");
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
  app_traceln("ENS160_Sensor::on_measure: using offset of " + String(so.temperature)
              + " for real temperature of" + String(_temperature));
  _temperature += so.temperature;
  app_traceln("ENS160_Sensor::on_measure: using offset of " + String(so.humidity)
              + " for real humidity of" + String(_humidity));
  _humidity += so.humidity;
  app_traceln("ENS160_Sensor::on_measure: using offset of " + String(so.eco2) + " for real eCO2 of"
              + String(_eco2));
  _eco2 += so.eco2;
  app_traceln("ENS160_Sensor::on_measure: using offset of " + String(so.tvoc) + " for real TVOC of"
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