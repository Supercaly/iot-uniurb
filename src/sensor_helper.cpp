#include "sensor_helper.h"

#include <Arduino.h>

#include "board_preference.h"
#include "log.h"
#include "network/influxdb.h"
#include "sensor/sensor.h"
#include "utils.h"

bool init_available_sensors() {
  bool has_errors = false;
  for (int i = 0; i < COUNT_SENSORS; i++) {
    if (Preference.has_sensor((SensorType)i)) {
      if (!sensor_type_to_impl[i]->init()) {
        app_errorln("something went wrong initializing sensor"
                    + sensor_type_to_string((SensorType)i));
        has_errors = true;
      }
    }
  }
  return has_errors;
}

bool measure_available_sensors() {
  bool has_errors = false;
  for (int i = 0; i < COUNT_SENSORS; i++) {
    if (Preference.has_sensor((SensorType)i)) {
      if (!sensor_type_to_impl[i]->measure()) {
        app_errorln("something went wrong measuring sensor" + sensor_type_to_string((SensorType)i));
        has_errors = true;
      }
    }
  }
  return has_errors;
}

void measure_and_send_task_code(void *args) {
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
