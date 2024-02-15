#include "SGP30_sensor.h"

#include <Adafruit_SGP30.h>
#include <Arduino.h>

#include "../board_preference.h"
#include "../config.h"
#include "../log.h"

SGP30_Sensor SGP30Sensor;

bool SGP30_Sensor::on_init() {
  app_traceln("SGP30_Sensor::on_init: initializing sensor");
  if (!_sgp.begin()) {
    app_errorln("SGP30_Sensor::on_init: sensor not found");
    return false;
  }
  delay(SGP30_INIT_DELAY_MS);
  app_traceln("SGP30_Sensor::on_init: sensor initialized");
  return true;
}

bool SGP30_Sensor::on_measure() {
  uint16_t totalEco2 = 0, totalTvoc = 0;
  int      j = 0, k = 0;

  app_traceln("SGP30_Sensor::on_measure: reading sensor values");
  for (int i = 0; i < SGP30_AVG_NUM; i++) {
    _sgp.IAQmeasure();
    uint16_t currentTvoc = _sgp.TVOC;
    uint16_t currentEco2 = _sgp.eCO2;

    if (currentEco2 < SGP30_MAX_ECO2) {
      totalEco2 += currentEco2;
      j++;
    }

    if (currentTvoc < SGP30_MAX_TVOC) {
      totalTvoc += currentTvoc;
      k++;
    }
    delay(SGP30_SAMPLING_MS);
  }

  if (j > 0) {
    totalEco2 /= j;
  }
  if (k > 0) {
    totalTvoc /= k;
  }
  _tvoc = totalTvoc;
  _eco2 = totalEco2;

  // Remove offset from data
  SensorOffsets so;
  Preference.get_sensor_offsets(&so);
  app_debugln("SGP30_Sensor::on_measure: using offset of " + String(so.eco2) + " for real eCO2 of "
              + String(_eco2));
  _eco2 += so.eco2;
  app_debugln("SGP30_Sensor::on_measure: using offset of " + String(so.tvoc) + " for real TVOC of "
              + String(_tvoc));
  _tvoc += so.tvoc;

#ifdef PRINT_SENSORS_ON_READ
  app_infoln("SGP30 TVOC: " + String(_tvoc));
  app_infoln("SGP30 eCO2: " + String(_eco2));
#endif // PRINT_SENSORS_ON_READ

  return true;
}

void SGP30_Sensor::print_info(sensor_print_cb_t p) {
  p("SGP30 TVOC: " + String(_tvoc));
  p("SGP30 eCO2: " + String(_eco2));
}

void SGP30_Sensor::to_influx(Point *p) {
  p->addField(INFLUXDB_FIELD_SGP30_TVOC, _tvoc);
  p->addField(INFLUXDB_FIELD_SGP30_ECO2, _eco2);
}