#ifndef ENTITY_H
#define ENTITY_H
#include <SDL2/SDL.h>

#include <stdlib.h>
#include "utils/point.h"
#include <stdbool.h>  

typedef struct {
    SDL_Texture *tex;
    int frameCount;
    float frameDuration;
    float timer;
    int currentFrame;
} Animation;

typedef enum {
    ENTITY_PLAYER,
    ENTITY_ENEMY,
    ENTITY_BULLET
} EntityType;

/* --- Base Entity --- */
typedef struct Entity {
    EntityType type;
    Point pos;
    Point vel;
    int health;
    Animation *anim; // Pointer to animation data (if any)
    void (*update)(struct Entity *self, float dt);
} Entity;

Entity* entityCreate(EntityType type, float x, float y);
void entityDestroy(Entity *e);
void commonEntityUpdate(Entity *self, float dt);
bool isEntityAlive(const Entity *e);

#endif // ENTITY_H
