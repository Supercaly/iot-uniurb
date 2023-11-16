#include "button.h"

#include <Arduino.h>

#include "config.h"
#include "log.h"

#ifdef HAS_BUTTON

static int old_button_state = HIGH;

void button_task_code(void *args) {
  pinMode(BUTTON_PIN, INPUT_PULLUP);
  digitalWrite(BUTTON_PIN, HIGH);

  TickType_t last_check = xTaskGetTickCount();
  for (;;) {
    xTaskDelayUntil(&last_check, pdMS_TO_TICKS(1000));
    int btn_state = digitalRead(BUTTON_PIN);
    // TODO: Switch the WiFi connection back once the jumper is unset.
    if (old_button_state != btn_state && btn_state == LOW) {
      app_infoln("button_task_code: button pressed!");
    }
    old_button_state = btn_state;
  }
}

#endif // HAS_BUTTON
