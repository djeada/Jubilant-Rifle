#include "entities/player.h"
#include "utils/consts.h"
#include <SDL2/SDL.h>
#include <stdlib.h>

#define MELEE_COOLDOWN 0.5f
#define MELEE_RANGE 50
#define MELEE_DAMAGE 30
#define PLAYER_INITIAL_GRENADES 3

void playerUpdate(Entity *entity, float dt) {
  Player *player = (Player *)entity;
  
  /* Update melee cooldown */
  if (player->meleeTimer > 0) {
    player->meleeTimer -= dt;
    if (player->meleeTimer <= 0) {
      player->isMeleeAttacking = false;
    }
  }
  
}

Player *playerCreate(float x, float y) {
  Player *player = malloc(sizeof(Player));
  if (!player)
    return NULL;

  /* Create temporary entity to copy values from */
  Entity *tempEntity = entityCreate(ENTITY_PLAYER, x, y);
  if (!tempEntity) {
    free(player);
    return NULL;
  }
  player->base = *tempEntity;
  free(tempEntity); /* Free the temporary entity, values are copied */

  player->base.update = playerUpdate;
  player->base.health = 100;
  player->base.anim = malloc(sizeof(Animation));
  if (!player->base.anim) {
    free(player);
    return NULL;
  }
  player->base.anim->tex = NULL;
  player->base.anim->frameCount = 6;
  player->base.anim->frameDuration = 0.1f;
  player->base.anim->timer = 0;
  player->base.anim->currentFrame = 0;
  player->level = 1;
  player->base.direction = DIRECTION_LEFT;
  
  /* Initialize new player fields */
  player->isClimbing = false;
  player->climbIntentUp = false;
  player->climbIntentDown = false;
  player->jumpRequested = false;
  player->isMeleeAttacking = false;
  player->meleeTimer = 0.0f;
  player->meleeDamage = MELEE_DAMAGE;
  player->grenadeCount = PLAYER_INITIAL_GRENADES;
  player->jumpsUsed = 0;

  return player;
}

void playerDestroy(Player *player) {
  if (player) {
    if (player->base.anim) {
      free(player->base.anim);
    }
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

void playerMeleeAttack(Player *player) {
  if (!player || player->meleeTimer > 0)
    return;
  
  player->isMeleeAttacking = true;
  player->meleeTimer = MELEE_COOLDOWN;
}

bool playerMeleeHits(const Player *player, const SDL_Rect *targetRect) {
  if (!player || !targetRect || !player->isMeleeAttacking)
    return false;
  
  /* Calculate melee attack hitbox based on facing direction */
  SDL_Rect meleeRect;
  meleeRect.y = (int)player->base.pos.y;
  meleeRect.w = MELEE_RANGE;
  meleeRect.h = HUMANOID_FRAME_HEIGHT;
  
  if (player->base.direction == DIRECTION_RIGHT) {
    meleeRect.x = (int)player->base.pos.x + SPRITE_WIDTH;
  } else {
    meleeRect.x = (int)player->base.pos.x - MELEE_RANGE;
  }
  
  return SDL_HasIntersection(&meleeRect, targetRect);
}

int playerGetMeleeDamage(const Player *player) {
  if (!player)
    return 0;
  return (int)player->meleeDamage;
}

void playerStartClimbing(Player *player) {
  if (!player)
    return;
  player->isClimbing = true;
  /* Stop horizontal movement while climbing */
  player->base.vel.x = 0;
}

void playerStopClimbing(Player *player) {
  if (!player)
    return;
  player->isClimbing = false;
}

bool playerIsClimbing(const Player *player) {
  if (!player)
    return false;
  return player->isClimbing;
}

void playerAddGrenades(Player *player, int count) {
  if (!player || count <= 0)
    return;
  player->grenadeCount += count;
}

bool playerUseGrenade(Player *player) {
  if (!player || player->grenadeCount <= 0)
    return false;
  player->grenadeCount--;
  return true;
}

int playerGetGrenadeCount(const Player *player) {
  if (!player)
    return 0;
  return player->grenadeCount;
}
