#include "SPS30_sensor.h"

#include <Arduino.h>
#include <sps30.h>

#include "../board_preference.h"
#include "../config.h"
#include "../log.h"

SPS30_Sensor SPS30Sensor;

bool SPS30_Sensor::on_init() {
  int16_t ret;

  sensirion_i2c_init();

  int probe_attempt = 0;
  while (sps30_probe() != 0 && probe_attempt < SPS30_MAX_PROBE_RETRY) {
    app_traceln("SPS30_Sensor::init: sensor probe failed... retrying");
    delay(SPS30_PROBE_RETRY_DELAY_MS);
    probe_attempt++;
  }

  // If probe failed return false
  if (probe_attempt >= SPS30_MAX_PROBE_RETRY) {
    app_errorln("SPS30_Sensor::init: max probe retry reached");
    return false;
  }

  app_traceln("SPS30_Sensor::init: set fan auto-cleaning days to "
              + String(SPS30_FAN_AUTO_CLEAN_DAYS));
  ret = sps30_set_fan_auto_cleaning_interval_days(SPS30_FAN_AUTO_CLEAN_DAYS);
  if (ret) {
    app_errorln("SPS30_Sensor::init: error setting auto-cleaning interval: " + String(ret));
    return false;
  }

  app_traceln("SPS30_Sensor::init: start measurement");
  ret = sps30_start_measurement();
  if (ret < 0) {
    app_errorln("SPS30_Sensor::init: error starting measurement");
    return false;
  }

  delay(SPS30_INIT_DELAY_MS);
  app_traceln("SPS30_Sensor::init: sensor initialized");
  return true;
}

bool SPS30_Sensor::on_measure() {
  int16_t  ret;
  uint16_t data_ready;

  app_traceln("SPS30_Sensor::measure: reading sensor values");

  ret = sps30_read_data_ready(&data_ready);
  if (ret < 0) {
    app_errorln("SPS30_Sensor::measure: error reading data-ready flag");
    return false;
  }
  if (!data_ready) {
    app_infoln("SPS30_Sensor::measure: no new measurement available for reading");
  }

  ret = sps30_read_measurement(&_sps_meas);
  if (ret < 0) {
    app_errorln("SPS30_Sensor::measure: error reading measurement");
    return false;
  }

  // Remove offset from data
  SensorOffsets so;
  Preference.get_sensor_offsets(&so);
  app_traceln("SGP30_Sensor::measure: using offset of " + String(so.pm10) + " for real PM 10 of "
              + String(_sps_meas.mc_10p0));
  _sps_meas.mc_10p0 += so.pm10;

#ifdef PRINT_SENSORS_ON_READ
  app_infoln("SPS30 PM 1.0: " + String(_sps_meas.mc_1p0));
  app_infoln("SPS30 PM 2.5: " + String(_sps_meas.mc_2p5));
  app_infoln("SPS30 PM 4.0: " + String(_sps_meas.mc_4p0));
  app_infoln("SPS30 PM 10.0: " + String(_sps_meas.mc_10p0));
#endif // PRINT_SENSORS_ON_READ

  return true;
}
