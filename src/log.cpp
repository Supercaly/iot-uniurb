#include "log.h"

#include <Arduino.h>
#include <WString.h>

#include "config.h"
#include "utils.h"

static bool         s_cr_done = true;
static const String s_log_level_string[]
    = {"[TRACE]  ", "[DEBUG]  ", "[INFO]   ", "[ERROR]  ", "[FATAL]  ", "[SILENT] "};
static_assert(size_of_array(s_log_level_string) == (LOG_LEVEL_SILENT + 1),
              "The size of s_log_level_string has changed. "
              "You probably have added or removed a LOG_LEVEL. "
              "Please update the definitions above accordingly");

static void lock() {}

static void unlock() {}

void logger_init(int speed) {
  LOGGER_SERIAL.begin(speed);
  delay(200);
  LOGGER_SERIAL.println();
}

void logger_log(int level, bool line, const String &msg) {
  // TODO: Figure out if \n is compatible with all terminals or we need \r\n.
  lock();

  String str = emptyString;
  if (s_cr_done) {
    str += s_log_level_string[level];
  }
  str += msg;
  if (line) {
    str += "\n";
  }
  LOGGER_SERIAL.print(str);
  s_cr_done = line;

  unlock();
}
