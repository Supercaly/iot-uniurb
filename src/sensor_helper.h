#ifndef SENSOR_HELPER_H
#define SENSOR_HELPER_H

#include <Arduino.h>
#include <assert.h>

#include "sensor/DHT11_sensor.h"
#include "sensor/ENS160_sensor.h"
#include "sensor/MHZ19_sensor.h"
#include "sensor/SGP30_sensor.h"
#include "sensor/SPS30_sensor.h"
#include "sensor/sensor.h"
#include "utils.h"

// Measure task config.
#define MEASURE_TASK_STACK_SIZE 4096
#define MEASURE_TASK_PRIORITY   8
#define MEASURE_TASK_CORE       tskNO_AFFINITY

static AbstractSensor *sensor_type_to_impl[]
    = {&DHT11Sensor, &SGP30Sensor, &MHZ19Sensor, &SPS30Sensor, &ENS160Sensor};
static_assert(size_of_array(sensor_type_to_impl) == SensorType::COUNT_SENSORS,
              "The number of elements of sensor_type_to_impl have changed. "
              "You probably have added or removed a sensor. "
              "Please update the definitions above accordingly");

/*
 * Helper method to automatically init all sensors
 * if they are available.
 *
 * Returns true if all available sensors are initialized,
 * false if at least one available sensor cannot be initialized.
 */
bool init_available_sensors();

/*
 * Helper method to automatically measure all available sensors.
 *
 * Returns true if all available sensors are measured, false
 * if at least one available sensor cannot be measured.
 */
bool measure_available_sensors();

/*
 * Function representing a task executing inside the board
 * with the purpose of measuring data from all the available
 * sensors and sending it to InfluxDB.
 */
void measure_and_send_task_code(void *args);

#endif // SENSOR_HELPER_H
