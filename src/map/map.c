#include "map/map.h"
#include "utils/utils.h"
#include <stdio.h>
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
  char *newPath =
      customStrdup(path); // No need for custom customStrdup, use strdup
  if (!newPath) {
    return NULL; // Memory allocation failure
  }
  return newPath;
}

int parsePlatform(const char *line, Platform *platform) {
  int dummy; // Used to absorb the platform index, which is not used after
             // parsing
  // sscanf returns the number of successfully filled fields
  return sscanf(line, "platform%d = {%d, %d, %d, %d}", &dummy, &platform->x,
                &platform->y, &platform->width, &platform->height) == 5;
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

  char line[128];
  size_t currentPlatformIndex = 0;
  while (fgets(line, sizeof(line), file)) {
    char *newline = strchr(line, '\n');
    if (newline)
      *newline = '\0';

    if (strncmp(line, "background_image =", 18) == 0) {
      char *imagePath = line + 18;
      imagePath += strspn(imagePath, " =");
      free(map->backgroundImage);
      map->backgroundImage = allocateBackgroundImagePath(imagePath);
      if (!map->backgroundImage) {
        perror("Memory allocation failed for background image");
        fclose(file);
        return -1;
      }
    } else if (strncmp(line, "platform_count =", 16) == 0) {
      size_t count = atoi(line + 16);
      if (count <= 0) {
        fprintf(stderr, "Invalid platform count\n");
        fclose(file);
        return -1;
      }
      if (allocatePlatforms(map, count) != 0) {
        perror("Memory allocation failed for platforms");
        fclose(file);
        return -1;
      }
    } else if (sscanf(line, "platform%*d = {%*d, %*d, %*d, %*d}") ==
                   0 && // This line checks if the string follows the platform
                        // pattern
               currentPlatformIndex < map->platformCount) {
      if (!parsePlatform(line, &map->platforms[currentPlatformIndex])) {
        fprintf(stderr, "Platform parsing failed\n");
        fclose(file);
        return -1;
      }
      currentPlatformIndex++;
    } else {
      fprintf(stderr, "Unrecognized line or platform count exceeded\n");
      fclose(file);
      return -1;
    }
  }

  if (currentPlatformIndex != map->platformCount) {
    fprintf(stderr, "Platform count mismatch\n");
    fclose(file);
    return -1;
  }

  fclose(file);
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
