#include "SGP30_sensor.h"
#include "../common.h"
#include "DHT11_sensor.h"

#include "Adafruit_SGP30.h"

SGP30_Sensor SGP30Sensor;

bool SGP30_Sensor::init() {
  if (p_is_init) {
    Log.traceln("SGP30_Sensor::init: sensor already initialized");
    return true;
  }

  if (!_sgp.begin()) {
    Log.errorln("SGP30_Sensor::init: sensor not found");
    return false;
  }
  delay(SGP30_INIT_DELAY_MS);
  Log.traceln("SGP30_Sensor::init: sensor initialized");

  p_is_init = true;
  return true;
}

bool SGP30_Sensor::measure() {
  uint16_t currentTvoc = 0.0,
           currentEco2 = 0.0,
           currentH2 = 0.0,
           currentEthanol = 0.0,
           minTvoc = 0.0,
           maxTvoc = 0.0,
           minEco2 = 0.0,
           maxEco2 = 0.0,
           minH2 = 0.0,
           maxH2 = 0.0,
           minEthanol = 0.0,
           maxEthanol = 0.0;

  if (!p_is_init) {
    Log.infoln("SGP30_Sensor::measure: sensor not initialized");
    return false;
  }

  Log.traceln("SGP30_Sensor::measure: reading sensor values");
  if (DHT11Sensor.get_temperature() > 0) {
    Log.traceln("SGP30_Sensor::measure: compute absolute humidity with real temperature and humidity");
    _sgp.setHumidity(get_absolute_humidity(DHT11Sensor.get_temperature(), DHT11Sensor.get_humidity()));
  } else {
    Log.traceln("SGP30_Sensor::measure: compute absolute humidity with default temperature and humidity");
    _sgp.setHumidity(get_absolute_humidity(22.1, 45.2));
  }

  for (int i = 0; i < SENSOR_AVG_WINDOW; ++i) {
    _sgp.IAQmeasure();
    _sgp.IAQmeasureRaw();
    currentTvoc = _sgp.TVOC;
    currentEco2 = _sgp.eCO2;
    currentH2 = _sgp.rawH2;
    currentEthanol = _sgp.rawEthanol;

    if (currentTvoc < minTvoc) {
      minTvoc = currentTvoc;
    }
    if (currentTvoc > maxTvoc) {
      maxTvoc = currentTvoc;
    }
    if (currentEco2 < minEco2) {
      minEco2 = currentEco2;
    }
    if (currentEco2 > maxEco2) {
      maxEco2 = currentEco2;
    }
    if (currentH2 < minH2) {
      minEco2 = currentH2;
    }
    if (currentH2 > maxH2) {
      maxH2 = currentH2;
    }
    if (currentEthanol < minEthanol) {
      minEthanol = currentEthanol;
    }
    if (currentEthanol > maxEthanol) {
      maxEthanol = currentEthanol;
    }

    _tvoc += currentTvoc;
    _eco2 += currentEco2;
    _raw_h2 += currentH2;
    _raw_ethanol += currentEthanol;
    delay(SENSOR_AVG_DELAY_MS);
  }
  _tvoc -= minTvoc;
  _tvoc -= maxTvoc;
  _eco2 -= minEco2;
  _eco2 -= maxEco2;
  _raw_h2 -= minH2;
  _raw_h2 -= maxH2;
  _raw_ethanol -= minEthanol;
  _raw_ethanol -= maxEthanol;

  _tvoc /= (SENSOR_AVG_WINDOW - 2);
  _eco2 /= (SENSOR_AVG_WINDOW - 2);
  _raw_h2 /= (SENSOR_AVG_WINDOW - 2);
  _raw_ethanol /= (SENSOR_AVG_WINDOW - 2);

#ifdef PRINT_SENSORS_ON_READ
  Log.debugln("SGP30 TVOC: " + String(_tvoc));
  Log.debugln("SGP30 eCO2: " + String(_eco2));
  Log.debugln("SGP30 raw H2: " + String(_raw_h2));
  Log.debugln("SGP30 raw Ethanol: " + String(_raw_ethanol));
#endif  // PRINT_SENSORS_ON_READ

  return true;
}

uint32_t SGP30_Sensor::get_absolute_humidity(float temperature, float humidity) {
  const float absoluteHumidity = 216.7f * ((humidity / 100.0f) * 6.112f * exp((17.62f * temperature) / (243.12f + temperature)) / (273.15f + temperature));
  const uint32_t absoluteHumidityScaled = static_cast<uint32_t>(1000.0f * absoluteHumidity);
  return absoluteHumidityScaled;
}
