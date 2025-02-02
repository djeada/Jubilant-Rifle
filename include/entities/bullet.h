#ifndef BULLET_H
#define BULLET_H

#include "entity.h"

typedef enum {
    BULLET_SOURCE_PLAYER,
    BULLET_SOURCE_ENEMY
} BulletSource;

typedef struct Bullet {
    Entity base;       // Inheritance: Bullet “is-a” Entity.
    BulletSource source;
} Bullet;

// Creates a new bullet with the given source, position, and velocity.
Bullet* bulletCreate(BulletSource source, float x, float y, float vx, float vy);

// Updates a bullet (calls common entity update and then applies bullet‐specific logic).
void bulletUpdate(Bullet *bullet, float dt);

#endif // BULLET_H
