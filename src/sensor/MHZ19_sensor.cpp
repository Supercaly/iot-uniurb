#include "MHZ19_sensor.h"
#include "../common.h"

#include "MHZCO2.h"

MHZ19B mhz19;
int mhz_co2 = 0,
    mhz_min_co2 = 0,
    mhz_temp = 0,
    mhz_acc = 0;

bool MHZ19_init() {
  Log.traceln("MHZ19_init: sensor initialized");
  MHZ19_SERIAL.begin(MHZ19_BAUD_RATE, SERIAL_8N1, MHZ19_RX, MHZ19_TX);
  mhz19.begin(&MHZ19_SERIAL);
  delay(MHZ19_INIT_DELAY_MS);
  return true;
}

void MHZ19_read() {
  int currentCo2 = 0,
      currentMinCo2 = 0,
      currentTemp = 0,
      currentAcc = 0,
      minCo2 = 0,
      minMinCo2 = 0,
      minTemp = 0,
      minAcc = 0,
      maxCo2 = 0,
      maxMinCo2 = 0,
      maxTemp = 0,
      maxAcc = 0;

  for (int i = 0; i < SENSOR_AVG_WINDOW; ++i) {
    mhz19.measure();
    currentCo2 = mhz19.getCO2();
    currentMinCo2 = mhz19.getMinCO2();
    currentTemp = mhz19.getTemperature();
    currentAcc = mhz19.getAccuracy();

    if (currentCo2 < minCo2) {
      minCo2 = currentCo2;
    }
    if (currentCo2 > maxCo2) {
      maxCo2 = currentCo2;
    }
    if (currentMinCo2 < minMinCo2) {
      minMinCo2 = currentMinCo2;
    }
    if (currentMinCo2 > maxMinCo2) {
      maxMinCo2 = currentMinCo2;
    }
    if (currentTemp < minTemp) {
      minTemp = currentTemp;
    }
    if (currentTemp > maxTemp) {
      maxTemp = currentTemp;
    }
    if (currentAcc < minAcc) {
      minAcc = currentAcc;
    }
    if (currentAcc > maxAcc) {
      maxAcc = currentAcc;
    }

    mhz_co2 += currentCo2;
    mhz_min_co2 += currentMinCo2;
    mhz_temp += currentTemp;
    mhz_acc += currentAcc;
    delay(SENSOR_AVG_DELAY_MS);
  }
  mhz_co2 -= minCo2;
  mhz_co2 -= maxCo2;
  mhz_min_co2 -= minMinCo2;
  mhz_min_co2 -= maxMinCo2;
  mhz_temp -= minTemp;
  mhz_temp -= maxTemp;
  mhz_acc -= minAcc;
  mhz_acc -= maxAcc;

  mhz_co2 /= (SENSOR_AVG_WINDOW - 2);
  mhz_min_co2 /= (SENSOR_AVG_WINDOW - 2);
  mhz_temp /= (SENSOR_AVG_WINDOW - 2);
  mhz_acc /= (SENSOR_AVG_WINDOW - 2);

#ifdef PRINT_SENSORS_ON_READ
  Log.debugln("MHZ19 Co2: " + String(mhz_co2));
  Log.debugln("MHZ19 Min Co2: " + String(mhz_min_co2));
  Log.debugln("MHZ19 Temperature: " + String(mhz_temp));
  Log.debugln("MHZ19 Accuracy: " + String(mhz_acc));
#endif  // PRINT_SENSORS_ON_READ
}

int MHZ19_get_co2() {
  return mhz_co2;
}
int MHZ19_get_minCo2() {
  return mhz_min_co2;
}
int MHZ19_get_temperature() {
  return mhz_temp;
}
int MHZ19_get_accuracy() {
  return mhz_acc;
}
