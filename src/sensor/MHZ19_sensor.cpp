#include "MHZ19_sensor.h"

#include <Arduino.h>
#include <MHZCO2.h>

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
  int totalCo2 = 0;
  int j        = 0;

  app_traceln("MHZ19_Sensor::measure: reading sensor values");
  for (int i = 0; i < MHZ19_AVG_NUM; i++) {
    _mhz.measure();
    int currentCo2 = _mhz.getCO2();
    app_fatalln(String(currentCo2));

    if (currentCo2 < MHZ19_MAX_CO2) {
      totalCo2 += currentCo2;
      j++;
    }
    delay(MHZ19_SAMPLING_MS);
  }

  if (j > 0) {
    totalCo2 /= j;
  }
  _co2 = totalCo2;

#ifdef PRINT_SENSORS_ON_READ
  app_infoln("MHZ19 Co2: " + String(_co2));
#endif // PRINT_SENSORS_ON_READ

  return true;
}
