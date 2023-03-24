#include "SGP30_sensor.h"
#include "../common.h"
#include "DHT11_sensor.h"

#include "Adafruit_SGP30.h"

Adafruit_SGP30 sgp;
uint16_t sgp_tvoc = 0.0,
         sgp_eco2 = 0.0,
         sgp_raw_h2 = 0.0,
         sgp_raw_ethanol = 0.0;

/*
 * Return the absolute humidity from given temperature and humidity.
 */
static uint32_t get_absolute_humidity(float temperature, float humidity) {
  const float absoluteHumidity = 216.7f * ((humidity / 100.0f) * 6.112f * exp((17.62f * temperature) / (243.12f + temperature)) / (273.15f + temperature));
  const uint32_t absoluteHumidityScaled = static_cast<uint32_t>(1000.0f * absoluteHumidity);
  return absoluteHumidityScaled;
}

bool SGP30_init() {
  if (!sgp.begin()) {
    Log.errorln("SGP30_init: sensor not found");
    return false;
  }
  delay(SGP30_INIT_DELAY_MS);
  Log.traceln("SGP30_init: sensor initialized");
  return true;
}

void SGP30_read() {
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

  Log.traceln("SGP30_read: reading sensor values");
  if (DHT11_get_temperature() > 0) {
    Log.traceln("SGP30_read: compute absolute humidity with real temperature and humidity");
    sgp.setHumidity(get_absolute_humidity(DHT11_get_temperature(), DHT11_get_humidity()));
  } else {
    Log.traceln("SGP30_read: compute absolute humidity with default temperature and humidity");
    sgp.setHumidity(get_absolute_humidity(22.1, 45.2));
  }

  for (int i = 0; i < SENSOR_AVG_WINDOW; ++i) {
    sgp.IAQmeasure();
    sgp.IAQmeasureRaw();
    currentTvoc = sgp.TVOC;
    currentEco2 = sgp.eCO2;
    currentH2 = sgp.rawH2;
    currentEthanol = sgp.rawEthanol;

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

    sgp_tvoc += currentTvoc;
    sgp_eco2 += currentEco2;
    sgp_raw_h2 += currentH2;
    sgp_raw_ethanol += currentEthanol;
    delay(SENSOR_AVG_DELAY_MS);
  }
  sgp_tvoc -= minTvoc;
  sgp_tvoc -= maxTvoc;
  sgp_eco2 -= minEco2;
  sgp_eco2 -= maxEco2;
  sgp_raw_h2 -= minH2;
  sgp_raw_h2 -= maxH2;
  sgp_raw_ethanol -= minEthanol;
  sgp_raw_ethanol -= maxEthanol;

  sgp_tvoc /= (SENSOR_AVG_WINDOW - 2);
  sgp_eco2 /= (SENSOR_AVG_WINDOW - 2);
  sgp_raw_h2 /= (SENSOR_AVG_WINDOW - 2);
  sgp_raw_ethanol /= (SENSOR_AVG_WINDOW - 2);

#ifdef PRINT_SENSORS_ON_READ
  Log.debugln("SGP30 TVOC: " + String(sgp_tvoc));
  Log.debugln("SGP30 eCO2: " + String(sgp_eco2));
  Log.debugln("SGP30 raw H2: " + String(sgp_raw_h2));
  Log.debugln("SGP30 raw Ethanol: " + String(sgp_raw_ethanol));
#endif  // PRINT_SENSORS_ON_READ
}

int SGP30_get_TVOC() {
  return (int)sgp_tvoc;
}

int SGP30_get_eCO2() {
  return (int)sgp_eco2;
}

int SGP30_get_raw_H2() {
  return (int)sgp_raw_h2;
}

int SGP30_get_raw_ethanol() {
  return (int)sgp_raw_ethanol;
}
