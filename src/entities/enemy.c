#include "entities/enemy.h"

bool enemyIsAlive(Enemy *enemy) { return humanoidIsAlive(&enemy->base); }