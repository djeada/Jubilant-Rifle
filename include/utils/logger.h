#ifndef LOGGER_H
#define LOGGER_H

#include <stdio.h>

/**
 * @file logger.h
 * @brief Logging utilities for consistent debug and error messages.
 *
 * Provides macros for logging at different levels: DEBUG, INFO, WARNING, ERROR.
 * Logs include file name, line number, and function name for easier debugging.
 */

/* Log levels */
typedef enum {
  LOG_LEVEL_DEBUG = 0,
  LOG_LEVEL_INFO = 1,
  LOG_LEVEL_WARNING = 2,
  LOG_LEVEL_ERROR = 3,
  LOG_LEVEL_NONE = 4
} LogLevel;

/* Current log level - can be set at runtime */
extern LogLevel g_logLevel;

/**
 * @brief Set the global log level.
 * @param level The minimum level of messages to display.
 */
void loggerSetLevel(LogLevel level);

/**
 * @brief Get the current log level.
 * @return The current log level.
 */
LogLevel loggerGetLevel(void);

/**
 * @brief Internal logging function - use macros instead.
 */
void loggerLog(LogLevel level, const char *file, int line, const char *func,
               const char *fmt, ...);

/* Logging macros for convenience */
#define LOG_DEBUG(fmt, ...)                                                    \
  loggerLog(LOG_LEVEL_DEBUG, __FILE__, __LINE__, __func__, fmt, ##__VA_ARGS__)

#define LOG_INFO(fmt, ...)                                                     \
  loggerLog(LOG_LEVEL_INFO, __FILE__, __LINE__, __func__, fmt, ##__VA_ARGS__)

#define LOG_WARNING(fmt, ...)                                                  \
  loggerLog(LOG_LEVEL_WARNING, __FILE__, __LINE__, __func__, fmt, ##__VA_ARGS__)

#define LOG_ERROR(fmt, ...)                                                    \
  loggerLog(LOG_LEVEL_ERROR, __FILE__, __LINE__, __func__, fmt, ##__VA_ARGS__)

#endif /* LOGGER_H */
