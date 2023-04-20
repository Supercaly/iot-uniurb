#ifndef LOG_H
#define LOG_H

#include <WString.h>

#include "config.h"

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

/*
 * Class implementing a logger.
 */
class Logger {
  public:
  Logger(LogLevel level);

  void init(int speed = BAUD_RATE);

  void trace(const String &s);
  void debug(const String &s);
  void info(const String &s);
  void error(const String &s);
  void fatal(const String &s);

  void traceln();
  void debugln();
  void infoln();
  void errorln();
  void fatalln();

  void traceln(const String &s);
  void debugln(const String &s);
  void infoln(const String &s);
  void errorln(const String &s);
  void fatalln(const String &s);

  private:
  LogLevel _level;
  bool     _cr_done;

  void print(LogLevel level, const String &s);
  void println(LogLevel level, const String &s);
};

// Global Logger instance.
extern Logger Log;

#endif // LOG_H
