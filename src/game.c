#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <stdbool.h>
#include <stdio.h>

#include "consts.h"
#include "game.h"

int globalTime = 0;
SDL_Texture *texture_a;
SDL_Texture *texture_b;
SDL_Texture *backgroundTexture;
SDL_Texture *bulletTexture;

bool processEvents(SDL_Window *window, Humanoid *player, Vector *bullets) {
  SDL_Event event;

  while (SDL_PollEvent(&event)) {
    switch (event.type) {
    case SDL_WINDOWEVENT_CLOSE: {
      if (window) {
        SDL_DestroyWindow(window);
        window = NULL;
        return false;
      }

    } break;
    case SDL_KEYDOWN: {
      switch (event.key.keysym.sym) {
      case SDLK_ESCAPE:
        return false;
      }
    } break;
    case SDL_QUIT:
      return false;
    }
  }

  const unsigned char *state = SDL_GetKeyboardState(NULL);
  if (!player->shooting) {
    if (state[SDL_SCANCODE_LEFT]) {
      moveLeft(player);

      if (globalTime % 6 == 0) {
        incrementSprite(player);
      }
    } else if (state[SDL_SCANCODE_RIGHT]) {
      moveRight(player);

      if (globalTime % 6 == 0) {
        incrementSprite(player);
      }
    } else {
      stop(player);
    }

    if (state[SDL_SCANCODE_UP]) {
      jump(player);
    }
  }

  if (!player->walking) {
    if (state[SDL_SCANCODE_SPACE]) // && !player->dy)
    {
      if (globalTime % 6 == 0) {
        shoot(player, bullets);
      }

    } else {
      player->currentSprite = 4;
      player->shooting = 0;
    }
  }

  return true;
}

void render(SDL_Renderer *renderer, Humanoid *player, Vector *enemies,
            Vector *bullets, SDL_Texture *backgroundTexture,
            SDL_Texture *bulletTexture) {

  SDL_SetRenderDrawColor(renderer, 0, 0, 0,
                         0); // set the drawing color to black
  SDL_RenderClear(renderer);
  SDL_RenderCopy(renderer, backgroundTexture, NULL, NULL);

  // render player
  if (player->visible) {
    SDL_Rect srcRect = {HUMANOID_WIDTH * player->currentSprite, 0,
                        HUMANOID_WIDTH, HUMANOID_HEIGHT};
    SDL_Rect rect = {player->position.x, player->position.y, HUMANOID_WIDTH,
                     HUMANOID_HEIGHT};
    SDL_RenderCopyEx(renderer, player->texture, &srcRect, &rect, 0, NULL,
                     player->facingLeft);
  }

  // render enemies
  for (int i = 0; i < enemies->size; i++) {
    Humanoid *enemy = &((Humanoid*) enemies->data)[i];
    if (enemy->visible) {
      SDL_Rect srcRect = {HUMANOID_WIDTH * enemy->currentSprite, 0,
                          HUMANOID_WIDTH, HUMANOID_HEIGHT};
      SDL_Rect rect = {enemy->position.x, enemy->position.y, HUMANOID_WIDTH,
                       HUMANOID_HEIGHT};
      SDL_RenderCopyEx(renderer, enemy->texture, &srcRect, &rect, 0, NULL,
                       enemy->facingLeft);
    }
  }

  // render bullets
  for (int i = 0; i < bullets->size - 1; i++) {
    Bullet *bullet = &((Bullet*) bullets->data)[i];
    SDL_Rect rect = {bullet->position.x, bullet->position.y, BULLET_WIDTH,
                     BULLET_HEIGHT};
    SDL_RenderCopy(renderer, bulletTexture, NULL, &rect);
  }

  SDL_RenderPresent(renderer);
}

void updateLogic(Humanoid *player, Vector *enemies, Vector *bullets) {

  moveHumanoid(player);

  for (int i = 0; i < bullets->size; i++) {
    Bullet *bullet = &((Bullet*) bullets->data)[i];

    moveBullet(bullet);

    if (bulletOutOfScreen(bullet)) {
      removeFromVector(bullets, i);
      continue;
    }

    for (int j = 0; j < enemies->size; j++) {
      Humanoid *enemy = &((Humanoid*) enemies->data)[j];

      if (collidesWithBullet(enemy, bullet)) {
        die(enemy);
        removeFromVector(bullets, i);
        break;
      }

      if (globalTime % 15 == 0) {

        if (!enemy->alive && enemy->visible) {

          hide(enemy);
          // removeFromVector(enemies, j);
          // j--;
          break;
        }
      }
    }
  }

  globalTime++;
}

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
  SDL_RenderSetLogicalSize(renderer, BOARD_WIDTH, BOARD_HEIGHT);

  // create the the entities
  Humanoid player;
  loadTexture(SHEET_PATH, renderer, &texture_a);
  humanoidConstructor(&player, texture_a, createPoint(0, 0), false, 4, true,
                      true);

  Vector enemies;
  vectorConstructor(&enemies, 10, HUMANOID);
  loadTexture(ENEMY_A_PATH, renderer, &texture_b);

  for (int i = 0; i < 10; i++) {
    Humanoid enemy;
    humanoidConstructor(&enemy, texture_b, createPoint(i * 30, 110), true, 4,
                        true, true);
    append(&enemies, &enemy);
  }

  Vector bullets;
  vectorConstructor(&bullets, MAX_BULLETS, BULLET);

  // load textures
  SDL_Surface *bg;
  loadSurface(BACKGROUND_PATH, &bg);

  backgroundTexture = SDL_CreateTextureFromSurface(renderer, bg);
  SDL_FreeSurface(bg);

  SDL_Surface *bullet;
  loadSurface(BULLET_PATH, &bullet);

  bulletTexture = SDL_CreateTextureFromSurface(renderer, bullet);
  SDL_FreeSurface(bullet);

  bool gameFlag = true;

  while (gameFlag) {
    gameFlag = processEvents(window, &player, &bullets);
    updateLogic(&player, &enemies, &bullets);
    render(renderer, &player, &enemies, &bullets, backgroundTexture,
           bulletTexture);

    SDL_Delay(10); // don't burn up the CPU
  }

  // cleanup
  SDL_DestroyWindow(window);
  SDL_DestroyRenderer(renderer);
  SDL_DestroyTexture(backgroundTexture);
  SDL_DestroyTexture(bulletTexture);
  SDL_DestroyTexture(texture_a);
  SDL_DestroyTexture(texture_b);

  humanoidDestructor(&player);
  clear(&enemies);
  clear(&bullets);
  SDL_Quit();
}
