#ifndef MAP_H
#define MAP_H

#include "utils/point.h"
#include <SDL2/SDL.h>
#include <stdlib.h>

/**
 * @file map.h
 * @brief Map structures and loading utilities.
 *
 * Provides functionality for loading, parsing, and managing game maps
 * including platforms and background images.
 */

/* Use SDL_Rect for platforms. */
typedef SDL_Rect Platform;

/**
 * @brief Represents a game map with platforms and background.
 */
typedef struct {
  Platform *platforms;    /**< Array of platforms in the map */
  size_t platformCount;   /**< Number of platforms */
  char *backgroundImage;  /**< Path to the background image */
  SDL_Rect rect;          /**< Map boundaries (top-left at origin) */
} Map;

/**
 * @brief Get a random position on top of a platform.
 * @param platform The platform to position on.
 * @return A Point representing a valid position on the platform.
 */
Point getRandomPositionOnPlatform(const Platform *platform);

/**
 * @brief Allocate memory for platforms in a map.
 * @param map The map to allocate platforms for.
 * @param count Number of platforms to allocate.
 * @return 0 on success, -1 on failure.
 */
int allocatePlatforms(Map *map, size_t count);

/**
 * @brief Allocate and copy a background image path.
 * @param path The path to copy.
 * @return Allocated string on success, NULL on failure.
 */
char *allocateBackgroundImagePath(const char *path);

/**
 * @brief Parse a string value from JSON data.
 * @param json The JSON string to parse.
 * @param key The key to look for.
 * @param value Output buffer for the value.
 * @param valueSize Size of the output buffer.
 * @return 0 on success, -1 on failure.
 */
int parseStringValue(const char *json, const char *key, char *value,
                     size_t valueSize);

/**
 * @brief Parse an integer value from JSON data.
 * @param json The JSON string to parse.
 * @param key The key to look for.
 * @param value Output pointer for the value.
 * @return 0 on success, -1 on failure.
 */
int parseIntValue(const char *json, const char *key, int *value);

/**
 * @brief Count the number of platforms in JSON data.
 * @param json The JSON string to parse.
 * @return Number of platforms found, or -1 on error.
 */
int countPlatformsInJson(const char *json);

/**
 * @brief Parse platforms from JSON data.
 * @param json The JSON string to parse.
 * @param platforms Output array of platforms.
 * @param platformCount Number of platforms to parse.
 * @return 0 on success, -1 on failure.
 */
int parsePlatforms(const char *json, Platform *platforms, size_t platformCount);

/**
 * @brief Parse a map file and populate a Map structure.
 * @param filePath Path to the map configuration file.
 * @param map Output Map structure to populate.
 * @return 0 on success, -1 on failure.
 */
int parseMapFile(const char *filePath, Map *map);

/**
 * @brief Initialize a Map structure to safe defaults.
 * @param map The map to initialize.
 */
void mapInit(Map *map);

/**
 * @brief Free all resources associated with a map.
 * @param map The map to destroy.
 */
void mapDestructor(Map *map);

#endif /* MAP_H */
