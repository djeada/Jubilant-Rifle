#include "map/map_generator.h"
#include "map/ladder.h"
#include "map/trap.h"
#include "map/flag.h"
#include "utils/consts.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <math.h>

void mapGenParamsInit(MapGenParams *params, int width, int height) {
  if (!params)
    return;

  params->width = width;
  params->height = height;
  params->platformCount = 40;
  params->minPlatformWidth = 200;
  params->maxPlatformWidth = 500;
  params->platformHeight = 30;
  params->verticalSpacing = 300;
  params->horizontalSpacing = 400;
  params->ladderCount = 20;
  params->trapCount = 15;
  params->flagCount = 5;
  params->style = MAP_GEN_STYLE_MIXED;
  params->seed = 0; // Will use time
}

int generatePlatforms(const MapGenParams *params, Map *map) {
  if (!params || !map)
    return -1;

  /* Initialize random seed */
  if (params->seed != 0) {
    srand(params->seed);
  } else {
    srand((unsigned int)time(NULL));
  }

  /* Allocate platforms */
  if (allocatePlatforms(map, params->platformCount) != 0)
    return -1;

  switch (params->style) {
  case MAP_GEN_STYLE_HORIZONTAL: {
    /* Generate platforms in horizontal rows */
    int platformsPerRow = (params->width / params->horizontalSpacing);
    int numRows = params->platformCount / platformsPerRow + 1;
    int idx = 0;

    for (int row = 0; row < numRows && idx < (int)params->platformCount; row++) {
      int y = params->height - 100 - (row * params->verticalSpacing);
      if (y < 100)
        break;

      for (int col = 0; col < platformsPerRow && idx < (int)params->platformCount; col++) {
        int x = 50 + (col * params->horizontalSpacing) + randomRange(-50, 50);
        int w = randomRange(params->minPlatformWidth, params->maxPlatformWidth);

        map->platforms[idx].x = x;
        map->platforms[idx].y = y + randomRange(-30, 30);
        map->platforms[idx].w = w;
        map->platforms[idx].h = params->platformHeight;
        idx++;
      }
    }
    break;
  }

  case MAP_GEN_STYLE_VERTICAL: {
    /* Generate platforms in vertical columns */
    int platformsPerColumn = (params->height / params->verticalSpacing);
    int numColumns = params->platformCount / platformsPerColumn + 1;
    int idx = 0;

    for (int col = 0; col < numColumns && idx < (int)params->platformCount; col++) {
      int x = 100 + (col * params->horizontalSpacing);
      if (x >= params->width - 300)
        break;

      for (int row = 0; row < platformsPerColumn && idx < (int)params->platformCount; row++) {
        int y = params->height - 100 - (row * params->verticalSpacing);
        if (y < 100)
          break;

        int w = randomRange(params->minPlatformWidth, params->maxPlatformWidth);

        map->platforms[idx].x = x + randomRange(-50, 50);
        map->platforms[idx].y = y;
        map->platforms[idx].w = w;
        map->platforms[idx].h = params->platformHeight;
        idx++;
      }
    }
    break;
  }

  case MAP_GEN_STYLE_SPIRAL: {
    /* Generate platforms in a spiral pattern */
    int centerX = params->width / 2;
    int centerY = params->height / 2;
    float angle = 0;
    float radius = 200;

    for (int i = 0; i < (int)params->platformCount; i++) {
      int x = centerX + (int)(radius * cos(angle)) - params->minPlatformWidth / 2;
      int y = centerY + (int)(radius * sin(angle));
      int w = randomRange(params->minPlatformWidth, params->maxPlatformWidth);

      /* Keep within bounds */
      if (x < 0) x = 0;
      if (x + w >= params->width) x = params->width - w - 50;
      if (y < 100) y = 100;
      if (y >= params->height - 100) y = params->height - 100;

      map->platforms[i].x = x;
      map->platforms[i].y = y;
      map->platforms[i].w = w;
      map->platforms[i].h = params->platformHeight;

      angle += 0.5f;
      radius += 15;
    }
    break;
  }

  case MAP_GEN_STYLE_MIXED:
  case MAP_GEN_STYLE_RANDOM:
  default: {
    /* Generate platforms randomly across the map */
    for (int i = 0; i < (int)params->platformCount; i++) {
      int x = randomRange(0, params->width - params->maxPlatformWidth);
      int y = randomRange(params->height / 4, params->height - 100);
      int w = randomRange(params->minPlatformWidth, params->maxPlatformWidth);

      /* Try to avoid excessive overlap */
      bool overlap = false;
      for (int j = 0; j < i; j++) {
        SDL_Rect check = {x, y, w, params->platformHeight};
        if (rectsOverlap(&check, &map->platforms[j])) {
          overlap = true;
          break;
        }
      }

      /* If overlap, try to adjust position */
      if (overlap) {
        x = randomRange(0, params->width - params->maxPlatformWidth);
        y = randomRange(params->height / 4, params->height - 100);
      }

      map->platforms[i].x = x;
      map->platforms[i].y = y;
      map->platforms[i].w = w;
      map->platforms[i].h = params->platformHeight;
    }
    break;
  }
  }

  return 0;
}

