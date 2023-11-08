#include "map/map.h"
#include "utils/utils.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
int parseMapFile(const char *filePath, Map *map) {
  if (filePath == NULL || map == NULL) {
    perror("Invalid file path or map");
    return -1;
  }

  FILE *file = fopen(filePath, "r");
  if (file == NULL) {
    perror("Error opening file");
    return -1;
  }

  char line[128];
  int currentPlatformIndex = 0;
  int dummy = 0; // Dummy variable for absorbing the platform index
  while (fgets(line, sizeof(line), file)) {
    char *newline = strchr(line, '\n'); // Find newline character if present.
    if (newline)
      *newline = '\0'; // Replace it with null terminator.

    // Parse background image.
    if (strncmp(line, "background_image =", 18) == 0) {
      char *imagePath = line + 18;
      imagePath += strspn(imagePath, " ="); // Trim spaces and '='.
      free(map->backgroundImage);           // Free old image path if exists.
      map->backgroundImage = my_strdup(imagePath);
      if (!map->backgroundImage) {
        perror("Memory allocation failed for background image");
        fclose(file);
        return -1;
      }
    } else if (strncmp(line, "platform_count =", 16) == 0) {
      // Parse platform count.
      free(map->platforms); // Free old platforms if they exist.
      map->platformCount = atoi(line + 16);
      if (map->platformCount <= 0) {
        perror("Invalid platform count");
        fclose(file);
        return -1;
      }
      map->platforms = (Platform *)calloc(map->platformCount, sizeof(Platform));
      if (!map->platforms) {
        perror("Memory allocation failed for platforms");
        fclose(file);
        return -1;
      }
    } else if (currentPlatformIndex < map->platformCount) {
      // Use sscanf to extract the numbers after the platform index
      if (sscanf(line, "platform%d = {%d, %d, %d, %d}", // Adjusted the format
                                                        // to skip the index
                 &dummy, // Use a dummy int to skip the platform index number
                 &map->platforms[currentPlatformIndex].x,
                 &map->platforms[currentPlatformIndex].y,
                 &map->platforms[currentPlatformIndex].width,
                 &map->platforms[currentPlatformIndex].height) != 5) {
        // Handle parsing error
        perror("Platform parsing failed");
        fclose(file);
        return -1;
      }
      currentPlatformIndex++;
    } else {
      // We have more platforms than specified by platformCount
      perror("Platform count exceeded");
      fclose(file);
      return -1;
    }
  }

  if (currentPlatformIndex != map->platformCount) {
    // The actual number of platforms read doesn't match the count specified in
    // the file
    perror("Platform count mismatch");
    fclose(file);
    return -1;
  }

  fclose(file);
  return 0;
}

void freeMapResources(Map *map) {
  if (map) {
    free(map->backgroundImage);
    map->backgroundImage = NULL;
    free(map->platforms);
    map->platforms = NULL;
    map->platformCount = 0;
  }
}
