#include "SPS30_sensor.h"
#include "../common.h"

#include <sps30.h>

struct sps30_measurement sps30;

bool SPS30_init() {
  int16_t ret;

  Log.traceln("SPS30_init: initializing sensor");

  sensirion_i2c_init();

  int probe_attempt = 0;
  while (sps30_probe() != 0 && probe_attempt < SPS30_MAX_PROBE_RETRY) {
    Log.traceln("SPS30_init: sensor probe failed... retrying");
    delay(SPS30_PROBE_RETRY_DELAY_MS);
    probe_attempt++;
  }

  // If probe failed return false
  if (probe_attempt >= SPS30_MAX_PROBE_RETRY) {
    Log.errorln("SPS30_init: max probe retry reached");
    return false;
  }

  Log.traceln("SPS30_init: set fan auto-cleaning days to " + String(SPS30_FAN_AUTO_CLEAN_DAYS));
  ret = sps30_set_fan_auto_cleaning_interval_days(SPS30_FAN_AUTO_CLEAN_DAYS);
  if (ret) {
    Log.errorln("SPS30_init: error setting auto-cleaning interval: " + String(ret));
    return false;
  }

  Log.traceln("SPS30_inti: start measurement");
  ret = sps30_start_measurement();
  if (ret < 0) {
    Log.errorln("SPS30_init: error starting measurement");
    return false;
  }

  Log.traceln("SPS30_init: sensor initialized");
  delay(SPS30_INIT_DELAY_MS);
  return true;
}

void SPS30_read() {
  int16_t ret;
  uint16_t data_ready;

  Log.traceln("SPS30_read: reading sensor values");
  delay(1000);
  ret = sps30_read_data_ready(&data_ready);
  if (ret < 0) {
    Log.errorln("SPS30_read: error reading data-ready flag");
    return;
  }
  if (!data_ready) {
    Log.errorln("SPS30_read: no new measurement available for reading");
  }

  // TODO: Figure out if we need to exit in case we cannot read new data or if we can use old data
  ret = sps30_read_measurement(&sps30);
  if (ret < 0) {
    Log.errorln("SPS30_read: error reading measurement");
    return;
  }

#ifdef PRINT_SENSORS_ON_READ
  Log.debugln("SPS30 PM 1.0: " + String(sps30.mc_1p0));
  Log.debugln("SPS30 PM 2.5: " + String(sps30.mc_2p5));
  Log.debugln("SPS30 PM 4.0: " + String(sps30.mc_4p0));
  Log.debugln("SPS30 PM 10.0: " + String(sps30.mc_10p0));
#endif  // PRINT_SENSORS_ON_READ
}

float SPS30_get_mc1p0() {
  return sps30.mc_1p0;
}

float SPS30_get_mc2p5() {
  return sps30.mc_2p5;
}

float SPS30_get_mc4p0() {
  return sps30.mc_4p0;
}

float SPS30_get_mc10p0() {
  return sps30.mc_10p0;
}

float SPS30_get_nc1p0() {
  return sps30.nc_1p0;
}

float SPS30_get_nc2p5() {
  return sps30.nc_2p5;
}

float SPS30_get_nc4p0() {
  return sps30.nc_4p0;
}

float SPS30_get_nc10p0() {
  return sps30.nc_10p0;
}

float SPS30_get_particle_size() {
  return sps30.typical_particle_size;
}
