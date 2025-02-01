#include "entities/player.h"

Entity *playerCreate(float x, float y) {
  Entity *player = entityCreate(ENTITY_PLAYER, x, y);
  player->health = 100;
  player->update = playerUpdate;
  player->anim = malloc(sizeof(Animation));
  player->anim->frameCount = 6;       // e.g. 6 frames in the sprite sheet
  player->anim->frameDuration = 0.1f; // each frame lasts 0.1 seconds
  player->anim->timer = 0;
  player->anim->currentFrame = 0;
  return player;
}

void playerUpdate(Entity *self, float dt) { commonEntityUpdate(self, dt); }
