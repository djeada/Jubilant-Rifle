#include "map/map.h"
#include "utils/utils.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int allocatePlatforms(Map *map, size_t count) {
  free(map->platforms); // Free old platforms if they exist.
  map->platforms = calloc(count, sizeof(Platform));
  if (!map->platforms) {
    return -1; // Memory allocation failure
  }
  map->platformCount = count;
  return 0;
}

char *allocateBackgroundImagePath(const char *path) {
  char *newPath = strdup(path); // No need for custom customStrdup, use strdup
  if (!newPath) {
    return NULL; // Memory allocation failure
  }
  return newPath;
}

int parseStringValue(const char *json, const char *key, char *value,
                     size_t valueSize) {
  char *start = strstr(json, key);
  if (!start)
    return -1;
  start = strchr(start, ':');
  if (!start)
    return -1;
  start += 2; // Skip over the ": "
  if (*start == '"')
    start++; // Skip opening quote
  char *end = strchr(start, '"');
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
  char *start = strstr(json, key);
  if (!start)
    return -1;
  start = strchr(start, ':');
  if (!start)
    return -1;
  *value = atoi(start + 1);
  return 0;
}

int parsePlatforms(const char *json, Platform *platforms,
                   size_t platformCount) {
  char *start = strstr(json, "\"platforms\"");
  if (!start)
    return -1;
  start = strchr(start, '[');
  if (!start)
    return -1;
  start++; // Move past the '['

  size_t index = 0;
  while (index < platformCount) {
    Platform *platform = &platforms[index];
    if (sscanf(start,
               " { \"x\": %d , \"y\": %d , \"width\": %d , \"height\": %d } ,",
               &platform->x, &platform->y, &platform->width,
               &platform->height) == 4) {
      index++;
      start = strchr(start, '}');
      if (!start)
        return -1;
      start += 2; // Move past the '},'
    } else if (sscanf(start,
                      " { \"x\": %d , \"y\": %d , \"width\": %d , \"height\": "
                      "%d }",
                      &platform->x, &platform->y, &platform->width,
                      &platform->height) == 4) {
      index++;
      break;
    } else {
      return -1;
    }
  }

  return index == platformCount ? 0 : -1;
}

int parseMapFile(const char *filePath, Map *map) {
  if (!filePath || !map) {
    fprintf(stderr, "Invalid file path or map\n");
    return -1;
  }

  FILE *file = fopen(filePath, "r");
  if (!file) {
    perror("Error opening file");
    return -1;
  }

  fseek(file, 0, SEEK_END);
  long length = ftell(file);
  fseek(file, 0, SEEK_SET);
  char *data = malloc(length + 1);
  if (!data) {
    perror("Memory allocation failed");
    fclose(file);
    return -1;
  }
  fread(data, 1, length, file);
  data[length] = '\0';
  fclose(file);

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

  if (parseIntValue(data, "\"width\"", &map->width) != 0 ||
      parseIntValue(data, "\"height\"", &map->height) != 0) {
    fprintf(stderr, "Failed to parse map dimensions\n");
    free(data);
    return -1;
  }

  size_t platformCount = 20; // We know there are 20 platforms
  if (allocatePlatforms(map, platformCount) != 0) {
    perror("Memory allocation failed for platforms");
    free(data);
    return -1;
  }

  if (parsePlatforms(data, map->platforms, platformCount) != 0) {
    fprintf(stderr, "Failed to parse platforms\n");
    free(data);
    return -1;
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
  }
}
