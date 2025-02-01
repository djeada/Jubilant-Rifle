#ifndef PLAYER_H
#define PLAYER_H
#include "entities/entity.h"

Entity * player_create(float x, float y) ;
void player_update(Entity *self, float dt);

#endif
