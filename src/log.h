#ifndef LOG_H
#define LOG_H

#include <Arduino.h>

#include "config.h"

/*
 * Class implementing a logger.
 */
class Logger {
  public:
  /*
   * Enum representing the log level.
   */
  enum LogLevel {
    TRACE,
    DEBUG,
    INFO,
    ERROR,
    FATAL,
    SILENT
  };

  Logger(LogLevel level);

  void init(int speed = BAUD_RATE);

  inline void trace(const String &s) { print(LogLevel::TRACE, s); }

  inline void debug(const String &s) { print(LogLevel::DEBUG, s); }

  inline void info(const String &s) { print(LogLevel::INFO, s); }

  inline void error(const String &s) { print(LogLevel::ERROR, s); }

  inline void fatal(const String &s) { print(LogLevel::FATAL, s); }

  inline void traceln(const String &s = "") { println(LogLevel::TRACE, s); }

  inline void debugln(const String &s = "") { println(LogLevel::DEBUG, s); }

  inline void infoln(const String &s = "") { println(LogLevel::INFO, s); }

  inline void errorln(const String &s = "") { println(LogLevel::ERROR, s); }

  inline void fatalln(const String &s = "") { println(LogLevel::FATAL, s); }

  private:
  LogLevel _level;
  bool     _cr_done = true;

  void print(LogLevel level, const String &s);
  void println(LogLevel level, const String &s);
};

// Global Logger instance.
extern Logger Log;

#endif // LOG_H
