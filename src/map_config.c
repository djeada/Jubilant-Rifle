#include "map_config.h"
#include <string.h>
#include <stdlib.h>

#define MAX_CONFIG_SIZE 1024


unsigned int parse_map_config(const char *filename, Map *map) {
    // Read the file to an array of strings where each string is a line
    char lines[MAX_CONFIG_SIZE][MAX_CONFIG_SIZE];
    unsigned int i = 0;

    FILE * file = fopen(filename, "r");
    while(fgets(lines[i], MAX_CONFIG_SIZE, file)) {
        // get rid of ending \n from fgets 
        lines[i][strlen(lines[i]) - 1] = '\0';
        i++;
    }
    // close the file
    fclose(file);

    unsigned line_count = i;

    // Parse the lines

    for (unsigned int i = 0; i < line_count; i++) {
        const char *line = lines[i];
        printf("%s \n", line);

    }

    map->numberOfLevels = 0;


    return 0;
}
