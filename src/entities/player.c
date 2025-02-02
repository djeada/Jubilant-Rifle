#include "entities/player.h"

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
  return player;
}

void playerDestroy(Player *player) {
  if (player) {
    entityDestroy(&player->base);
    free(player);
  }
}

bool isPlayerAlive(const Player *player) {
  return isEntityAlive(&player->base);
}

