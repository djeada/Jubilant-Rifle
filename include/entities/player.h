#ifndef PLAYER_H
#define PLAYER_H

#include "entities/bullet_pool.h"
#include "entities/entity.h"

typedef struct Player {
  Entity base;
  int level;
} Player;

/* Creation / Destruction */
Player *playerCreate(float x, float y);
void playerDestroy(Player *player);

/* Update */
void playerUpdate(Entity *entity, float dt);

/* Actions */
void playerSetHorizontalVelocity(Player *player, float vx);
void playerSetVerticalVelocity(Player *player, float vy);
void playerStopHorizontal(Player *player);
void playerStopVertical(Player *player);
void playerShoot(Player *player, BulletPool *bulletPool);

/* Queries */
bool isPlayerAlive(const Player *player);
Direction getPlayerFacingDirection(const Player *player);

#endif // PLAYER_H

