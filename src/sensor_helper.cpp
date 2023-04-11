#include "sensor_helper.h"
#include "network/influxdb.h"

bool init_all_available_sensors() {
  bool has_errors = false;
  for (int i = 0; i < size_of_array(type_to_sensor_map); i++) {
    SensorTypeToImplPair p = type_to_sensor_map[i];
    if (Preference.has_sensor(p.type)) {
      if (p.sensor == nullptr || !p.sensor->init()) {
        Log.errorln("something went wrong initializing "
                    + SensorType_to_String(p.type));
        has_errors = true;
      }
    }
  }
  return has_errors;
}

bool measure_all_available_sensors() {
  bool has_errors = false;
  for (int i = 0; i < size_of_array(type_to_sensor_map); i++) {
    SensorTypeToImplPair p = type_to_sensor_map[i];
    if (Preference.has_sensor(p.type)) {
      if (p.sensor == nullptr || !p.sensor->measure()) {
        Log.errorln("something went wrong measuring "
                    + SensorType_to_String(p.type));
        has_errors = true;
      }
    }
  }
  return has_errors;
}

void measure_and_send_task_code(void *args) {
  TickType_t last_measure_time = xTaskGetTickCount();
  for (;;) {
    xTaskDelayUntil(
      &last_measure_time,
      pdMS_TO_TICKS(SENSOR_READING_DELAY_MS));

    measure_all_available_sensors();
#ifdef HAS_INFLUXDB
    // Send sensor values to InfluxDB
    if (!influxdb_write_sensors()) {
      Log.fatalln("something went wrong writing to InfluxDB");
      reboot_board();
    }
#endif  // HAS_INFLUXDB
  }
}
