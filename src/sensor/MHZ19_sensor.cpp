#include "MHZ19_sensor.h"

#include <Arduino.h>
#include <MHZCO2.h>

#include "../board_preference.h"
#include "../config.h"
#include "../log.h"

MHZ19_Sensor MHZ19Sensor;

volatile uint16_t pwm_time_high_ms;

static void irq_fn() {
  static uint32_t start = 0;
  int             v     = digitalRead(MHZ19B_PWM_PIN);
  if (v == HIGH) {
    start = millis();
  } else {
    pwm_time_high_ms = millis() - start;
  }
}

static uint16_t measure_co2_pwm() {
  noInterrupts();
  uint16_t high_time_ms = pwm_time_high_ms;
  interrupts();

  uint16_t concentration = round(((high_time_ms - 2) * MHZ19_RANGE) * 0.001);
  return concentration;
}

bool MHZ19_Sensor::on_init() {
  attachInterrupt(digitalPinToInterrupt(MHZ19B_PWM_PIN), irq_fn, CHANGE);

  delay(MHZ19_INIT_DELAY_MS);
  app_traceln("MHZ19_Sensor::init: sensor initialized");
  return true;
}

bool MHZ19_Sensor::on_measure() {
  int totalCo2 = 0;
  int j        = 0;

  app_traceln("MHZ19_Sensor::measure: reading sensor values");
  for (int i = 0; i < MHZ19_AVG_NUM; i++) {
    int currentCo2 = measure_co2_pwm();
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

  // Remove offset from data
  SensorOffsets so;
  Preference.get_sensor_offsets(&so);
  float co2_offset = so.co2 * 0.01;
  app_traceln("MHZ19_Sensor::measure: using offset of " + String(co2_offset) + " for real CO2 of "
              + String(_co2));
  _co2 *= round(co2_offset);

#ifdef PRINT_SENSORS_ON_READ
  app_infoln("MHZ19 Co2: " + String(_co2));
#endif // PRINT_SENSORS_ON_READ

  return true;
}
