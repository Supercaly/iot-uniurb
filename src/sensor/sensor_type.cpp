#include "sensor_type.h"

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
