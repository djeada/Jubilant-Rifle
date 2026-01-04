#include "utils/logger.h"
#include <stdarg.h>
#include <stdio.h>
#include <time.h>

/* Default log level */
LogLevel g_logLevel = LOG_LEVEL_INFO;

void loggerSetLevel(LogLevel level) { g_logLevel = level; }

LogLevel loggerGetLevel(void) { return g_logLevel; }

static const char *levelToString(LogLevel level) {
  switch (level) {
  case LOG_LEVEL_DEBUG:
    return "DEBUG";
  case LOG_LEVEL_INFO:
    return "INFO";
  case LOG_LEVEL_WARNING:
    return "WARNING";
  case LOG_LEVEL_ERROR:
    return "ERROR";
  default:
    return "UNKNOWN";
  }
}

static const char *levelToColor(LogLevel level) {
  switch (level) {
  case LOG_LEVEL_DEBUG:
    return "\033[36m"; /* Cyan */
  case LOG_LEVEL_INFO:
    return "\033[32m"; /* Green */
  case LOG_LEVEL_WARNING:
    return "\033[33m"; /* Yellow */
  case LOG_LEVEL_ERROR:
    return "\033[31m"; /* Red */
  default:
    return "\033[0m"; /* Reset */
  }
}

void loggerLog(LogLevel level, const char *file, int line, const char *func,
               const char *fmt, ...) {
  if (level < g_logLevel) {
    return;
  }

  /* Get current time */
  time_t now = time(NULL);
  struct tm *tm_info = localtime(&now);
  char timeStr[20];
  strftime(timeStr, sizeof(timeStr), "%Y-%m-%d %H:%M:%S", tm_info);

  /* Extract just the filename from the path */
  const char *filename = file;
  const char *p = file;
  while (*p) {
    if (*p == '/' || *p == '\\') {
      filename = p + 1;
    }
    p++;
  }

  /* Print log header with color */
  FILE *out = (level >= LOG_LEVEL_WARNING) ? stderr : stdout;
  fprintf(out, "%s[%s] %s (%s:%d in %s): \033[0m", levelToColor(level),
          levelToString(level), timeStr, filename, line, func);

  /* Print the actual message */
  va_list args;
  va_start(args, fmt);
  vfprintf(out, fmt, args);
  va_end(args);

  fprintf(out, "\n");
  fflush(out);
}
