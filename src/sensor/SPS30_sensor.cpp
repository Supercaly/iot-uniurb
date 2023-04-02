#include "SPS30_sensor.h"
#include "../common.h"

#include <sps30.h>

SPS30_Sensor SPS30Sensor;

bool SPS30_Sensor::init() {
  int16_t ret;

  if (p_is_init) {
    Log.traceln("SPS30_Sensor::init: sensor already initialized");
    return true;
  }

  sensirion_i2c_init();

  int probe_attempt = 0;
  while (sps30_probe() != 0 && probe_attempt < SPS30_MAX_PROBE_RETRY) {
    Log.traceln("SPS30_Sensor::init: sensor probe failed... retrying");
    delay(SPS30_PROBE_RETRY_DELAY_MS);
    probe_attempt++;
  }

  // If probe failed return false
  if (probe_attempt >= SPS30_MAX_PROBE_RETRY) {
    Log.errorln("SPS30_Sensor::init: max probe retry reached");
    return false;
  }

  Log.traceln("SPS30_Sensor::init: set fan auto-cleaning days to " + String(SPS30_FAN_AUTO_CLEAN_DAYS));
  ret = sps30_set_fan_auto_cleaning_interval_days(SPS30_FAN_AUTO_CLEAN_DAYS);
  if (ret) {
    Log.errorln("SPS30_Sensor::init: error setting auto-cleaning interval: " + String(ret));
    return false;
  }

  Log.traceln("SPS30_Sensor::init: start measurement");
  ret = sps30_start_measurement();
  if (ret < 0) {
    Log.errorln("SPS30_Sensor::init: error starting measurement");
    return false;
  }

  delay(SPS30_INIT_DELAY_MS);
  Log.traceln("SPS30_Sensor::init: sensor initialized");

  p_is_init = true;
  return true;
}

bool SPS30_Sensor::measure() {
  int16_t ret;
  uint16_t data_ready;

  if (!p_is_init) {
    Log.infoln("SPS30_Sensor::init: sensor not initialized");
    return false;
  }

  Log.traceln("SPS30_Sensor::measure: reading sensor values");
  delay(1000);
  ret = sps30_read_data_ready(&data_ready);
  if (ret < 0) {
    Log.errorln("SPS30_Sensor::measure: error reading data-ready flag");
    return false;
  }
  if (!data_ready) {
    Log.errorln("SPS30_Sensor::measure: no new measurement available for reading");
  }

  ret = sps30_read_measurement(&_sps_meas);
  if (ret < 0) {
    Log.errorln("SPS30_Sensor::measure: error reading measurement");
    return false;
  }

#ifdef PRINT_SENSORS_ON_READ
  Log.debugln("SPS30 PM 1.0: " + String(_sps_meas.mc_1p0));
  Log.debugln("SPS30 PM 2.5: " + String(_sps_meas.mc_2p5));
  Log.debugln("SPS30 PM 4.0: " + String(_sps_meas.mc_4p0));
  Log.debugln("SPS30 PM 10.0: " + String(_sps_meas.mc_10p0));
#endif  // PRINT_SENSORS_ON_READ

  return true;
}