int generateLadders(Map *map, int ladderCount) {
  if (!map || ladderCount <= 0)
    return 0;

  LadderArray *ladders = (LadderArray *)map->ladders;
  if (!ladders)
    return -1;

  /* For each ladder, try to connect two platforms */
  for (int i = 0; i < ladderCount && i < (int)map->platformCount - 1; i++) {
    /* Pick a platform */
    int platformIdx = randomRange(0, (int)map->platformCount - 2);
    Platform *platform = &map->platforms[platformIdx];

    /* Find a platform below or above */
    Platform *nearPlatform = NULL;
    int minDist = 1000;

    for (size_t j = 0; j < map->platformCount; j++) {
      if (j == (size_t)platformIdx)
        continue;

      int dist = abs(map->platforms[j].y - platform->y);
      if (dist > 50 && dist < minDist && dist < 500) {
        minDist = dist;
        nearPlatform = &map->platforms[j];
      }
    }

    if (nearPlatform) {
      Ladder ladder;
      /* Position ladder at edge of platform */
      ladder.rect.x = platform->x + randomRange(platform->w / 4, 3 * platform->w / 4);
      ladder.rect.w = LADDER_WIDTH;

      /* Determine top and bottom */
      if (platform->y < nearPlatform->y) {
        ladder.rect.y = platform->y;
        ladder.rect.h = nearPlatform->y - platform->y;
        ladder.topPlatformY = platform->y;
        ladder.bottomPlatformY = nearPlatform->y;
      } else {
        ladder.rect.y = nearPlatform->y;
        ladder.rect.h = platform->y - nearPlatform->y;
        ladder.topPlatformY = nearPlatform->y;
        ladder.bottomPlatformY = platform->y;
      }

      ladderArrayAdd(ladders, &ladder);
    }
  }

  return 0;
}

int generateTraps(Map *map, int trapCount) {
  if (!map || trapCount <= 0)
    return 0;

  TrapArray *traps = (TrapArray *)map->traps;
  if (!traps)
    return -1;

  /* Place traps on or near platforms */
  for (int i = 0; i < trapCount && i < (int)map->platformCount; i++) {
    int platformIdx = randomRange(0, (int)map->platformCount - 1);
    Platform *platform = &map->platforms[platformIdx];

    Trap trap = {0};
    trap.type = TRAP_TYPE_FUEL_BARREL;
    trap.state = TRAP_STATE_INACTIVE;
    trap.damage = TRAP_FUEL_BARREL_DAMAGE;
    trap.health = 50;
    trap.effectRadius = 100.0f;
    trap.effectDuration = 3.0f;
    trap.damageInterval = 0.3f;
    trap.damageTimer = 0;
    trap.effectTimer = 0;

    /* Position on platform */
    trap.rect.x = platform->x + randomRange(50, platform->w - 90);
    trap.rect.y = platform->y - 50;
    trap.rect.w = 40;
    trap.rect.h = 50;

    trapArrayAdd(traps, &trap);
  }

  return 0;
}

int generateFlags(Map *map, int flagCount) {
  if (!map || flagCount <= 0)
    return 0;

  FlagArray *flags = (FlagArray *)map->flags;
  if (!flags)
    return -1;

  /* Distribute flags evenly across map */
  int platformStep = (int)map->platformCount / (flagCount + 1);
  if (platformStep < 1)
    platformStep = 1;

  for (int i = 0; i < flagCount && i * platformStep < (int)map->platformCount; i++) {
    int platformIdx = (i + 1) * platformStep;
    if (platformIdx >= (int)map->platformCount)
      platformIdx = (int)map->platformCount - 1;

    Platform *platform = &map->platforms[platformIdx];

    Flag flag = {0};
    flag.state = FLAG_STATE_UNCAPTURED;
    flag.captureProgress = 0.0f;
    flag.captureTime = FLAG_CAPTURE_TIME;
    flag.pointValue = FLAG_CAPTURE_POINTS;
    flag.isObjective = true;

    /* Position on platform */
    flag.rect.x = platform->x + platform->w / 2 - 40;
    flag.rect.y = platform->y - 100;
    flag.rect.w = 80;
    flag.rect.h = 100;

    flagArrayAdd(flags, &flag);
  }

  return 0;
}

