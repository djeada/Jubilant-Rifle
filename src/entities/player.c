#include "entities/player.h"
#include <stdlib.h>

void playerUpdate(Entity *entity, float dt) {
  Player *player = (Player *)entity;
  commonEntityUpdate(&player->base, dt);
}

Player *playerCreate(float x, float y) {
  Player *player = malloc(sizeof(Player));
  if (!player)
    return NULL;

  player->base = *entityCreate(ENTITY_PLAYER, x, y);
  player->base.update = playerUpdate;
  player->base.health = 100;
  player->base.anim = malloc(sizeof(Animation));
  player->base.anim->frameCount = 6;
  player->base.anim->frameDuration = 0.1f;
  player->base.anim->timer = 0;
  player->base.anim->currentFrame = 0;
  player->level = 1;
  player->base.direction = DIRECTION_LEFT;

  return player;
}

void playerDestroy(Player *player) {
  if (player) {
    entityDestroy(&player->base);
    free(player);
  }
}

void playerSetHorizontalVelocity(Player *player, float vx) {
  if (vx > 0)
    player->base.direction = DIRECTION_RIGHT;
  else if (vx < 0)
    player->base.direction = DIRECTION_LEFT;
  player->base.vel.x = vx;
}

void playerSetVerticalVelocity(Player *player, float vy) {
  player->base.vel.y = vy;
}

void playerStopHorizontal(Player *player) {
  playerSetHorizontalVelocity(player, 0);
}

void playerStopVertical(Player *player) {
  playerSetVerticalVelocity(player, 0);
}

void playerShoot(Player *player, BulletPool *bulletPool) {
  bulletPoolSpawn(bulletPool, BULLET_SOURCE_PLAYER, player->base.pos.x + 20,
                  player->base.pos.y, 300, 0);
}

bool isPlayerAlive(const Player *player) {
  return isEntityAlive(&player->base);
}

Direction getPlayerFacingDirection(const Player *player) {
  return player->base.direction;
}

