#ifndef MAP_MANAGER_H
#define MAP_MANAGER_H

#include "map/map.h"

typedef struct {
    const char **mapPaths;  // Array of file paths (strings)
    size_t mapCount;        // Number of maps available
    size_t currentIndex;    // Index of the next map to load
} MapManager;


void initMapManager(MapManager *manager, const char **mapPaths, size_t count);
int getNextMap(MapManager *manager, Map *map);

#endif  // MAP_MANAGER_H
