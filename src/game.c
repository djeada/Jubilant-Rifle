#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <SDL2/SDL_ttf.h>
#include <stdbool.h>
#include <stdio.h>

#include "consts.h"
#include "game.h"

int globalTime = 0;
SDL_Texture *backgroundTexture;
SDL_Texture *bulletTexture;
TTF_Font *font;

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

      if (globalTime % TIME_INTERVAL_B == 0) {
        incrementSprite(player);
      }
    } else if (state[SDL_SCANCODE_RIGHT]) {
      moveRight(player);

      if (globalTime % TIME_INTERVAL_B == 0) {
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
      if (globalTime % TIME_INTERVAL_B == 0) {
        shoot(player, bullets);
      }
    } else {
      player->currentSprite = 4;
      player->shooting = 0;
    }
  }

  return true;
}

void updateLogic(Map *map, Humanoid *player, Vector *enemies, Vector *bullets) {

  // update player
  moveHumanoid(player, map);

  // update enemies
  if (globalTime % TIME_INTERVAL_A == 0) {
    for (int i = 0; i < enemies->size; i++) {
      Humanoid *enemy = &((Humanoid *)enemies->data)[i];
      if (!enemy->visible) {
        continue;
      }

      if (arePointsInProximity(&player->position, &enemy->position, 50)) {
        shoot(enemy, bullets);
      }
      else {
      executeRoutine(enemy);

      }


      if (globalTime % TIME_INTERVAL_C == 0) {
        incrementSprite(enemy);
      }
    }
  }

  // update bullets
  for (int i = 0; i < bullets->size; i++) {
    Bullet *bullet = &((Bullet *)bullets->data)[i];

    moveBullet(bullet);

    if (bulletOutOfScreen(bullet)) {
      removeFromVector(bullets, i);
      i--;
      continue;
    }

    for (int j = 0; j < enemies->size; j++) {
      Humanoid *enemy = &((Humanoid *)enemies->data)[j];

      if (collidesWithBullet(enemy, bullet)) {
        die(enemy);
        removeFromVector(bullets, i);
        i--;
        break;
      }

      if (globalTime % TIME_INTERVAL_D == 0) {

        if (!enemy->alive && enemy->visible) {
          removeFromVector(enemies, j);
          break;
        }
      }
    }
  }

  globalTime++;
}

void initializeEnemies(Map *map, Vector *enemies, SDL_Texture *texture) {
  for (unsigned int i = 0; i < map->numberOfLevels; i++) {
    for (unsigned int j = 0; j < map->levels[i].numberOfPlatforms; j++) {
      Platform *platform = &map->levels[i].platforms[j];
      Humanoid enemy;
      humanoidConstructor(&enemy, texture,
                          createPoint(platform->startX, platform->y),
                          createPoint(platform->startX, 0),
                          createPoint(platform->endX, 0), true, 4, true, true);
      append(enemies, &enemy);
    }
  }
}

void runGame() {


  Map map;
  parseMapConfig(MAP_CONFIG_PATH, &map);

  SDL_Texture *texture_a;
  SDL_Texture *texture_b;

  SDL_Init(SDL_INIT_EVERYTHING);

  // init font
  TTF_Init();
  font = TTF_OpenFont(FONT_PATH, 12);

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
  humanoidConstructor(&player, texture_a, createPoint(0, 0), createPoint(0, 0),
                      createPoint(300, 0), false, 4, true, true);

  Vector enemies;
  vectorConstructor(&enemies, 10, HUMANOID);
  loadTexture(ENEMY_A_PATH, renderer, &texture_b);
  initializeEnemies(&map, &enemies, texture_b);

  Vector bullets;
  vectorConstructor(&bullets, MAX_BULLETS, BULLET);

  // load textures
  SDL_Surface *background;
  loadSurface(BACKGROUND_PATH, &background);

  backgroundTexture = SDL_CreateTextureFromSurface(renderer, background);
  SDL_FreeSurface(background);

  SDL_Surface *bullet;
  loadSurface(BULLET_PATH, &bullet);

  bulletTexture = SDL_CreateTextureFromSurface(renderer, bullet);
  SDL_FreeSurface(bullet);

  bool gameFlag = true;

  while (gameFlag) {
    gameFlag = processEvents(window, &player, &bullets);
    updateLogic(&map, &player, &enemies, &bullets);
    renderAll(renderer, &player, &enemies, &bullets);
    SDL_Delay(10); // don't burn up the CPU
  }

  // cleanup
  SDL_DestroyWindow(window);
  SDL_DestroyRenderer(renderer);
  SDL_DestroyTexture(backgroundTexture);
  SDL_DestroyTexture(bulletTexture);
  SDL_DestroyTexture(texture_a);
  SDL_DestroyTexture(texture_b);
  TTF_CloseFont(font);

  freeMap(&map);
  humanoidDestructor(&player);
  clear(&enemies);
  clear(&bullets);
  SDL_Quit();
}
