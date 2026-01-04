#include "map/flag.h"
#include <stdio.h>
#include <string.h>

#define FLAG_INITIAL_CAPACITY 8
#define DEFAULT_CAPTURE_TIME 3.0f
#define DEFAULT_FLAG_POINTS 500

void flagArrayInit(FlagArray *arr) {
  if (!arr)
    return;

  arr->flags = NULL;
  arr->count = 0;
  arr->capacity = 0;
  arr->captured = 0;
  arr->required = 0;
}

void flagArrayDestroy(FlagArray *arr) {
  if (!arr)
    return;

  free(arr->flags);
  arr->flags = NULL;
  arr->count = 0;
  arr->capacity = 0;
  arr->captured = 0;
  arr->required = 0;
}

int flagArrayAdd(FlagArray *arr, const Flag *flag) {
  if (!arr || !flag)
    return -1;

  /* Grow array if needed */
  if (arr->count >= arr->capacity) {
    size_t newCapacity = (arr->capacity == 0) ? FLAG_INITIAL_CAPACITY 
                                               : arr->capacity * 2;
    Flag *newFlags = realloc(arr->flags, newCapacity * sizeof(Flag));
    if (!newFlags)
      return -1;
    
    arr->flags = newFlags;
    arr->capacity = newCapacity;
  }

  arr->flags[arr->count] = *flag;
  arr->count++;
  
  if (flag->isObjective) {
    arr->required++;
  }
  
  return 0;
}

int flagArrayUpdate(FlagArray *arr, const SDL_Rect *playerRect, float dt) {
  if (!arr || !playerRect)
    return 0;

  int capturedThisFrame = 0;

  for (size_t i = 0; i < arr->count; i++) {
    Flag *flag = &arr->flags[i];
    
    if (flag->state == FLAG_STATE_CAPTURED)
      continue;

    /* Check if player is in capture zone */
    if (SDL_HasIntersection(&flag->rect, playerRect)) {
      flag->state = FLAG_STATE_CAPTURING;
      flag->captureProgress += dt / flag->captureTime;
      
      if (flag->captureProgress >= 1.0f) {
        flag->captureProgress = 1.0f;
        flag->state = FLAG_STATE_CAPTURED;
        arr->captured++;
        capturedThisFrame++;
      }
    } else {
      /* Player left capture zone, reset progress slowly */
      if (flag->state == FLAG_STATE_CAPTURING) {
        flag->captureProgress -= dt / (flag->captureTime * 2);
        if (flag->captureProgress <= 0.0f) {
          flag->captureProgress = 0.0f;
          flag->state = FLAG_STATE_UNCAPTURED;
        }
      }
    }
  }

  return capturedThisFrame;
}

bool flagArrayObjectivesComplete(const FlagArray *arr) {
  if (!arr || arr->required == 0)
    return true;
  
  size_t capturedObjectives = 0;
  for (size_t i = 0; i < arr->count; i++) {
    if (arr->flags[i].isObjective && 
        arr->flags[i].state == FLAG_STATE_CAPTURED) {
      capturedObjectives++;
    }
  }
  
  return capturedObjectives >= arr->required;
}

void flagArrayReset(FlagArray *arr) {
  if (!arr)
    return;

  arr->captured = 0;
  for (size_t i = 0; i < arr->count; i++) {
    arr->flags[i].state = FLAG_STATE_UNCAPTURED;
    arr->flags[i].captureProgress = 0.0f;
  }
}

int countFlagsInJson(const char *json) {
  if (!json)
    return 0;

  const char *start = strstr(json, "\"flags\"");
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

int parseFlags(const char *json, FlagArray *arr) {
  if (!json || !arr)
    return -1;

  const char *start = strstr(json, "\"flags\"");
  if (!start)
    return 0; /* No flags section is not an error */

  start = strchr(start, '[');
  if (!start)
    return -1;
  start++; /* Move past '[' */

  int flagCount = countFlagsInJson(json);
  if (flagCount <= 0)
    return 0;

  for (int i = 0; i < flagCount; i++) {
    /* Skip whitespace and commas */
    while (*start && (*start == ' ' || *start == '\n' || *start == ',' || 
           *start == '\t' || *start == '\r'))
      start++;
    
    if (*start != '{')
      return -1;

    Flag flag = {0};
    flag.state = FLAG_STATE_UNCAPTURED;
    flag.captureProgress = 0.0f;
    flag.captureTime = DEFAULT_CAPTURE_TIME;
    flag.pointValue = DEFAULT_FLAG_POINTS;
    flag.isObjective = true;
    
    int charsRead = 0;
    
    if (sscanf(start, " { \"x\" : %d , \"y\" : %d , \"width\" : %d , \"height\" : %d } %n",
               &flag.rect.x, &flag.rect.y, &flag.rect.w, &flag.rect.h, 
               &charsRead) == 4) {
      flagArrayAdd(arr, &flag);
      start += charsRead;
    } else {
      return -1;
    }
  }

  return 0;
}
