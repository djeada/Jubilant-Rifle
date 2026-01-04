#ifndef LADDER_H
#define LADDER_H

/**
 * @file ladder.h
 * @brief Ladder structures for vertical movement between platforms.
 *
 * Provides functionality for ladders that allow players to climb
 * between different platform levels.
 */

#include "utils/point.h"
#include <SDL2/SDL.h>
#include <stdbool.h>
#include <stdlib.h>

/**
 * @brief Represents a ladder connecting platforms.
 */
typedef struct Ladder {
  SDL_Rect rect;     /**< Ladder bounds (x, y, width, height) */
  int topPlatformY;  /**< Y position of the top platform */
  int bottomPlatformY; /**< Y position of the bottom platform */
} Ladder;

/**
 * @brief Array of ladders in a map.
 */
typedef struct LadderArray {
  Ladder *ladders;    /**< Array of ladders */
  size_t count;       /**< Number of ladders */
  size_t capacity;    /**< Allocated capacity */
} LadderArray;

/**
 * @brief Initialize a ladder array.
 * @param arr The array to initialize.
 */
void ladderArrayInit(LadderArray *arr);

/**
 * @brief Destroy a ladder array and free resources.
 * @param arr The array to destroy.
 */
void ladderArrayDestroy(LadderArray *arr);

/**
 * @brief Add a ladder to the array.
 * @param arr The array to add to.
 * @param ladder The ladder to add.
 * @return 0 on success, -1 on failure.
 */
int ladderArrayAdd(LadderArray *arr, const Ladder *ladder);

/**
 * @brief Check if a point is on a ladder.
 * @param arr The ladder array to check.
 * @param x X coordinate.
 * @param y Y coordinate.
 * @param width Entity width.
 * @param height Entity height.
 * @return Pointer to the ladder if found, NULL otherwise.
 */
const Ladder *ladderArrayFindAt(const LadderArray *arr, float x, float y, 
                                  int width, int height);

/**
 * @brief Check if an entity rect overlaps with any ladder.
 * @param arr The ladder array.
 * @param entityRect The entity's bounding rect.
 * @return Pointer to overlapping ladder, or NULL.
 */
const Ladder *ladderArrayCheckOverlap(const LadderArray *arr, 
                                        const SDL_Rect *entityRect);

/**
 * @brief Parse ladders from JSON data.
 * @param json The JSON string to parse.
 * @param arr The array to populate.
 * @return 0 on success, -1 on failure.
 */
int parseLadders(const char *json, LadderArray *arr);

/**
 * @brief Count ladders in JSON data.
 * @param json The JSON string to parse.
 * @return Number of ladders found, or -1 on error.
 */
int countLaddersInJson(const char *json);

#endif /* LADDER_H */
