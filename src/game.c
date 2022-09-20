#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <stdbool.h>
#include <stdio.h>

#include "consts.h"
#include "game.h"
int globalTime = 0;

bool processEvents(SDL_Window *window, Humanoid *man, Vector *bullets) {
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
  if (!man->shooting) {
    if (state[SDL_SCANCODE_LEFT]) {
      moveLeft(man);

      if (globalTime % 6 == 0) {
        incrementSprite(man);
      }
    } else if (state[SDL_SCANCODE_RIGHT]) {
      moveRight(man);

      if (globalTime % 6 == 0) {
        incrementSprite(man);
      }
    } else {
      stop(man);
    }

    if (state[SDL_SCANCODE_UP]) {
      jump(man);
    }
  }

  if (!man->walking) {
    if (state[SDL_SCANCODE_SPACE]) // && !man->dy)
    {
      if (globalTime % 6 == 0) {
        shoot(man, bullets);
      }

    } else {
      man->currentSprite = 4;
      man->shooting = 0;
    }
  }

  return true;
}

void render(SDL_Renderer *renderer, Humanoid *man, Vector *enemies,
            Vector *bullets, SDL_Texture *backgroundTexture,
            SDL_Texture *bulletTexture) {

  SDL_SetRenderDrawColor(renderer, 0, 0, 0,
                         0); // set the drawing color to black
  SDL_RenderClear(renderer);
  SDL_RenderCopy(renderer, backgroundTexture, NULL, NULL);

  // warrior
  if (man->visible) {
    SDL_Rect srcRect = {40 * man->currentSprite, 0, 40, 50};
    SDL_Rect rect = {man->position.x, man->position.y, 40, 50};
    SDL_RenderCopyEx(renderer, man->texture, &srcRect, &rect, 0, NULL,
                     man->facingLeft);
  }

  // enemy

  for (int i = 0; i < enemies->size; i++) {
    if (((Humanoid *)enemies->data[i])->visible) {
      SDL_Rect eSrcRect = {40 * ((Humanoid *)enemies->data[i])->currentSprite,
                           0, 40, 50};
      SDL_Rect eRect = {((Humanoid *)enemies->data[i])->position.x,
                        ((Humanoid *)enemies->data[i])->position.y, 40, 50};
      SDL_RenderCopyEx(renderer, ((Humanoid *)enemies->data[i])->texture,
                       &eSrcRect, &eRect, 0, NULL,
                       ((Humanoid *)enemies->data[i])->facingLeft);
    }
  }
  for (int i = 0; i < MAX_BULLETS; i++)
    if (bullets->data[i]) {
      SDL_Rect rect = {((Bullet *)bullets->data[i])->position.x,
                       ((Bullet *)bullets->data[i])->position.y, 8, 8};
      SDL_RenderCopy(renderer, bulletTexture, NULL, &rect);
    }

  SDL_RenderPresent(renderer);
}

void updateLogic(Humanoid *man, Vector *enemies, Vector *bullets) {

  moveHumanoid(man);

  for (int i = 0; i < bullets->size; i++) {

    moveBullet(((Bullet *)bullets->data[i]));

    if (bulletOutOfScreen(((Bullet *)bullets->data[i]))) {
      removeFromVector(bullets, i);
      continue;
    }

    for (int j = 0; j < enemies->size; j++) {

      if (collidesWithBullet(((Humanoid *)enemies->data[j]),
                             ((Bullet *)bullets->data[i]))) {
        die(((Humanoid *)enemies->data[j]));
        removeFromVector(bullets, i);
        break;
      }

      if (globalTime % 15 == 0) {

        if (!((Humanoid *)enemies->data[j])->alive &&
            ((Humanoid *)enemies->data[j])->visible) {

          ((Humanoid *)enemies->data[j])->visible = false;
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
  Humanoid man;
  SDL_Texture *texture_a;
  loadTexture(SHEET_PATH, renderer, &texture_a);
  Point position;
  pointConstructor(&position, 0, 310);
  humanoidConstructor(&man, position, 4, true, true, false, texture_a);

  Vector enemies;
  vectorConstructor(&enemies, 10, HUMANOID);
  SDL_Texture *texture_b;
  loadTexture(ENEMY_A_PATH, renderer, &texture_b);

  for (int i = 0; i < 10; i++) {
    Humanoid enemy;
    pointConstructor(&position, i * 30, 110);
    humanoidConstructor(&enemy, position, 4, true, true, true, texture_b);
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
    render(renderer, &man, &enemies, &bullets, backgroundTexture,
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