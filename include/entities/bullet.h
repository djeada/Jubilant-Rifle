#ifndef BULLET_H
#define BULLET_H

#include "entity.h"

typedef enum { BULLET_SOURCE_PLAYER, BULLET_SOURCE_ENEMY } BulletSource;

typedef struct Bullet {
  Entity base; // Inheritance: Bullet “is-a” Entity.
  BulletSource source;
} Bullet;

Bullet *bulletCreate(BulletSource source, float x, float y, float vx, float vy);
void bulletUpdate(Bullet *bullet, float dt);
bool isBulletAlive(const Bullet *bullet);

#endif // BULLET_H

