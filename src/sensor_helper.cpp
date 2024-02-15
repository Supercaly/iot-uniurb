#include "sensor_helper.h"

#include <Arduino.h>

#include "board_preference.h"
#include "log.h"
#include "network/influxdb.h"
#include "sensor/sensor.h"
#include "utils.h"

bool init_available_sensors() {
  app_traceln("init_available_sensors: initializing all available sensors");
  bool has_errors = false;
  for (int i = 0; i < COUNT_SENSORS; i++) {
    if (Preference.has_sensor((SensorType)i)) {
      if (!sensor_type_to_impl[i]->init()) {
        app_errorln("init_available_sensors: cannot initialize sensor"
                    + sensor_type_to_string((SensorType)i));
        has_errors = true;
      }
    }
  }
  if (!has_errors) {
    app_debugln("init_available_sensors: all available sensors initialized");
  }
  return has_errors;
}

bool measure_available_sensors() {
  app_traceln("measure_available_sensors: measuring all available sensors");
  bool has_errors = false;
  for (int i = 0; i < COUNT_SENSORS; i++) {
    if (Preference.has_sensor((SensorType)i)) {
      if (!sensor_type_to_impl[i]->measure()) {
        app_errorln("measure_available_sensors: cannot measure sensor"
                    + sensor_type_to_string((SensorType)i));
        has_errors = true;
      }
    }
  }
  if (!has_errors) {
    app_debugln("measure_available_sensors: all available sensors measured");
  }
  return has_errors;
}

void measure_and_send_task_code(void *args) {
  app_traceln("measure_and_send_task_code: starting task '" + String(MEASURE_TASK_NAME) + "'");
  TickType_t last_measure_time = xTaskGetTickCount();
  for (;;) {
    measure_available_sensors();
#ifdef HAS_INFLUXDB
    // Send sensor values to InfluxDB
    if (!influxdb_write_sensors()) {
      app_fatalln("something went wrong writing to InfluxDB");
      reboot_board();
    }
#endif // HAS_INFLUXDB
    xTaskDelayUntil(&last_measure_time, pdMS_TO_TICKS(SENSOR_READING_DELAY_MS));
  }
}
