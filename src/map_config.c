#include "map_config.h"
#include <stdlib.h>
#include <string.h>

void fillValueFromConfig(char file_content[MAX_CONFIG_SIZE][MAX_CONFIG_SIZE],
                         unsigned int startIndex, unsigned int endIndex,
                         const char *keyWord, unsigned int *value) {
  for (unsigned int i = startIndex; i < endIndex; i++) {
    const char *line = file_content[i];
    if (strstr(line, keyWord) != NULL) {
      *value = numberFromString(line);
      break;
    }
  }
}

void parsePlatforms(char file_content[MAX_CONFIG_SIZE][MAX_CONFIG_SIZE],
                    unsigned int startIndex, unsigned int endIndex,
                    unsigned int levelIndex, unsigned int platformLevel,
                    Map *map) {
  fillValueFromConfig(file_content, startIndex, endIndex, startXString,
                      &map->levels[levelIndex].platforms[platformLevel].startX);
  fillValueFromConfig(file_content, startIndex, endIndex, endXString,
                      &map->levels[levelIndex].platforms[platformLevel].endX);
  fillValueFromConfig(file_content, startIndex, endIndex, platformYString,
                      &map->levels[levelIndex].platforms[platformLevel].y);
}

void parseLevel(char file_content[MAX_CONFIG_SIZE][MAX_CONFIG_SIZE],
                unsigned int startIndex, unsigned int endIndex,
                unsigned int levelIndex, Map *map) {
  fillValueFromConfig(file_content, startIndex, endIndex, startHeightString,
                      &map->levels[levelIndex].startHeight);
  fillValueFromConfig(file_content, startIndex, endIndex, endHeightString,
                      &map->levels[levelIndex].endHeight);
  fillValueFromConfig(file_content, startIndex, endIndex,
                      numberOfPlatformsString,
                      &map->levels[levelIndex].numberOfPlatforms);
  map->levels[levelIndex].platforms =
      malloc(sizeof(Platform) * map->levels[levelIndex].numberOfPlatforms);
  unsigned int lastPlatformIndex = startIndex;
  for (unsigned int i = 0; i < map->levels[levelIndex].numberOfPlatforms; i++) {
    Point platformSection =
        findSection(file_content, lastPlatformIndex, PLATFORM_STRING);
    lastPlatformIndex = platformSection.y;
    parsePlatforms(file_content, platformSection.x, platformSection.y,
                   levelIndex, i, map);
  }
}

void parseMap(char file_content[MAX_CONFIG_SIZE][MAX_CONFIG_SIZE],
              unsigned int startIndex, unsigned int endIndex, Map *map) {
  fillValueFromConfig(file_content, startIndex, endIndex, numberOfLevelsString,
                      &map->numberOfLevels);
  map->levels = malloc(sizeof(Level) * map->numberOfLevels);
  unsigned int lastLevelIndex = startIndex;
  for (unsigned int i = 0; i < map->numberOfLevels; i++) {
    Point levelSection =
        findSection(file_content, lastLevelIndex, LEVEL_STRING);
    lastLevelIndex = levelSection.y;
    parseLevel(file_content, levelSection.x, levelSection.y, i, map);
  }
}

Point findSection(char file_content[MAX_CONFIG_SIZE][MAX_CONFIG_SIZE],
                  unsigned int startIndex, const char *sectionName) {
  // section starts with the first '{' after the section name
  // section ends with the '}' that closes the section
  // there could be multiple opening brackets for smaller sections inside the
  // section we want to find boundries i.e. the index in the file_content array
  // of the first '{' and the last '}' of the section

  Point sectionBoundries = createPoint(-1, -1);
  int sectionStart = -1;
  int sectionEnd = -1;
  int sectionDepth = -1;

  unsigned int i = 0;
  for (i = startIndex; i < MAX_CONFIG_SIZE; i++) {
    const char *line = file_content[i];
    if (strstr(line, sectionName) != NULL) {
      sectionStart = i;
      if (strstr(line, openingBracket) != NULL) {
        sectionDepth++;
      }
      break;
    }
  }

  for (unsigned int j = i + 1; j < MAX_CONFIG_SIZE; j++) {
    const char *line = file_content[j];
    if (strstr(line, openingBracket) != NULL) {
      sectionDepth++;
    }
    if (strstr(line, closingBracket) != NULL) {

      if (sectionDepth == 0) {
        sectionEnd = j;
        break;
      }
      sectionDepth--;
    }
  }

  sectionBoundries.x = sectionStart;
  sectionBoundries.y = sectionEnd;
  return sectionBoundries;
}

unsigned int parseMapConfig(const char *filename, Map *map) {
  // Read the file to an array of strings where each string is a line
  char lines[MAX_CONFIG_SIZE][MAX_CONFIG_SIZE];
  unsigned int i = 0;

  FILE *file = fopen(filename, "r");
  while (fgets(lines[i], MAX_CONFIG_SIZE, file)) {
    // get rid of ending \n from fgets
    lines[i][strlen(lines[i]) - 1] = '\0';
    i++;
  }

  // Close the file
  fclose(file);

  unsigned lineCount = i;

  parseMap(lines, 0, lineCount, map);

  return 0;
}
