#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <stdbool.h>
#include <stdio.h>

#include "consts.h"
#include "engine.h"
#include "game.h"
#include "utils.h"

void run_game() {

  SDL_Init(SDL_INIT_VIDEO);

  // get the screen width and height
  SDL_Rect screenRect;
  SDL_GetDisplayBounds(0, &screenRect);

  // create the application window
  SDL_Window *window =
      SDL_CreateWindow("Game Window",           // window title
                       SDL_WINDOWPOS_UNDEFINED, // initial x position
                       SDL_WINDOWPOS_UNDEFINED, // initial y position
                       screenRect.w,            // width, in pixels
                       screenRect.h,            // height, in pixels
                       0                        // flags
      );

  // setup the renderer
  SDL_Renderer *renderer =
      SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);
  SDL_RenderSetLogicalSize(renderer, 600, 400);

  // create the the entities
  Humanoid man;
  SDL_Texture *texture_a;
  loadTexture(SHEET_PATH, renderer, &texture_a);
  humanoidConstructor(&man, 0, 310, 4, true, true, false, texture_a);

  Vector enemies;
  vectorConstructor(&enemies, 10, HUMANOID);
  SDL_Texture *texture_b;
  loadTexture(ENEMY_A_PATH, renderer, &texture_b);

  for (int i = 0; i < 10; i++) {
    Humanoid enemy;
    humanoidConstructor(&enemy, i * 30, 110, 4, true, true, true, texture_b);
    append(&enemies, &enemy);
  }

  Vector bullets;
  vectorConstructor(&bullets, MAX_BULLETS, BULLET);

  // load textures
  SDL_Surface *bg;
  loadSurface(BACKGROUND_PATH, &bg);

  SDL_Texture *backgroundTexture;
  backgroundTexture = SDL_CreateTextureFromSurface(renderer, bg);
  SDL_FreeSurface(bg);

  SDL_Surface *bullet;
  loadSurface(BULLET_PATH, &bullet);

  SDL_Texture *bulletTexture;
  bulletTexture = SDL_CreateTextureFromSurface(renderer, bullet);
  SDL_FreeSurface(bullet);

  bool gameFlag = true;

  while (gameFlag) {
    gameFlag = processEvents(window, &man, &bullets);
    updateLogic(&man, &enemies, &bullets);
    doRender(renderer, &man, &enemies, &bullets, backgroundTexture,
             bulletTexture);

    SDL_Delay(10); // don't burn up the CPU
  }

  // Clean up
  SDL_DestroyWindow(window);
  SDL_DestroyRenderer(renderer);
  SDL_DestroyTexture(backgroundTexture);
  SDL_DestroyTexture(bulletTexture);
  SDL_DestroyTexture(texture_a);
  SDL_DestroyTexture(texture_b);

  humanoidDestructor(&man);
  clear(&enemies);
  clear(&bullets);
  SDL_Quit();
}