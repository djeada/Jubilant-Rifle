#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <stdbool.h>
#include <stdio.h>

#include "consts.h"
#include "game.h"
#include "utils.h"

void run_game() {

  SDL_Init(SDL_INIT_VIDEO);

  Bullet *bullets[MAX_BULLETS] = {NULL};

  // get screen width and height
  SDL_Rect screenRect;
  SDL_GetDisplayBounds(0, &screenRect);

  // Create an application window with the following settings:
  SDL_Window *window =
      SDL_CreateWindow("Game Window",           // window title
                       SDL_WINDOWPOS_UNDEFINED, // initial x position
                       SDL_WINDOWPOS_UNDEFINED, // initial y position
                       screenRect.w,            // width, in pixels
                       screenRect.h,            // height, in pixels
                       0                        // flags
      );
  SDL_Renderer *renderer =
      SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);

  SDL_RenderSetLogicalSize(renderer, 600, 400);

  SDL_Surface *sheet;

  Humanoid man;
  humanoidConstructor(&man, 0, 310, 4, true, true, false, SHEET_PATH, renderer);

  Humanoid enemy;
  humanoidConstructor(&enemy, 390, 110, 4, true, true, true, ENEMY_A_PATH,
                      renderer);

  // load the bg
  SDL_Surface *bg;
  loadTexture(BACKGROUND_PATH, &bg);

  SDL_Texture *backgroundTexture;
  backgroundTexture = SDL_CreateTextureFromSurface(renderer, bg);
  SDL_FreeSurface(bg);

  // load the bullet
  SDL_Surface *bullet;
  loadTexture(BULLET_PATH, &bullet);

  SDL_Texture *bulletTexture;
  bulletTexture = SDL_CreateTextureFromSurface(renderer, bullet);
  SDL_FreeSurface(bullet);

  // The window is open: enter program loop (see SDL_PollEvent)
  bool gameFlag = true;

  while (gameFlag) {
    gameFlag = processEvents(window, &man, bullets);
    updateLogic(&man, &enemy, bullets);
    doRender(renderer, &man, &enemy, bullets, backgroundTexture, bulletTexture);

    SDL_Delay(10); // don't burn up the CPU
  }

  // Clean up
  SDL_DestroyWindow(window);
  SDL_DestroyRenderer(renderer);
  SDL_DestroyTexture(backgroundTexture);
  SDL_DestroyTexture(bulletTexture);

  humanoidDestructor(&man);
  humanoidDestructor(&enemy);
  cleanupBullets(bullets);
  SDL_Quit();
}