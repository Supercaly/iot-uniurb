#ifndef LOG_H
#define LOG_H

#include <WString.h>

#include "config.h"

// Log levels.
#define LOG_LEVEL_TRACE  0
#define LOG_LEVEL_DEBUG  1
#define LOG_LEVEL_INFO   2
#define LOG_LEVEL_ERROR  3
#define LOG_LEVEL_FATAL  4
#define LOG_LEVEL_SILENT 5

#ifdef IS_DEBUG
#  define CURRENT_LOG_LEVEL LOG_LEVEL_DEBUG
#else
#  define CURRENT_LOG_LEVEL LOG_LEVEL_INFO
#endif

#if CURRENT_LOG_LEVEL <= LOG_LEVEL_TRACE
#  define LOG_TRACE(msg)   logger_log(LOG_LEVEL_TRACE, false, msg)
#  define LOG_TRACELN(msg) logger_log(LOG_LEVEL_TRACE, true, msg)
#else
#  define LOG_TRACE(msg)
#  define LOG_TRACELN(msg)
#endif

#if CURRENT_LOG_LEVEL <= LOG_LEVEL_DEBUG
#  define LOG_DEBUG(msg)   logger_log(LOG_LEVEL_DEBUG, false, msg)
#  define LOG_DEBUGLN(msg) logger_log(LOG_LEVEL_DEBUG, true, msg)
#else
#  define LOG_DEBUG(msg)
#  define LOG_DEBUGLN(msg)
#endif

#if CURRENT_LOG_LEVEL <= LOG_LEVEL_INFO
#  define LOG_INFO(msg)   logger_log(LOG_LEVEL_INFO, false, msg)
#  define LOG_INFOLN(msg) logger_log(LOG_LEVEL_INFO, true, msg)
#else
#  define LOG_INFO(msg)
#  define LOG_INFOLN(msg)
#endif

#if CURRENT_LOG_LEVEL <= LOG_LEVEL_ERROR
#  define LOG_ERROR(msg)   logger_log(LOG_LEVEL_ERROR, false, msg)
#  define LOG_ERRORLN(msg) logger_log(LOG_LEVEL_ERROR, true, msg)
#else
#  define LOG_ERROR(msg)
#  define LOG_ERRORLN(msg)
#endif

#if CURRENT_LOG_LEVEL <= LOG_LEVEL_FATAL
#  define LOG_FATAL(msg)   logger_log(LOG_LEVEL_FATAL, false, msg)
#  define LOG_FATALLN(msg) logger_log(LOG_LEVEL_FATAL, true, msg)
#else
#  define LOG_FATAL(msg)
#  define LOG_FATALLN(msg)
#endif

void logger_init(int speed = BAUD_RATE);
void logger_log(int level, bool line, const String &msg = "");

#endif // LOG_H
