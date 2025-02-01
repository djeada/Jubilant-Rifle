#include "game/game.h"
#include "rendering/render.h"
#include <SDL2/SDL.h>
#include <stdio.h>

int main(int argc, char *argv[]) {
  (void)argc;
  (void)argv;
  if (SDL_Init(SDL_INIT_VIDEO) != 0) {
    printf("SDL_Init Error: %s\n", SDL_GetError());
    return 1;
  }

  SDL_Window *win = SDL_CreateWindow("Game", SDL_WINDOWPOS_CENTERED,
                                     SDL_WINDOWPOS_CENTERED, 640, 480, 0);
  if (!win) {
    SDL_Quit();
    return 1;
  }

  SDL_Renderer *renderer =
      SDL_CreateRenderer(win, -1, SDL_RENDERER_ACCELERATED);
  if (!renderer) {
    SDL_DestroyWindow(win);
    SDL_Quit();
    return 1;
  }

  // Initialize textures (TextureManager is defined in render.h/render.c)
  TextureManager texManager = initTextureManager(renderer);

  // Run the game loop (which handles both menu and game states)
  runGame(renderer, &texManager);

  // Cleanup
  destroyTextureManager(&texManager);
  SDL_DestroyRenderer(renderer);
  SDL_DestroyWindow(win);
  SDL_Quit();
  return 0;
}
