#include "consts.h"
#include "map.h"

unsigned int parseMapConfig(const char *filename, Map *map);
Point findSection(char file_content[MAX_CONFIG_SIZE][MAX_CONFIG_SIZE],
                  unsigned int startIndex, const char *sectionName);
