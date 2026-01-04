#include "map/ladder.h"
#include <stdio.h>
#include <string.h>

#define LADDER_INITIAL_CAPACITY 8

void ladderArrayInit(LadderArray *arr) {
  if (!arr)
    return;

  arr->ladders = NULL;
  arr->count = 0;
  arr->capacity = 0;
}

void ladderArrayDestroy(LadderArray *arr) {
  if (!arr)
    return;

  free(arr->ladders);
  arr->ladders = NULL;
  arr->count = 0;
  arr->capacity = 0;
}

int ladderArrayAdd(LadderArray *arr, const Ladder *ladder) {
  if (!arr || !ladder)
    return -1;

  /* Grow array if needed */
  if (arr->count >= arr->capacity) {
    size_t newCapacity = (arr->capacity == 0) ? LADDER_INITIAL_CAPACITY 
                                               : arr->capacity * 2;
    Ladder *newLadders = realloc(arr->ladders, newCapacity * sizeof(Ladder));
    if (!newLadders)
      return -1;
    
    arr->ladders = newLadders;
    arr->capacity = newCapacity;
  }

  arr->ladders[arr->count] = *ladder;
  arr->count++;
  return 0;
}

const Ladder *ladderArrayFindAt(const LadderArray *arr, float x, float y,
                                  int width, int height) {
  if (!arr)
    return NULL;

  SDL_Rect entityRect = {(int)x, (int)y, width, height};
  return ladderArrayCheckOverlap(arr, &entityRect);
}

const Ladder *ladderArrayCheckOverlap(const LadderArray *arr, 
                                        const SDL_Rect *entityRect) {
  if (!arr || !entityRect)
    return NULL;

  for (size_t i = 0; i < arr->count; i++) {
    if (SDL_HasIntersection(&arr->ladders[i].rect, entityRect)) {
      return &arr->ladders[i];
    }
  }
  return NULL;
}

int countLaddersInJson(const char *json) {
  if (!json)
    return 0;

  const char *start = strstr(json, "\"ladders\"");
  if (!start)
    return 0;

  start = strchr(start, '[');
  if (!start)
    return 0;

  const char *end = strchr(start, ']');
  if (!end)
    return 0;

  /* Count '{' characters between '[' and ']' */
  int count = 0;
  const char *p = start;
  while (p < end) {
    if (*p == '{')
      count++;
    p++;
  }

  return count;
}

int parseLadders(const char *json, LadderArray *arr) {
  if (!json || !arr)
    return -1;

  const char *start = strstr(json, "\"ladders\"");
  if (!start)
    return 0; /* No ladders section is not an error */

  start = strchr(start, '[');
  if (!start)
    return -1;
  start++; /* Move past '[' */

  int ladderCount = countLaddersInJson(json);
  if (ladderCount <= 0)
    return 0;

  for (int i = 0; i < ladderCount; i++) {
    /* Skip whitespace and commas */
    while (*start && (*start == ' ' || *start == '\n' || *start == ',' || 
           *start == '\t' || *start == '\r'))
      start++;
    
    if (*start != '{')
      return -1;

    Ladder ladder = {0};
    int charsRead = 0;
    
    if (sscanf(start, " { \"x\" : %d , \"y\" : %d , \"width\" : %d , \"height\" : %d } %n",
               &ladder.rect.x, &ladder.rect.y, &ladder.rect.w, &ladder.rect.h, 
               &charsRead) == 4) {
      ladder.topPlatformY = ladder.rect.y;
      ladder.bottomPlatformY = ladder.rect.y + ladder.rect.h;
      ladderArrayAdd(arr, &ladder);
      start += charsRead;
    } else {
      return -1;
    }
  }

  return 0;
}
