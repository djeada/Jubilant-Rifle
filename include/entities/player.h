#ifndef PLAYER_H
#define PLAYER_H
#include "entities/entity.h"

typedef struct Player {
    Entity base; 
    int level;    
} Player;

Player* playerCreate(float x, float y);
void playerDestroy(Player* player);
void playerUpdate(Entity* entity, float dt);
bool isPlayerAlive(const Player *player);

#endif
