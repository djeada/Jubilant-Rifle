#include "map/map.h"
#include "map/ladder.h"
#include "map/trap.h"
#include "map/flag.h"
#include "utils/consts.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

/**
 * Helper: read the entire contents of a file into a dynamically allocated
 * string. Returns the string on success (which the caller must free) or NULL
 * on failure.
 */
static char *readFileContents(const char *filePath) {
  if (!filePath)
    return NULL;

  FILE *file = fopen(filePath, "rb");
  if (!file)
    return NULL;

  fseek(file, 0, SEEK_END);
  long length = ftell(file);
  if (length < 0) {
    fclose(file);
    return NULL;
  }
  fseek(file, 0, SEEK_SET);

  char *data = malloc((size_t)length + 1);
  if (!data) {
    fclose(file);
    return NULL;
  }

  size_t bytesRead = fread(data, 1, (size_t)length, file);
  data[bytesRead] = '\0';
  fclose(file);
  return data;
}

void mapInit(Map *map) {
  if (map) {
    map->platforms = NULL;
    map->platformCount = 0;
    map->backgroundImage = NULL;
    map->rect.x = 0;
    map->rect.y = 0;
    map->rect.w = 0;
    map->rect.h = 0;
    map->ladders = NULL;
    map->traps = NULL;
    map->flags = NULL;
  }
}

Point getRandomPositionOnPlatform(const Platform *platform) {
  Point position = {0, 0};
  if (!platform)
    return position;

  /* Ensure there is room for the humanoid frame on this platform. */
  int maxOffset = platform->w - HUMANOID_FRAME_WIDTH;
  if (maxOffset <= 0) {
    maxOffset = 0;
  }
  int random_x = platform->x + (rand() % (maxOffset + 1));
  int y = platform->y - HUMANOID_FRAME_HEIGHT;
  position.x = (float)random_x;
  position.y = (float)y;
  return position;
}

int allocatePlatforms(Map *map, size_t count) {
    if (!map) return -1;
    free(map->platforms);  // free any existing platforms
    map->platforms = calloc(count, sizeof(Platform));
    if (!map->platforms) {
        return -1; // Memory allocation failure
    }
    map->platformCount = count;
    return 0;
}

char *allocateBackgroundImagePath(const char *path) {
    char *newPath = strdup(path);
    return newPath;  // strdup returns NULL on allocation failure
}

int parseStringValue(const char *json, const char *key, char *value, size_t valueSize) {
    const char *start = strstr(json, key);
    if (!start)
        return -1;
    start = strchr(start, ':');
    if (!start)
        return -1;
    // Skip the colon and any following whitespace (including newlines and tabs).
    start++;
    while (*start == ' ' || *start == '\t' || *start == '\n' || *start == '\r')
        start++;
    if (*start == '"')
        start++; // skip opening quote
    else
        return -1; // expecting a quoted string
    const char *end = strchr(start, '"');
    if (!end)
        return -1;
    size_t len = end - start;
    if (len >= valueSize)
        return -1;
    strncpy(value, start, len);
    value[len] = '\0';
    return 0;
}

int parseIntValue(const char *json, const char *key, int *value) {
    const char *start = strstr(json, key);
    if (!start)
        return -1;
    start = strchr(start, ':');
    if (!start)
        return -1;
    // Skip whitespace after colon
    start++;
    while (*start == ' ' || *start == '\t' || *start == '\n' || *start == '\r')
        start++;
    // Use strtol for robust conversion.
    char *endptr;
    long num = strtol(start, &endptr, 10);
    if (start == endptr) {
        return -1; // no valid conversion
    }
    *value = (int)num;
    return 0;
}

int countPlatformsInJson(const char *json) {
  if (!json)
    return -1;

  const char *start = strstr(json, "\"platforms\"");
  if (!start)
    return -1;

  start = strchr(start, '[');
  if (!start)
    return -1;

  /* Find the matching closing bracket */
  const char *end = strchr(start, ']');
  if (!end)
    return -1;

  /* Count the number of '{' characters between '[' and ']' */
  int count = 0;
  const char *p = start;
  while (p < end) {
    if (*p == '{')
      count++;
    p++;
  }

  return count;
}

