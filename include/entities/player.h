#ifndef PLAYER_H
#define PLAYER_H
#include "entities/entity.h"
#include "entities/bullet_pool.h"


typedef struct Player {
    Entity base; 
    int level;    
} Player;

// Creation and destruction
Player* playerCreate(float x, float y);
void playerDestroy(Player* player);

// Public API functions to control the player
void playerSetHorizontalVelocity(Player* player, float vx);
void playerSetVerticalVelocity(Player* player, float vy);
void playerShoot(Player* player, BulletPool* bulletPool);

// Possibly more actions (e.g., for animations, state changes)
void playerStopHorizontal(Player* player);
void playerStopVertical(Player* player);

void playerUpdate(Entity* entity, float dt);
bool isPlayerAlive(const Player *player);

#endif // PLAYER_H
