#include "DHT11_sensor.h"
#include "../common.h"

#include <DHT.h>

DHT dht(DHT11_PIN, DHT11);
float dht_temperature = 0.0,
      dht_humidity = 0.0;

bool DHT11_init() {
  Log.traceln("DHT11_init: sensor initialized");
  dht.begin();
  delay(DHT11_INIT_DELAY_MS);
  return true;
}

void DHT11_read() {
  float currentT = 0.0,
        currentH = 0.0,
        maxT = 0.0,
        minT = 0.0,
        maxH = 0.0,
        minH = 0.0;

  Log.traceln("DHT11_read: reading sensor values");
  dht_temperature = 0.0;
  dht_humidity = 0.0;
  for (int i = 0; i < SENSOR_AVG_WINDOW; ++i) {
    currentT = dht.readTemperature();
    currentH = dht.readHumidity();

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

    dht_temperature += currentT;
    dht_humidity += currentH;
    delay(SENSOR_AVG_DELAY_MS);
  }
  dht_temperature -= minT;
  dht_temperature -= maxT;
  dht_humidity -= minH;
  dht_humidity -= maxH;

  dht_temperature /= (SENSOR_AVG_WINDOW - 2);
  dht_humidity /= (SENSOR_AVG_WINDOW - 2);

#ifdef PRINT_SENSORS_ON_READ
  Log.debugln("DHT11 temperature: " + String(dht_temperature));
  Log.debugln("DHT11 humidity: " + String(dht_humidity));
#endif  // PRINT_SENSORS_ON_READ
}

float DHT11_get_temperature() {
  return dht_temperature;
}

float DHT11_get_humidity() {
  return dht_humidity;
}
