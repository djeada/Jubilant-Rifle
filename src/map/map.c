#include "map/map.h"
#include "utils/consts.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

/* Helper: read the entire contents of a file into a dynamically allocated string.
   Returns the string on success (which the caller must free) or NULL on failure. */
static char *readFileContents(const char *filePath) {
    FILE *file = fopen(filePath, "rb");
    if (!file)
        return NULL;
    fseek(file, 0, SEEK_END);
    long length = ftell(file);
    fseek(file, 0, SEEK_SET);
    char *data = malloc(length + 1);
    if (!data) {
        fclose(file);
        return NULL;
    }
    fread(data, 1, length, file);
    data[length] = '\0';
    fclose(file);
    return data;
}

Point getRandomPositionOnPlatform(const Platform *platform) {
    // Ensure there is room for the humanoid frame on this platform.
    int maxOffset = platform->w - HUMANOID_FRAME_WIDTH;
    if (maxOffset <= 0) {
        maxOffset = 0;
    }
    int random_x = platform->x + (rand() % (maxOffset + 1));
    int y = platform->y - HUMANOID_FRAME_HEIGHT;
    Point position = { random_x, y };
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
    // Skip the colon and any following whitespace.
    start++;
    while (*start == ' ' || *start == '\t')
        start++;
    if (*start == '"')
        start++; // skip opening quote
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
    // Use strtol for robust conversion.
    char *endptr;
    long num = strtol(start + 1, &endptr, 10);
    if (start + 1 == endptr) {
        return -1;
    }
    *value = (int)num;
    return 0;
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
    char *data = readFileContents(filePath);
    if (!data) {
        perror("Error reading file");
        return -1;
    }

    // Parse the background image path.
    char backgroundImagePath[256];
    if (parseStringValue(data, "\"background_image\"", backgroundImagePath, sizeof(backgroundImagePath)) != 0) {
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

    // Parse map dimensions.
    int width, height;
    if (parseIntValue(data, "\"width\"", &width) != 0 ||
        parseIntValue(data, "\"height\"", &height) != 0) {
        fprintf(stderr, "Failed to parse map dimensions\n");
        free(data);
        return -1;
    }
    map->rect.x = 0;
    map->rect.y = 0;
    map->rect.w = width;
    map->rect.h = height;

    // For this example, we expect there to be 20 platforms.
    size_t platformCount = 20;
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
