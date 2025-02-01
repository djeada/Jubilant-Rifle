#ifndef PLAYER_H
#define PLAYER_H
#include "entities/entity.h"

Entity * playerCreate(float x, float y) ;
void playerUpdate(Entity *self, float dt);

#endif
