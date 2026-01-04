#ifndef MAP_GENERATOR_H
#define MAP_GENERATOR_H

/**
 * @file map_generator.h
 * @brief Procedural map generation utilities.
 *
 * Provides functionality for automatically generating game maps with
 * platforms, ladders, traps, and flags arranged in various patterns.
 */

#include "map/map.h"
#include <stdbool.h>
#include <time.h>

/**
 * @brief Map generation style/theme.
 */
typedef enum {
  MAP_GEN_STYLE_HORIZONTAL,  /**< Mostly horizontal progression */
  MAP_GEN_STYLE_VERTICAL,    /**< Mostly vertical progression */
  MAP_GEN_STYLE_MIXED,       /**< Mixed horizontal and vertical */
  MAP_GEN_STYLE_SPIRAL,      /**< Spiral pattern */
  MAP_GEN_STYLE_RANDOM       /**< Random arrangement */
} MapGenStyle;

/**
 * @brief Parameters for map generation.
 */
typedef struct {
  int width;              /**< Map width in pixels */
  int height;             /**< Map height in pixels */
  int platformCount;      /**< Number of platforms to generate */
  int minPlatformWidth;   /**< Minimum platform width */
  int maxPlatformWidth;   /**< Maximum platform width */
  int platformHeight;     /**< Platform height */
  int verticalSpacing;    /**< Vertical spacing between platform levels */
  int horizontalSpacing;  /**< Horizontal spacing between platforms */
  int ladderCount;        /**< Number of ladders to generate */
  int trapCount;          /**< Number of traps to generate */
  int flagCount;          /**< Number of flags to generate */
  MapGenStyle style;      /**< Generation style */
  unsigned int seed;      /**< Random seed (0 for time-based) */
} MapGenParams;

/**
 * @brief Initialize map generation parameters with defaults.
 * @param params The parameters to initialize.
 * @param width Map width.
 * @param height Map height.
 */
void mapGenParamsInit(MapGenParams *params, int width, int height);

/**
 * @brief Generate a random map based on parameters.
 * @param params Generation parameters.
 * @param map Output map structure.
 * @return 0 on success, -1 on failure.
 */
int generateMap(const MapGenParams *params, Map *map);

/**
 * @brief Generate platforms for a map.
 * @param params Generation parameters.
 * @param map Output map structure.
 * @return 0 on success, -1 on failure.
 */
int generatePlatforms(const MapGenParams *params, Map *map);

/**
 * @brief Generate ladders connecting platforms.
 * @param map Map with platforms to add ladders to.
 * @param ladderCount Number of ladders to generate.
 * @return 0 on success, -1 on failure.
 */
int generateLadders(Map *map, int ladderCount);

/**
 * @brief Generate traps on or near platforms.
 * @param map Map with platforms to add traps to.
 * @param trapCount Number of traps to generate.
 * @return 0 on success, -1 on failure.
 */
int generateTraps(Map *map, int trapCount);

/**
 * @brief Generate flags at strategic locations.
 * @param map Map with platforms to add flags to.
 * @param flagCount Number of flags to generate.
 * @return 0 on success, -1 on failure.
 */
int generateFlags(Map *map, int flagCount);

/**
 * @brief Save a generated map to a file.
 * @param map The map to save.
 * @param filePath Output file path.
 * @return 0 on success, -1 on failure.
 */
int saveMapToFile(const Map *map, const char *filePath);

/**
 * @brief Helper: Get a random integer in range [min, max].
 * @param min Minimum value (inclusive).
 * @param max Maximum value (inclusive).
 * @return Random integer in range.
 */
static inline int randomRange(int min, int max) {
  if (max <= min)
    return min;
  return min + (rand() % (max - min + 1));
}

/**
 * @brief Helper: Check if two rectangles overlap.
 * @param r1 First rectangle.
 * @param r2 Second rectangle.
 * @return true if rectangles overlap, false otherwise.
 */
static inline bool rectsOverlap(const SDL_Rect *r1, const SDL_Rect *r2) {
  return (r1->x < r2->x + r2->w && r1->x + r1->w > r2->x &&
          r1->y < r2->y + r2->h && r1->y + r1->h > r2->y);
}

#endif /* MAP_GENERATOR_H */
