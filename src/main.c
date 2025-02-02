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
  if (!win) {
    SDL_Quit();
  }
  return win;
}

SDL_Renderer *createRenderer(SDL_Window *win) {
  SDL_Renderer *renderer =
      SDL_CreateRenderer(win, -1, SDL_RENDERER_ACCELERATED);
  if (!renderer) {
    SDL_DestroyWindow(win);
    SDL_Quit();
  }
  return renderer;
}

void cleanup(SDL_Renderer *renderer, SDL_Window *win) {
  SDL_DestroyRenderer(renderer);
  SDL_DestroyWindow(win);
  TTF_Quit();
  SDL_Quit();
}

int main(int argc, char *argv[]) {
  (void)argc;
  (void)argv;

  // Initialize SDL and TTF
  if (initSDL() != 0) {
    return 1;
  }

  // Create the SDL window and renderer
  SDL_Window *win = createWindow();
  if (!win)
    return 1;

  SDL_Renderer *renderer = createRenderer(win);
  if (!renderer)
    return 1;

  // Run the game loop
  runGame(renderer);

  // Clean up all resources
  cleanup(renderer, win);

  return 0;
}

