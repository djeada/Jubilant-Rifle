#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <SDL2/SDL_mixer.h>
#include <SDL2/SDL_ttf.h>
#include <pthread.h>
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
#include "utils/resources.h"
#include "utils/time_manager.h"
#include "utils/utils.h"

Mix_Music *bgMusic = NULL;
pthread_t musicThread;

void cleanup(SDL_Renderer *renderer, SDL_Window *window, Map *map,
             Player *player) {
  Mix_HaltMusic();
  Mix_FreeMusic(bgMusic);
  bgMusic = NULL;
  Mix_CloseAudio();
  Mix_Quit();

  mapDestructor(map);
  humanoidDestructor(&player->base);
  freeResourcesInstance();
  SDL_DestroyRenderer(renderer);
  SDL_DestroyWindow(window);

  IMG_Quit();
  SDL_Quit();
  TTF_Quit();
}

int initializeSDL(SDL_Window **window, SDL_Renderer **renderer) {
  if (SDL_Init(SDL_INIT_VIDEO | SDL_INIT_AUDIO) < 0) {
    fprintf(stderr, "SDL could not initialize! SDL_Error: %s\n",
            SDL_GetError());
    return -1;
  }

  SDL_DisplayMode current;
  SDL_GetCurrentDisplayMode(0, &current);

  *window = SDL_CreateWindow(WINDOW_TITLE, SDL_WINDOWPOS_UNDEFINED,
                             SDL_WINDOWPOS_UNDEFINED, current.w, current.h,
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

  setRenderLogicalSize(*renderer, DEFAULT_WINDOW_WIDTH, DEFAULT_WINDOW_HEIGHT);
  IMG_Init(IMG_INIT_PNG);
  TTF_Init();

  if (Mix_OpenAudio(44100, MIX_DEFAULT_FORMAT, 2, 2048) < 0) {
    fprintf(stderr, "SDL_mixer could not initialize! SDL_mixer Error: %s\n",
            Mix_GetError());
    return -1;
  }

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
                    Player *player, Vector *enemies) {
  if (initializeSDL(window, renderer) != 0) {
    return false;
  }

  if (initializeMap(map) != 0) {
    cleanup(*renderer, *window, map, player);
    return false;
  }

  if (!initializeResourcesInstance(*renderer)) {
    cleanup(*renderer, *window, map, player);
    return false;
  }

  playerFactory(player);
  createEnemies(enemies, map);

  bgMusic = Mix_LoadMUS("resources/audio/entangled_life_lish_grooves.mp3");
  if (bgMusic == NULL) {
    logError("Failed to load background music! SDL_mixer Error: ");
    logError(Mix_GetError());
    return false;
  }

  return true;
}

void *playMusic(void *arg) {
  (void)arg;
  Mix_PlayMusic(bgMusic, -1);
  return NULL;
}

void runGame() {
  SDL_Window *window = NULL;
  SDL_Renderer *renderer = NULL;
  Map map = {0};
  Player player = {0};
  Vector enemies = {0};
  Camera camera = {0, 0, 500, 500};

  if (!initializeGame(&window, &renderer, &map, &player, &enemies)) {
    return;
  }

  pthread_create(&musicThread, NULL, playMusic, NULL);

  timeManagerInit();
  int gameRunning = 1;

  while (gameRunning) {
    gameRunning = processEvents(window, &player);
    updatePlayerState(&player, &enemies, &map);
    updateEnemies(&enemies, &player);
    centerCameraOnPlayer(&camera, &player.base, &map);
    render(renderer, &map, &player, &camera, &enemies);
    timeManagerUpdate();
    SDL_Delay(10);
  }

  cleanup(renderer, window, &map, &player);
  pthread_cancel(musicThread);
  pthread_join(musicThread, NULL);
}
