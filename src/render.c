#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <SDL2/SDL_ttf.h>
#include <stdbool.h>

#include "render.h"

extern SDL_Texture *backgroundTexture;
extern SDL_Texture *bulletTexture;
extern TTF_Font *font;

void render_entity(SDL_Renderer *renderer, SDL_Texture *texture,
                   unsigned int width, unsigned int height, unsigned int sprite,
                   Point *position, bool facingLeft) {
  SDL_Rect srcRect = {width * sprite, 0, width, height};
  SDL_Rect rect = {position->x, position->y, width, height};
  SDL_RenderCopyEx(renderer, texture, &srcRect, &rect, 0, NULL,
                   (SDL_RendererFlip)facingLeft);
}

void renderBullet(SDL_Renderer *renderer, Vector *bullets) {
  for (int i = 0; i < bullets->size; i++) {
    Bullet *bullet = &((Bullet *)bullets->data)[i];
    render_entity(renderer, bulletTexture, BULLET_WIDTH, BULLET_HEIGHT, 0,
                  &bullet->position, true);
  }
}

void renderHealthBar(SDL_Renderer *renderer, unsigned int length) {
  SDL_Rect healthBarBackground = {10, 0, 102, 12};
  SDL_SetRenderDrawColor(renderer, 31, 32, 34, 255);
  SDL_RenderFillRect(renderer, &healthBarBackground);
  SDL_Rect healthBar = {11, 1, length, 10};
  SDL_SetRenderDrawColor(renderer, 5, 195, 221, 100);
  SDL_RenderFillRect(renderer, &healthBar);
  // draw text
  SDL_Color color = {255, 255, 255, 255};

  SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);

  SDL_Surface *surface = TTF_RenderText_Solid(font, "Level 1 ", color);
  SDL_Texture *texture = SDL_CreateTextureFromSurface(renderer, surface);
  SDL_Rect textRect = {12, 1, 30, 10};
  SDL_RenderCopy(renderer, texture, NULL, &textRect);
  SDL_FreeSurface(surface);
  SDL_DestroyTexture(texture);
}

void renderAll(SDL_Renderer *renderer, Humanoid *player, Vector *enemies) {

  SDL_SetRenderDrawColor(renderer, 0, 0, 0,
                         0); // set the drawing color to black
  SDL_RenderClear(renderer);
  SDL_RenderCopy(renderer, backgroundTexture, NULL, NULL);

  // render player
  render_entity(renderer, player->texture, HUMANOID_WIDTH, HUMANOID_HEIGHT,
                player->currentSprite, &player->position, player->facingLeft);
  Vector *bullets = (Vector *)player->bullets;
  renderBullet(renderer, bullets);

  // render enemies
  for (int i = 0; i < enemies->size; i++) {
    Humanoid *enemy = &((Humanoid *)enemies->data)[i];
    if (!enemy->visible) {
      continue;
    }

    render_entity(renderer, enemy->texture, HUMANOID_WIDTH, HUMANOID_HEIGHT,
                  enemy->currentSprite, &enemy->position, enemy->facingLeft);

    Vector *bullets = (Vector *)enemy->bullets;
    renderBullet(renderer, bullets);
  }

  // render health bar
  renderHealthBar(renderer, player->life);

  SDL_RenderPresent(renderer);
}