int parsePlatforms(const char *json, Platform *platforms, size_t platformCount) {
    const char *start = strstr(json, "\"platforms\"");
    if (!start)
        return -1;
    start = strchr(start, '[');
    if (!start)
        return -1;
    start++; // move past '['

    for (size_t index = 0; index < platformCount; index++) {
        // Skip any whitespace or commas
        while (*start && (*start == ' ' || *start == '\n' || *start == ','))
            start++;
        if (*start != '{')
            return -1; // expected '{'
        int x, y, w, h, charsRead = 0;
        // Use %n to know how many characters were consumed.
        if (sscanf(start,
                   " { \"x\" : %d , \"y\" : %d , \"width\" : %d , \"height\" : %d } %n",
                   &x, &y, &w, &h, &charsRead) == 4) {
            platforms[index].x = x;
            platforms[index].y = y;
            platforms[index].w = w;
            platforms[index].h = h;
            start += charsRead;
        } else {
            return -1;
        }
    }
    return 0;
}

int parseMapFile(const char *filePath, Map *map) {
  if (!filePath || !map) {
    fprintf(stderr, "Invalid file path or map pointer\n");
    return -1;
  }

  /* Initialize map to safe defaults */
  mapInit(map);

  char *data = readFileContents(filePath);
  if (!data) {
    perror("Error reading file");
    return -1;
  }

  /* Parse the background image path. */
  char backgroundImagePath[256];
  if (parseStringValue(data, "\"background_image\"", backgroundImagePath,
                       sizeof(backgroundImagePath)) != 0) {
    fprintf(stderr, "Failed to parse background image path\n");
    free(data);
    return -1;
  }
  map->backgroundImage = allocateBackgroundImagePath(backgroundImagePath);
  if (!map->backgroundImage) {
    perror("Memory allocation failed for background image");
    free(data);
    return -1;
  }

  /* Parse map dimensions. */
  int width, height;
  if (parseIntValue(data, "\"width\"", &width) != 0 ||
      parseIntValue(data, "\"height\"", &height) != 0) {
    fprintf(stderr, "Failed to parse map dimensions\n");
    free(map->backgroundImage);
    map->backgroundImage = NULL;
    free(data);
    return -1;
  }
  map->rect.x = 0;
  map->rect.y = 0;
  map->rect.w = width;
  map->rect.h = height;

  /* Dynamically count platforms instead of hardcoding */
  int platformCount = countPlatformsInJson(data);
  if (platformCount <= 0) {
    fprintf(stderr, "No platforms found in map file\n");
    free(map->backgroundImage);
    map->backgroundImage = NULL;
    free(data);
    return -1;
  }

  if (allocatePlatforms(map, (size_t)platformCount) != 0) {
    perror("Memory allocation failed for platforms");
    free(map->backgroundImage);
    map->backgroundImage = NULL;
    free(data);
    return -1;
  }

  if (parsePlatforms(data, map->platforms, (size_t)platformCount) != 0) {
    fprintf(stderr, "Failed to parse platforms\n");
    mapDestructor(map);
    free(data);
    return -1;
  }

  /* Parse optional ladders */
  LadderArray *ladders = malloc(sizeof(LadderArray));
  if (ladders) {
    ladderArrayInit(ladders);
    parseLadders(data, ladders);
    map->ladders = ladders;
  }

  /* Parse optional traps */
  TrapArray *traps = malloc(sizeof(TrapArray));
  if (traps) {
    trapArrayInit(traps);
    parseTraps(data, traps);
    map->traps = traps;
  }

  /* Parse optional flags */
  FlagArray *flags = malloc(sizeof(FlagArray));
  if (flags) {
    flagArrayInit(flags);
    parseFlags(data, flags);
    map->flags = flags;
  }

  free(data);
  return 0;
}

void mapDestructor(Map *map) {
    if (map) {
        free(map->backgroundImage);
        map->backgroundImage = NULL;
        free(map->platforms);
        map->platforms = NULL;
        map->platformCount = 0;
        
        /* Destroy ladders */
        if (map->ladders) {
            ladderArrayDestroy((LadderArray *)map->ladders);
            free(map->ladders);
            map->ladders = NULL;
        }
        
        /* Destroy traps */
        if (map->traps) {
            trapArrayDestroy((TrapArray *)map->traps);
            free(map->traps);
            map->traps = NULL;
        }
        
        /* Destroy flags */
        if (map->flags) {
            flagArrayDestroy((FlagArray *)map->flags);
            free(map->flags);
            map->flags = NULL;
        }
    }
}
