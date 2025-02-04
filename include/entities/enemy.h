#ifndef ENEMY_H
#define ENEMY_H

#include "entities/bullet_pool.h"
#include "entities/entity.h"

typedef struct {
  Entity base;
  float shootTimer;
} Enemy;

/* Creation / Destruction */
Enemy *enemyCreate(float x, float y);

/* Update */
void enemyUpdate(Entity *self, float dt);

/* Actions */
void enemySetHorizontalVelocity(Enemy *enemy, float vx);
void enemySetVerticalVelocity(Enemy *enemy, float vy);
void enemyStopHorizontal(Enemy *enemy);
void enemyStopVertical(Enemy *enemy);
void enemyShoot(Enemy *enemy, BulletPool *bulletPool);

/* Queries */
bool isEnemyAlive(const Enemy *enemy);
Direction getEnemyFacingDirection(const Enemy *enemy);
bool isEnemyMoving(const Enemy *enemy);

#endif // ENEMY_H

