#include "utils/result.h"
#include <stdarg.h>
#include <stdio.h>
#include <string.h>

Result resultSuccess(void) {
  Result result;
  result.code = RESULT_OK;
  result.message[0] = '\0';
  return result;
}

Result resultError(ResultCode code, const char *message) {
  Result result;
  result.code = code;
  if (message) {
    strncpy(result.message, message, RESULT_ERROR_MSG_MAX - 1);
    result.message[RESULT_ERROR_MSG_MAX - 1] = '\0';
  } else {
    result.message[0] = '\0';
  }
  return result;
}

Result resultErrorFmt(ResultCode code, const char *fmt, ...) {
  Result result;
  result.code = code;

  va_list args;
  va_start(args, fmt);
  vsnprintf(result.message, RESULT_ERROR_MSG_MAX, fmt, args);
  va_end(args);

  return result;
}

bool resultIsOk(const Result *result) {
  return result && result->code == RESULT_OK;
}

bool resultIsErr(const Result *result) {
  return result && result->code != RESULT_OK;
}

const char *resultMessage(const Result *result) {
  if (!result) {
    return "";
  }
  return result->message;
}

const char *resultCodeToString(ResultCode code) {
  switch (code) {
  case RESULT_OK:
    return "Success";
  case RESULT_ERR_NULL_PARAM:
    return "Null parameter";
  case RESULT_ERR_ALLOCATION:
    return "Memory allocation failed";
  case RESULT_ERR_FILE_NOT_FOUND:
    return "File not found";
  case RESULT_ERR_FILE_READ:
    return "File read error";
  case RESULT_ERR_PARSE:
    return "Parse error";
  case RESULT_ERR_INVALID_STATE:
    return "Invalid state";
  case RESULT_ERR_OUT_OF_BOUNDS:
    return "Out of bounds";
  case RESULT_ERR_SDL:
    return "SDL error";
  case RESULT_ERR_UNKNOWN:
  default:
    return "Unknown error";
  }
}
