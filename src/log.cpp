#include "log.h"
#include "utils.h"

#include <assert.h>

#ifdef IS_DEBUG
// TODO: Change log level to debug when debug is on
Logger Log(Logger::LogLevel::TRACE);
#else
Logger Log(Logger::LogLevel::INFO);
#endif

static String log_level_string[] = {
  "[TRACE]  ",
  "[DEBUG]  ",
  "[INFO]   ",
  "[ERROR]  ",
  "[FATAL]  ",
  "[SILENT] "
};
static_assert(size_of_array(log_level_string) == 6,
              "The size of log_level_string has changed. "
              "Please update the definitions above accordingly");

Logger::Logger(LogLevel level)
  : _level(level) {}

void Logger::init(int speed) {
  LOGGER_SERIAL.begin(speed);
  delay(200);
  LOGGER_SERIAL.println();
}

void Logger::print(LogLevel level, const String &s) {
  if (level < _level)
    return;

  if (_cr_done) {
    LOGGER_SERIAL.print(log_level_string[level]);
  }
  LOGGER_SERIAL.print(s);
  _cr_done = false;
}

void Logger::println(LogLevel level, const String &s) {
  // TODO: Figure out if \n is compatible with all terminals or we need \r\n.
  print(level, s + "\n");
  _cr_done = true;
}
