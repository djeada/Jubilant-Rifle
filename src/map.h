#include "utils.h"

typedef struct {
    unsigned int startX;
    unsigned int endX;
    unsigned int y;
} Platform;

typedef struct {
    unsigned int startHeight;
    unsigned int endHeight;
    unsigned int numberOfPlatforms;
    Platform* platforms;
} Level;

typedef struct {
    unsigned int numberOfLevels;
    Level* levels;
} Map;

unsigned int coordinatesToGroundLevel(Map* map, Point* position);
