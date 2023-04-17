#include "log.h"

#ifdef IS_DEBUG
Logger Log(LogLevel::TRACE);
#else
Logger Log(LogLevel::INFO);
#endif // IS_DEBUG

static const String log_level_string[]
    = {"[TRACE]  ", "[DEBUG]  ", "[INFO]   ", "[ERROR]  ", "[FATAL]  ", "[SILENT] "};

Logger::Logger(LogLevel level) : _level(level), _cr_done(true) {}

void Logger::init(int speed) {
  LOGGER_SERIAL.begin(speed);
  delay(200);
  LOGGER_SERIAL.println();
}

void Logger::trace(const String &s) {
  print(LogLevel::TRACE, s);
}

void Logger::traceln() {
  println(LogLevel::TRACE, "");
}

void Logger::traceln(const String &s) {
  println(LogLevel::TRACE, s);
}

void Logger::debug(const String &s) {
  print(LogLevel::DEBUG, s);
}

void Logger::debugln() {
  println(LogLevel::DEBUG, "");
}

void Logger::debugln(const String &s) {
  println(LogLevel::DEBUG, s);
}

void Logger::info(const String &s) {
  print(LogLevel::INFO, s);
}

void Logger::infoln() {
  println(LogLevel::INFO, "");
}

void Logger::infoln(const String &s) {
  println(LogLevel::INFO, s);
}

void Logger::error(const String &s) {
  print(LogLevel::ERROR, s);
}

void Logger::errorln() {
  println(LogLevel::ERROR, "");
}

void Logger::errorln(const String &s) {
  println(LogLevel::ERROR, s);
}

void Logger::fatal(const String &s) {
  print(LogLevel::FATAL, s);
}

void Logger::fatalln() {
  println(LogLevel::FATAL, "");
}

void Logger::fatalln(const String &s) {
  println(LogLevel::FATAL, s);
}

void Logger::print(LogLevel level, const String &s) {
  if (level < _level) {
    return;
  }

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
