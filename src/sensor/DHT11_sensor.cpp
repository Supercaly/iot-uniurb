#include "DHT11_sensor.h"
#include "../common.h"

DHT11_Sensor DHT11Sensor;

DHT11_Sensor::DHT11_Sensor()
  : _dht(DHT11_PIN, DHT11),
    _temperature(0.0),
    _humidity(0.0) {}

bool DHT11_Sensor::init() {
  if (p_is_init) {
    Log.traceln("DHT11_Sensor::init: sensor already initialized");
    return true;
  }

  _dht.begin();
  delay(DHT11_INIT_DELAY_MS);
  Log.traceln("DHT11_Sensor::init: sensor initialized");

  p_is_init = true;
  return true;
}

bool DHT11_Sensor::measure() {
  float currentT = 0.0,
        currentH = 0.0,
        maxT = 0.0,
        minT = 0.0,
        maxH = 0.0,
        minH = 0.0;

  if (!p_is_init) {
    Log.errorln("DHT11_Sensor::measure: sensor not initialized");
    return false;
  }

  Log.traceln("DHT11_Sensor::measure: reading sensor values");
  _temperature = 0.0;
  _humidity = 0.0;
  for (int i = 0; i < SENSOR_AVG_WINDOW; ++i) {
    currentT = _dht.readTemperature();
    currentH = _dht.readHumidity();

    if (currentT < minT) {
      minT = currentT;
    }
    if (currentT > maxT) {
      maxT = currentT;
    }
    if (currentH < minH) {
      minH = currentH;
    }
    if (currentH > maxH) {
      maxH = currentH;
    }

    _temperature += currentT;
    _humidity += currentH;
    delay(SENSOR_AVG_DELAY_MS);
  }
  _temperature -= minT;
  _temperature -= maxT;
  _humidity -= minH;
  _humidity -= maxH;

  _temperature /= (SENSOR_AVG_WINDOW - 2);
  _humidity /= (SENSOR_AVG_WINDOW - 2);

#ifdef PRINT_SENSORS_ON_READ
  Log.debugln("DHT11 temperature: " + String(_temperature));
  Log.debugln("DHT11 humidity: " + String(_humidity));
#endif  // PRINT_SENSORS_ON_READ

  return true;
}
