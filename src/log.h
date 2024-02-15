#ifndef LOG_H
#define LOG_H

#include "config.h"

// Log levels.
#define APP_LOG_LEVEL_TRACE  0
#define APP_LOG_LEVEL_DEBUG  1
#define APP_LOG_LEVEL_INFO   2
#define APP_LOG_LEVEL_ERROR  3
#define APP_LOG_LEVEL_FATAL  4
#define APP_LOG_LEVEL_SILENT 5

#if APP_CURRENT_LOG_LEVEL <= APP_LOG_LEVEL_TRACE
#  define app_trace(msg)   logger_log(APP_LOG_LEVEL_TRACE, false, msg)
#  define app_traceln(msg) logger_log(APP_LOG_LEVEL_TRACE, true, msg)
#else
#  define app_trace(msg)
#  define app_traceln(msg)
#endif

#if APP_CURRENT_LOG_LEVEL <= APP_LOG_LEVEL_DEBUG
#  define app_debug(msg)   logger_log(APP_LOG_LEVEL_DEBUG, false, msg)
#  define app_debugln(msg) logger_log(APP_LOG_LEVEL_DEBUG, true, msg)
#else
#  define app_debug(msg)
#  define app_debugln(msg)
#endif

#if APP_CURRENT_LOG_LEVEL <= APP_LOG_LEVEL_INFO
#  define app_info(msg)   logger_log(APP_LOG_LEVEL_INFO, false, msg)
#  define app_infoln(msg) logger_log(APP_LOG_LEVEL_INFO, true, msg)
#else
#  define app_info(msg)
#  define app_infoln(msg)
#endif

#if APP_CURRENT_LOG_LEVEL <= APP_LOG_LEVEL_ERROR
#  define app_error(msg)   logger_log(APP_LOG_LEVEL_ERROR, false, msg)
#  define app_errorln(msg) logger_log(APP_LOG_LEVEL_ERROR, true, msg)
#else
#  define app_error(msg)
#  define app_errorln(msg)
#endif

#if APP_CURRENT_LOG_LEVEL <= APP_LOG_LEVEL_FATAL
#  define app_fatal(msg)   logger_log(APP_LOG_LEVEL_FATAL, false, msg)
#  define app_fatalln(msg) logger_log(APP_LOG_LEVEL_FATAL, true, msg)
#else
#  define app_fatal(msg)
#  define app_fatalln(msg)
#endif

void logger_init(int speed = BAUD_RATE);
void logger_log(int level, bool line, const String &msg = "");

#endif // LOG_H
