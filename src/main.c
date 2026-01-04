#include "game/game.h"
#include "rendering/render.h"
#include "utils/consts.h"
#include <SDL2/SDL.h>
#include <SDL2/SDL_ttf.h>
#include <stdio.h>

int initSDL() {
  if (SDL_Init(SDL_INIT_VIDEO) != 0) {
    printf("SDL_Init Error: %s\n", SDL_GetError());
    return 1;
  }
  if (TTF_Init() != 0) {
    fprintf(stderr, "TTF_Init Error: %s\n", TTF_GetError());
    SDL_Quit();
    return 1;
  }
  return 0;
}

SDL_Window *createWindow() {
  SDL_Window *win = SDL_CreateWindow(
      "Jubilant Rifle", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED,
      WINDOWED_MODE_WIDTH, WINDOWED_MODE_HEIGHT, 0);
  return win;
}

SDL_Renderer *createRenderer(SDL_Window *win) {
  SDL_Renderer *renderer =
      SDL_CreateRenderer(win, -1, SDL_RENDERER_ACCELERATED);
  return renderer;
}

void cleanup(SDL_Renderer *renderer, SDL_Window *win, int ttfInitialized,
             int sdlInitialized) {
  if (renderer) {
    SDL_DestroyRenderer(renderer);
  }
  if (win) {
    SDL_DestroyWindow(win);
  }
  if (ttfInitialized) {
    TTF_Quit();
  }
  if (sdlInitialized) {
    SDL_Quit();
  }
}

int main(int argc, char *argv[]) {
  (void)argc;
  (void)argv;
  int sdlInitialized = 0;
  int ttfInitialized = 0;
  SDL_Window *win = NULL;
  SDL_Renderer *renderer = NULL;

  // Initialize SDL and TTF
  if (initSDL() != 0) {
    return 1;
  }
  sdlInitialized = 1;
  ttfInitialized = 1;

  // Create the SDL window and renderer
  win = createWindow();
  if (!win) {
    cleanup(NULL, NULL, ttfInitialized, sdlInitialized);
    return 1;
  }

  renderer = createRenderer(win);
  if (!renderer) {
    cleanup(NULL, win, ttfInitialized, sdlInitialized);
    return 1;
  }

  // Run the game loop
  runGame(renderer);

  // Clean up all resources
  cleanup(renderer, win, ttfInitialized, sdlInitialized);

  return 0;
}
