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

  SDL_Surface *sheet;

  // create the the entities
  Humanoid man;
  humanoidConstructor(&man, 0, 310, 4, true, true, false, SHEET_PATH, renderer);

  Humanoid enemy;
  humanoidConstructor(&enemy, 390, 110, 4, true, true, true, ENEMY_A_PATH,
                      renderer);

  Vector bullets;
  vectorConstructor(&bullets, MAX_BULLETS, BULLET);

  // load textures
  SDL_Surface *bg;
  loadTexture(BACKGROUND_PATH, &bg);

  SDL_Texture *backgroundTexture;
  backgroundTexture = SDL_CreateTextureFromSurface(renderer, bg);
  SDL_FreeSurface(bg);

  SDL_Surface *bullet;
  loadTexture(BULLET_PATH, &bullet);

  SDL_Texture *bulletTexture;
  bulletTexture = SDL_CreateTextureFromSurface(renderer, bullet);
  SDL_FreeSurface(bullet);

  bool gameFlag = true;

  while (gameFlag) {
    gameFlag = processEvents(window, &man, &bullets);
    updateLogic(&man, &enemy, &bullets);
    doRender(renderer, &man, &enemy, &bullets, backgroundTexture,
             bulletTexture);

    SDL_Delay(10); // don't burn up the CPU
  }

  // Clean up
  SDL_DestroyWindow(window);
  SDL_DestroyRenderer(renderer);
  SDL_DestroyTexture(backgroundTexture);
  SDL_DestroyTexture(bulletTexture);

  humanoidDestructor(&man);
  humanoidDestructor(&enemy);
  clear(&bullets);
  SDL_Quit();
}