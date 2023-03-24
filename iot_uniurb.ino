#include "common.h"

void setup() {
  LOGGER_SERIAL.begin(BAUD_RATE);
  LOGGER_SERIAL.println("Hello");
}

void loop() {
  
}