int generateMap(const MapGenParams *params, Map *map) {
  if (!params || !map)
    return -1;

  /* Initialize map */
  mapInit(map);

  /* Set map dimensions */
  map->rect.x = 0;
  map->rect.y = 0;
  map->rect.w = params->width;
  map->rect.h = params->height;

  /* Set background */
  map->backgroundImage = allocateBackgroundImagePath("resources/textures/background.png");
  if (!map->backgroundImage)
    return -1;

  /* Generate platforms */
  if (generatePlatforms(params, map) != 0) {
    mapDestructor(map);
    return -1;
  }

  /* Initialize arrays for other elements */
  LadderArray *ladders = malloc(sizeof(LadderArray));
  if (ladders) {
    ladderArrayInit(ladders);
    map->ladders = ladders;
  }

  TrapArray *traps = malloc(sizeof(TrapArray));
  if (traps) {
    trapArrayInit(traps);
    map->traps = traps;
  }

  FlagArray *flags = malloc(sizeof(FlagArray));
  if (flags) {
    flagArrayInit(flags);
    map->flags = flags;
  }

  /* Generate other elements */
  generateLadders(map, params->ladderCount);
  generateTraps(map, params->trapCount);
  generateFlags(map, params->flagCount);

  return 0;
}

int saveMapToFile(const Map *map, const char *filePath) {
  if (!map || !filePath)
    return -1;

  FILE *file = fopen(filePath, "w");
  if (!file)
    return -1;

  /* Write JSON header */
  fprintf(file, "{\n");
  fprintf(file, "  \"background_image\": \"%s\",\n",
          map->backgroundImage ? map->backgroundImage : "resources/textures/background.png");
  fprintf(file, "  \"width\": %d,\n", map->rect.w);
  fprintf(file, "  \"height\": %d,\n", map->rect.h);

  /* Write platforms */
  fprintf(file, "  \"platforms\": [\n");
  for (size_t i = 0; i < map->platformCount; i++) {
    fprintf(file, "    { \"x\": %d, \"y\": %d, \"width\": %d, \"height\": %d }%s\n",
            map->platforms[i].x, map->platforms[i].y, map->platforms[i].w,
            map->platforms[i].h, (i < map->platformCount - 1) ? "," : "");
  }
  fprintf(file, "  ]");

  /* Write ladders if present */
  LadderArray *ladders = (LadderArray *)map->ladders;
  if (ladders && ladders->count > 0) {
    fprintf(file, ",\n  \"ladders\": [\n");
    for (size_t i = 0; i < ladders->count; i++) {
      fprintf(file, "    { \"x\": %d, \"y\": %d, \"width\": %d, \"height\": %d }%s\n",
              ladders->ladders[i].rect.x, ladders->ladders[i].rect.y,
              ladders->ladders[i].rect.w, ladders->ladders[i].rect.h,
              (i < ladders->count - 1) ? "," : "");
    }
    fprintf(file, "  ]");
  }

  /* Write traps if present */
  TrapArray *traps = (TrapArray *)map->traps;
  if (traps && traps->count > 0) {
    fprintf(file, ",\n  \"traps\": [\n");
    for (size_t i = 0; i < traps->count; i++) {
      fprintf(file, "    { \"x\": %d, \"y\": %d, \"width\": %d, \"height\": %d, \"type\": %d }%s\n",
              traps->traps[i].rect.x, traps->traps[i].rect.y, traps->traps[i].rect.w,
              traps->traps[i].rect.h, (int)traps->traps[i].type,
              (i < traps->count - 1) ? "," : "");
    }
    fprintf(file, "  ]");
  }

  /* Write flags if present */
  FlagArray *flags = (FlagArray *)map->flags;
  if (flags && flags->count > 0) {
    fprintf(file, ",\n  \"flags\": [\n");
    for (size_t i = 0; i < flags->count; i++) {
      fprintf(file, "    { \"x\": %d, \"y\": %d, \"width\": %d, \"height\": %d }%s\n",
              flags->flags[i].rect.x, flags->flags[i].rect.y, flags->flags[i].rect.w,
              flags->flags[i].rect.h, (i < flags->count - 1) ? "," : "");
    }
    fprintf(file, "  ]");
  }

  fprintf(file, "\n}\n");
  fclose(file);

  return 0;
}
