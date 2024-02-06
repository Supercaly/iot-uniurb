#ifndef SENSOR_HELPER_H
#define SENSOR_HELPER_H

#include <Arduino.h>
#include <WString.h>
#include <assert.h>

#include "sensor/DHT11_sensor.h"
#include "sensor/ENS160_sensor.h"
#include "sensor/MHZ19_sensor.h"
#include "sensor/SGP30_sensor.h"
#include "sensor/SPS30_sensor.h"
#include "sensor/abstract_sensor.h"
#include "sensor/sensor_type.h"
#include "utils.h"

// Measure task config.
#define MEASURE_TASK_STACK_SIZE 4096
#define MEASURE_TASK_PRIORITY   8
#define MEASURE_TASK_CORE       tskNO_AFFINITY

/*
 * Struct that maps SensorType to objects
 * implementing AbstractSensor.
 */
struct SensorTypeToImplPair {
  SensorType      type;
  AbstractSensor *sensor;
};

static const SensorTypeToImplPair type_to_sensor_map[] = {
    {SensorType::SENSOR_DHT11,  &DHT11Sensor },
    {SensorType::SENSOR_SGP30,  &SGP30Sensor },
    {SensorType::SENSOR_MHZ19,  &MHZ19Sensor },
    {SensorType::SENSOR_SPS30,  &SPS30Sensor },
    {SensorType::SENSOR_ENS160, &ENS160Sensor}
};
static_assert(size_of_array(type_to_sensor_map) == SensorType::COUNT_SENSORS,
              "The number of elements of type_to_sensor_map have changed. "
              "You probably have added or removed a sensor. "
              "Please update the definitions above accordingly");

/*
 * Helper method to automatically init all sensors
 * if they are available.
 *
 * Returns true if all available sensors are initialized,
 * false if at least one available sensor cannot be initialized.
 */
bool init_all_available_sensors();

/*
 * Helper method to automatically measure all available sensors.
 *
 * Returns true if all available sensors are measured, false
 * if at least one available sensor cannot be measured.
 */
bool measure_all_available_sensors();

/*
 * Helper method to print automatically info about each
 * available sensor.
 *
 * This function takes as input a callback method to abstract
 * the task of producing the info text from the function
 * that displays it.
 */
void print_available_sensors_info(void (*print)(String));

/*
 * Function representing a task executing inside the board
 * with the purpose of measuring data from all the available
 * sensors and sending it to InfluxDB.
 */
void measure_and_send_task_code(void *args);

#endif // SENSOR_HELPER_H
