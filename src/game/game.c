#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <SDL2/SDL_ttf.h>
#include <stdio.h>
#include <stdlib.h>

#include "entities/humanoid.h"
#include "game/event_handler.h"
#include "game/game.h"
#include "game/game_logic.h"
#include "map/map.h"
#include "rendering/render.h"
#include "utils/time_manager.h"

#define MAP_FILE_PATH "resources/maps/mountains.cfg"
#define PLAYER_TEXTURE_PATH "resources/textures/player_b.png"
#define FONT_PATH "resources/fonts/FreeSans.ttf"
#define WINDOW_TITLE "SDL Platformer"
#define SCREEN_WIDTH 1920  // Your design width
#define SCREEN_HEIGHT 1080 // Your design height
TTF_Font *font = NULL;
void cleanup(SDL_Renderer *renderer, SDL_Window *window, Map *map,
             Humanoid *player) {
  freeMapResources(map);
  humanoidDestructor(player);
  SDL_DestroyRenderer(renderer);
  SDL_DestroyWindow(window);
  TTF_CloseFont(font);

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
  setRenderLogicalSize(*renderer, SCREEN_WIDTH, SCREEN_HEIGHT);
  IMG_Init(IMG_INIT_PNG);             // Initialize image library
  TTF_Init();                         // Initialize TTF library
  font = TTF_OpenFont(FONT_PATH, 12); // Load font

  return font ? 0 : -1;
}

int initializeMap(Map *map) {
  if (parseMapFile(MAP_FILE_PATH, map) != 0) {
    fprintf(stderr, "Failed to load map.\n");
    return -1;
  }
  return 0;
}

void initializeHumanoid(Humanoid *player, SDL_Renderer *renderer,
                        const char *texturePath) {
  SDL_Texture *playerTexture = NULL;
  loadTexture(renderer, texturePath, &playerTexture);
  if (!playerTexture) {
    fprintf(stderr, "Failed to load player texture.\n");
    return;
  }

  // Initialize humanoid properties
  int initialSpriteIndex = 0;
  bool isFacingLeft = false;
  bool isWalking = false;
  int initialPosX = 100;
  int initialPosY = 100;
  int movementStartX = 0;
  int movementEndX = 0;
  short initialLife = 100;
  bool isAlive = true;
  bool isVisible = true;

  humanoidConstructor(player, initialSpriteIndex, isFacingLeft, isWalking,
                      initialPosX, initialPosY, movementStartX, movementEndX,
                      playerTexture, initialLife, isAlive, isVisible);
}

int initializeGame(SDL_Window **window, SDL_Renderer **renderer, Map *map,
                   Humanoid *player) {
  if (initializeSDL(window, renderer) != 0) {
    return -1;
  }

  if (initializeMap(map) != 0) {
    cleanup(*renderer, *window, map, player);
    return -1;
  }

  initializeHumanoid(player, *renderer, PLAYER_TEXTURE_PATH);

  return 0;
}

void runGame() {
  SDL_Window *window = NULL;
  SDL_Renderer *renderer = NULL;
  Map map = {0};
  Humanoid player;
  Camera camera = {0, 0, 500, 500};

  timeManagerInit();

  if (initializeGame(&window, &renderer, &map, &player) != 0) {
    return; // Initialization failed
  }

  int gameRunning = 1;
  while (gameRunning) {
    gameRunning = processEvents(window, &player);
    updateHumanoid(&player, &map);
    centerCameraOnPlayer(&camera, &player);
    render(renderer, &map, &player, &camera);
    timeManagerUpdate();
    SDL_Delay(10); // Sleep to prevent CPU exhaustion
  }

  cleanup(renderer, window, &map, &player);
}