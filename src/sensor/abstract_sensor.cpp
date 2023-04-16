#include "abstract_sensor.h"

bool AbstractSensor::init() {
  if (p_is_init) {
    return true;
  }

  bool init_res = on_init();
  p_is_init = init_res;
  return init_res;
}

bool AbstractSensor::measure() {
  if (!p_is_init) {
    return true;
  }

  return on_measure();
}
