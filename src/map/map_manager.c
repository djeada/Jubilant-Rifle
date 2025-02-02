#include "map/map_manager.h"
#include <stdio.h>

void initMapManager(MapManager *manager, const char **mapPaths, size_t count) {
  manager->mapPaths = mapPaths;
  manager->mapCount = count;
  manager->currentIndex = 0;
}

int getNextMap(MapManager *manager, Map *map) {
  if (manager->mapCount == 0) {
    fprintf(stderr, "No map paths provided.\n");
    return -1;
  }

  // Get the current map file path
  const char *currentMapPath = manager->mapPaths[manager->currentIndex];

  // Parse the map file
  if (parseMapFile(currentMapPath, map) != 0) {
    fprintf(stderr, "Failed to load map: %s\n", currentMapPath);
    return -1;
  }

  // Move the counter forward in a circular fashion
  manager->currentIndex = (manager->currentIndex + 1) % manager->mapCount;

  return 0;
}
