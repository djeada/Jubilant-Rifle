#ifndef FLAG_H
#define FLAG_H

/**
 * @file flag.h
 * @brief Flag capture objectives for level goals.
 *
 * Provides functionality for flag capture mechanics that add
 * strategic objectives to gameplay.
 */

#include "utils/point.h"
#include <SDL2/SDL.h>
#include <stdbool.h>
#include <stdlib.h>

/**
 * @brief State of a flag.
 */
typedef enum {
  FLAG_STATE_UNCAPTURED,  /**< Flag not yet captured */
  FLAG_STATE_CAPTURING,   /**< Player is capturing the flag */
  FLAG_STATE_CAPTURED     /**< Flag has been captured */
} FlagState;

/**
 * @brief Represents a capturable flag in the map.
 */
typedef struct Flag {
  SDL_Rect rect;           /**< Flag bounds and capture zone */
  FlagState state;         /**< Current state */
  float captureProgress;   /**< Progress toward capture (0.0 - 1.0) */
  float captureTime;       /**< Time required to capture in seconds */
  int pointValue;          /**< Points awarded for capture */
  bool isObjective;        /**< Whether this flag is a level objective */
} Flag;

/**
 * @brief Array of flags in a map.
 */
typedef struct FlagArray {
  Flag *flags;       /**< Array of flags */
  size_t count;      /**< Number of flags */
  size_t capacity;   /**< Allocated capacity */
  size_t captured;   /**< Number of captured flags */
  size_t required;   /**< Number of flags required to complete objective */
} FlagArray;

/**
 * @brief Initialize a flag array.
 * @param arr The array to initialize.
 */
void flagArrayInit(FlagArray *arr);

/**
 * @brief Destroy a flag array and free resources.
 * @param arr The array to destroy.
 */
void flagArrayDestroy(FlagArray *arr);

/**
 * @brief Add a flag to the array.
 * @param arr The array to add to.
 * @param flag The flag to add.
 * @return 0 on success, -1 on failure.
 */
int flagArrayAdd(FlagArray *arr, const Flag *flag);

/**
 * @brief Update flag capture progress.
 * @param arr The flag array.
 * @param playerRect Player's bounding rect.
 * @param dt Delta time in seconds.
 * @return Number of flags captured this frame.
 */
int flagArrayUpdate(FlagArray *arr, const SDL_Rect *playerRect, float dt);

/**
 * @brief Check if all objective flags are captured.
 * @param arr The flag array.
 * @return true if all objectives complete, false otherwise.
 */
bool flagArrayObjectivesComplete(const FlagArray *arr);

/**
 * @brief Reset all flags to uncaptured state.
 * @param arr The flag array.
 */
void flagArrayReset(FlagArray *arr);

/**
 * @brief Parse flags from JSON data.
 * @param json The JSON string to parse.
 * @param arr The array to populate.
 * @return 0 on success, -1 on failure.
 */
int parseFlags(const char *json, FlagArray *arr);

/**
 * @brief Count flags in JSON data.
 * @param json The JSON string to parse.
 * @return Number of flags found, or -1 on error.
 */
int countFlagsInJson(const char *json);

#endif /* FLAG_H */
