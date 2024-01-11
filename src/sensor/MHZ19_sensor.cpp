#include "MHZ19_sensor.h"

#include <Arduino.h>
#include <WString.h>

#include "../config.h"
#include "../log.h"

MHZ19_Sensor MHZ19Sensor;

bool MHZ19_Sensor::on_init() {
  MHZ19_SERIAL.begin(MHZ19_BAUD_RATE, SERIAL_8N1, MHZ19_RX, MHZ19_TX);
  _mhz.begin(&MHZ19_SERIAL);
  _mhz.setPPM(5000);

  delay(MHZ19_INIT_DELAY_MS);
  app_traceln("MHZ19_Sensor::init: sensor initialized");
  return true;
}

bool MHZ19_Sensor::on_measure() {
  int currentCo2 = 0, minCo2 = 0, maxCo2 = 0;

  app_traceln("MHZ19_Sensor::measure: reading sensor values");
  int i = 0, k = 0;
  while (i < SENSOR_AVG_WINDOW && k < 100) {
    // for (int i = 0; i < SENSOR_AVG_WINDOW; ++i) {
    int err    = _mhz.measure();
    currentCo2 = _mhz.getCO2();

    if (err == MHZCO2_OK && currentCo2 <= 5500) {
      if (currentCo2 < minCo2) {
        minCo2 = currentCo2;
      }
      if (currentCo2 > maxCo2) {
        maxCo2 = currentCo2;
      }
      _co2 += currentCo2;
      i++;
    }
    k++;

    delay(SENSOR_AVG_DELAY_MS);
  }
  if (i > 2) {
    _co2 -= minCo2;
    _co2 -= maxCo2;
    _co2 /= (i - 2);
  }

#ifdef PRINT_SENSORS_ON_READ
  app_infoln("MHZ19 Co2: " + String(_co2));
#endif // PRINT_SENSORS_ON_READ

  return true;
}
