/**
 * @file map_gen_example.c
 * @brief Example program demonstrating map generation functionality.
 *
 * This example shows how to use the map generator to create procedural maps
 * with different styles and parameters. The generated maps are saved to files
 * that can be used in the game.
 *
 * Compile with: gcc -o map_gen_example map_gen_example.c -I../include -lm
 * Run with: ./map_gen_example
 */

#include "map/map_generator.h"
#include "map/map.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

void printUsage(const char *program) {
  printf("Map Generator Example\n");
  printf("Usage: %s [style] [output_file]\n\n", program);
  printf("Styles:\n");
  printf("  horizontal - Platforms arranged in horizontal rows\n");
  printf("  vertical   - Platforms arranged in vertical columns\n");
  printf("  spiral     - Platforms arranged in a spiral pattern\n");
  printf("  mixed      - Mixed horizontal and vertical (default)\n");
  printf("  random     - Random platform placement\n\n");
  printf("Example: %s horizontal resources/maps/generated_horizontal.cfg\n", program);
}

int main(int argc, char *argv[]) {
  MapGenParams params;
  Map map;
  MapGenStyle style = MAP_GEN_STYLE_MIXED;
  const char *outputFile = "resources/maps/generated.cfg";

  /* Parse command line arguments */
  if (argc >= 2) {
    const char *styleStr = argv[1];
    if (strcmp(styleStr, "horizontal") == 0) {
      style = MAP_GEN_STYLE_HORIZONTAL;
    } else if (strcmp(styleStr, "vertical") == 0) {
      style = MAP_GEN_STYLE_VERTICAL;
    } else if (strcmp(styleStr, "spiral") == 0) {
      style = MAP_GEN_STYLE_SPIRAL;
    } else if (strcmp(styleStr, "mixed") == 0) {
      style = MAP_GEN_STYLE_MIXED;
    } else if (strcmp(styleStr, "random") == 0) {
      style = MAP_GEN_STYLE_RANDOM;
    } else if (strcmp(styleStr, "help") == 0 || strcmp(styleStr, "-h") == 0) {
      printUsage(argv[0]);
      return 0;
    } else {
      fprintf(stderr, "Unknown style: %s\n", styleStr);
      printUsage(argv[0]);
      return 1;
    }
  }

  if (argc >= 3) {
    outputFile = argv[2];
  }

  /* Initialize generation parameters */
  printf("Initializing map generation parameters...\n");
  mapGenParamsInit(&params, 6400, 3600);
  params.style = style;
  params.platformCount = 50;
  params.ladderCount = 25;
  params.trapCount = 20;
  params.flagCount = 5;

  printf("Generating map with style: ");
  switch (style) {
  case MAP_GEN_STYLE_HORIZONTAL:
    printf("HORIZONTAL\n");
    break;
  case MAP_GEN_STYLE_VERTICAL:
    printf("VERTICAL\n");
    break;
  case MAP_GEN_STYLE_SPIRAL:
    printf("SPIRAL\n");
    break;
  case MAP_GEN_STYLE_MIXED:
    printf("MIXED\n");
    break;
  case MAP_GEN_STYLE_RANDOM:
    printf("RANDOM\n");
    break;
  }

  printf("Map dimensions: %dx%d\n", params.width, params.height);
  printf("Platform count: %d\n", params.platformCount);
  printf("Ladder count: %d\n", params.ladderCount);
  printf("Trap count: %d\n", params.trapCount);
  printf("Flag count: %d\n\n", params.flagCount);

  /* Generate the map */
  printf("Generating map...\n");
  if (generateMap(&params, &map) != 0) {
    fprintf(stderr, "Failed to generate map\n");
    return 1;
  }

  printf("Map generated successfully!\n");
  printf("  Platforms: %zu\n", map.platformCount);
  
  if (map.ladders) {
    LadderArray *ladders = (LadderArray *)map.ladders;
    printf("  Ladders: %zu\n", ladders->count);
  }
  
  if (map.traps) {
    TrapArray *traps = (TrapArray *)map.traps;
    printf("  Traps: %zu\n", traps->count);
  }
  
  if (map.flags) {
    FlagArray *flags = (FlagArray *)map.flags;
    printf("  Flags: %zu\n\n", flags->count);
  }

  /* Save to file */
  printf("Saving map to: %s\n", outputFile);
  if (saveMapToFile(&map, outputFile) != 0) {
    fprintf(stderr, "Failed to save map to file\n");
    mapDestructor(&map);
    return 1;
  }

  printf("Map saved successfully!\n");
  printf("\nYou can now use this map in the game by adding it to the MAPS\n");
  printf("constant in include/utils/consts.h\n");

  /* Cleanup */
  mapDestructor(&map);

  return 0;
}
