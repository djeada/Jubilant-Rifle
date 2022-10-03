#include "consts.h"
#include "map.h"

int parseMapConfig(const char *filename, Map *map);
Point findSection(char file_content[MAX_CONFIG_SIZE][MAX_CONFIG_SIZE],
                  int startIndex, const char *sectionName);
