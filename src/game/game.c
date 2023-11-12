#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <SDL2/SDL_ttf.h>
#include <stdio.h>
#include <stdlib.h>

#include "entities/humanoid.h"
#include "entities/humanoid_factories.h"
#include "game/event_handler.h"
#include "game/game.h"
#include "game/game_logic.h"
#include "map/map.h"
#include "rendering/render.h"
#include "utils/consts.h"
#include "utils/time_manager.h"
#include "utils/utils.h"

#define MAP_FILE_PATH "resources/maps/mountains.cfg"
#define FONT_PATH "resources/fonts/FreeSans.ttf"
#define WINDOW_TITLE "SDL Platformer"

#include "utils/resources.h"

void cleanup(SDL_Renderer *renderer, SDL_Window *window, Map *map,
             Humanoid *player) {
  mapDestructor(map);
  humanoidDestructor(player);
  freeResourcesInstance();
  SDL_DestroyRenderer(renderer);
  SDL_DestroyWindow(window);

  IMG_Quit(); // After destroying all IMG textures but before SDL_Quit
  SDL_Quit();
  TTF_Quit();
}

int initializeSDL(SDL_Window **window, SDL_Renderer **renderer) {
  if (SDL_Init(SDL_INIT_VIDEO) < 0) {
    fprintf(stderr, "SDL could not initialize! SDL_Error: %s\n",
            SDL_GetError());
    return -1;
  }

  SDL_DisplayMode current;
  SDL_GetCurrentDisplayMode(
      0, &current); // Get current display mode for the default display

  *window = SDL_CreateWindow(WINDOW_TITLE, SDL_WINDOWPOS_UNDEFINED,
                             SDL_WINDOWPOS_UNDEFINED, current.w,
                             current.h, // Use current display resolution
                             SDL_WINDOW_FULLSCREEN);

  if (!*window) {
    fprintf(stderr, "Window could not be created! SDL_Error: %s\n",
            SDL_GetError());
    SDL_Quit();
    return -1;
  }

  *renderer = SDL_CreateRenderer(*window, -1, SDL_RENDERER_ACCELERATED);
  if (!*renderer) {
    fprintf(stderr, "Renderer could not be created! SDL_Error: %s\n",
            SDL_GetError());
    SDL_DestroyWindow(*window);
    SDL_Quit();
    return -1;
  }

  // Set logical size for renderer
  setRenderLogicalSize(*renderer, DEFAULT_WINDOW_WIDTH, DEFAULT_WINDOW_HEIGHT);
  IMG_Init(IMG_INIT_PNG); // Initialize image library
  TTF_Init();             // Initialize TTF library

  return 0;
}

int initializeMap(Map *map) {
  if (parseMapFile(MAP_FILE_PATH, map) != 0) {
    fprintf(stderr, "Failed to load map.\n");
    return -1;
  }
  return 0;
}

bool initializeGame(SDL_Window **window, SDL_Renderer **renderer, Map *map,
                    Humanoid *player, Vector *enemies) {
  if (initializeSDL(window, renderer) != 0) {
    return false;
  }

  if (initializeMap(map) != 0) {
    cleanup(*renderer, *window, map, player);
    return false;
  }

  if (!initializeResourcesInstance(*renderer)) {
    return false;
  }

  createPlayerHumanoid(player);
  createEnemies(enemies, map);

  return true;
}

void runGame() {
  SDL_Window *window = NULL;
  SDL_Renderer *renderer = NULL;
  Map map = {0};
  Humanoid player = {0};
  Vector enemies = {0};
  Camera camera = {0, 0, 500, 500};

  if (!initializeGame(&window, &renderer, &map, &player, &enemies)) {
    return; // Initialization failed
  }

  timeManagerInit();
  int gameRunning = 1;

  while (gameRunning) {
    gameRunning = processEvents(window, &player);
    updateHumanoid(&player, &map);
    centerCameraOnPlayer(&camera, &player);
    render(renderer, &map, &player, &camera, &enemies);
    timeManagerUpdate();
    SDL_Delay(10); // Sleep to prevent CPU exhaustion
  }

  cleanup(renderer, window, &map, &player);
}
