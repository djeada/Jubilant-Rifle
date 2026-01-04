#ifndef RESULT_H
#define RESULT_H

/**
 * @file result.h
 * @brief Error handling utilities for consistent result management.
 *
 * Provides a Result type that can represent success or failure with
 * an optional error message. This enables better error propagation
 * throughout the codebase.
 */

#include <stdbool.h>

/* Maximum length for error messages */
#define RESULT_ERROR_MSG_MAX 256

/**
 * @brief Error codes for various failure scenarios.
 */
typedef enum {
  RESULT_OK = 0,
  RESULT_ERR_NULL_PARAM,
  RESULT_ERR_ALLOCATION,
  RESULT_ERR_FILE_NOT_FOUND,
  RESULT_ERR_FILE_READ,
  RESULT_ERR_PARSE,
  RESULT_ERR_INVALID_STATE,
  RESULT_ERR_OUT_OF_BOUNDS,
  RESULT_ERR_SDL,
  RESULT_ERR_UNKNOWN
} ResultCode;

/**
 * @brief Result type for functions that can fail.
 */
typedef struct {
  ResultCode code;
  char message[RESULT_ERROR_MSG_MAX];
} Result;

/**
 * @brief Create a success result.
 * @return A Result indicating success.
 */
Result resultSuccess(void);

/**
 * @brief Create an error result.
 * @param code The error code.
 * @param message The error message (can be NULL).
 * @return A Result indicating failure.
 */
Result resultError(ResultCode code, const char *message);

/**
 * @brief Create an error result with formatted message.
 * @param code The error code.
 * @param fmt The format string.
 * @param ... Format arguments.
 * @return A Result indicating failure.
 */
Result resultErrorFmt(ResultCode code, const char *fmt, ...);

/**
 * @brief Check if a result indicates success.
 * @param result The result to check.
 * @return true if successful, false otherwise.
 */
bool resultIsOk(const Result *result);

/**
 * @brief Check if a result indicates failure.
 * @param result The result to check.
 * @return true if failed, false otherwise.
 */
bool resultIsErr(const Result *result);

/**
 * @brief Get the error message from a result.
 * @param result The result.
 * @return The error message, or empty string if success.
 */
const char *resultMessage(const Result *result);

/**
 * @brief Get a human-readable string for an error code.
 * @param code The error code.
 * @return A string describing the error code.
 */
const char *resultCodeToString(ResultCode code);

#endif /* RESULT_H */
