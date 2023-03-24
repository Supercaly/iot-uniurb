#ifndef UTILS_H
#define UTILS_H

#include <Arduino.h>

/*
 * Macro to turn on a LED.
 */
#define LED_ON(led) \
  do { \
    pinMode(led, OUTPUT); \
    digitalWrite(led, HIGH); \
  } while (0)

/*
 * Macro to turn off a LED.
 */
#define LED_OFF(led) \
  do { \
    pinMode(led, OUTPUT); \
    digitalWrite(led, LOW); \
  } while (0)

#endif  // UTILS_H
