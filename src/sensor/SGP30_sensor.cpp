#include "SGP30_sensor.h"

#include <Adafruit_SGP30.h>
#include <Arduino.h>
#include <WString.h>

#include "../board_preference.h"
#include "../config.h"
#include "../log.h"

SGP30_Sensor SGP30Sensor;

bool SGP30_Sensor::on_init() {
  if (!_sgp.begin()) {
    app_errorln("SGP30_Sensor::init: sensor not found");
    return false;
  }
  delay(SGP30_INIT_DELAY_MS);
  app_traceln("SGP30_Sensor::init: sensor initialized");
  return true;
}

bool SGP30_Sensor::on_measure() {
  uint16_t totalEco2 = 0, totalTvoc = 0;
  int      j = 0, k = 0;

  app_traceln("SGP30_Sensor::measure: reading sensor values");
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
  int16_t eco2_offset = Preference.get_eco2_offset();
  int16_t tvoc_offset = Preference.get_tvoc_offset();
  app_traceln("SGP30_Sensor::measure: using offset of " + String(eco2_offset) + " for real eCO2 of "
              + String(_eco2));
  _eco2 += eco2_offset;
  app_traceln("SGP30_Sensor::measure: using offset of " + String(tvoc_offset) + " for real TVOC of "
              + String(_tvoc));
  _tvoc += tvoc_offset;

#ifdef PRINT_SENSORS_ON_READ
  app_infoln("SGP30 TVOC: " + String(_tvoc));
  app_infoln("SGP30 eCO2: " + String(_eco2));
#endif // PRINT_SENSORS_ON_READ

  return true;
}
