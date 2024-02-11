#include "sensor_helper.h"

#include <Arduino.h>

#include "board_preference.h"
#include "log.h"
#include "network/influxdb.h"
#include "sensor/DHT11_sensor.h"
#include "sensor/ENS160_sensor.h"
#include "sensor/MHZ19_sensor.h"
#include "sensor/SGP30_sensor.h"
#include "sensor/SPS30_sensor.h"
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

void print_available_sensors_info(void (*print)(String)) {
  if (Preference.has_sensor(SensorType::SENSOR_DHT11)) {
    print("DHT11 Temperature: " + String(DHT11Sensor.get_temperature()));
    print("DHT11 Humidity: " + String(DHT11Sensor.get_humidity()));
  }
  if (Preference.has_sensor(SensorType::SENSOR_SGP30)) {
    print("SGP30 TVOC: " + String(SGP30Sensor.get_TVOC()));
    print("SGP30 eCO2: " + String(SGP30Sensor.get_eCO2()));
  }
  if (Preference.has_sensor(SensorType::SENSOR_SPS30)) {
    print("SPS30 MC 1.0: " + String(SPS30Sensor.get_mc_1p0()));
    print("SPS30 MC 2.5: " + String(SPS30Sensor.get_mc_2p5()));
    print("SPS30 MC 4.0: " + String(SPS30Sensor.get_mc_4p0()));
    print("SPS30 MC 10.0: " + String(SPS30Sensor.get_mc_10p0()));
    print("SPS30 Particle Size: " + String(SPS30Sensor.get_particle_size()));
  }
  if (Preference.has_sensor(SensorType::SENSOR_MHZ19)) {
    print("MHZ19 CO2: " + String(MHZ19Sensor.get_co2()));
  }
  if (Preference.has_sensor(SensorType::SENSOR_ENS160)) {
    print("ENS160 Temperature: " + String(ENS160Sensor.get_temperature()));
    print("ENS160 Humidity: " + String(ENS160Sensor.get_humidity()));
    print("ENS160 TVOC: " + String(ENS160Sensor.get_TVOC()));
    print("ENS160 eCO2: " + String(ENS160Sensor.get_eCO2()));
  }
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
