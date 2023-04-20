#include "sensor_type.h"

#include <assert.h>

String SensorType_to_String(SensorType t) {
  switch (t) {
  case SENSOR_DHT11:
    return "SENSOR_DHT11";
  case SENSOR_SGP30:
    return "SENSOR_SGP30";
  case SENSOR_MHZ19:
    return "SENSOR_MHZ19";
  case SENSOR_SPS30:
    return "SENSOR_SPS30";
  default:
    assert(false && "SensorType_to_String: unreachable");
    exit(1);
  }
}

bool SensorType_by_name(String s, SensorType *t) {
  if (s == "DHT11") {
    *t = SensorType::SENSOR_DHT11;
    return true;
  } else if (s == "SGP30") {
    *t = SensorType::SENSOR_SGP30;
    return true;
  } else if (s == "SPS30") {
    *t = SensorType::SENSOR_SPS30;
    return true;
  } else if (s == "MHZ19") {
    *t = SensorType::SENSOR_MHZ19;
    return true;
  } else {
    return false;
  }
}
