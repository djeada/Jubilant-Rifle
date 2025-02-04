#ifndef ENTITY_H
#define ENTITY_H

#include <SDL2/SDL.h>
#include <stdlib.h>
#include <stdbool.h>
#include "utils/point.h"

/* --- Animation --- */
typedef struct {
    SDL_Texture *tex;
    int frameCount;
    float frameDuration;
    float timer;
    int currentFrame;
} Animation;

/* --- Entity Types --- */
typedef enum {
    ENTITY_PLAYER,
    ENTITY_ENEMY,
    ENTITY_BULLET
} EntityType;

/* --- Direction --- */
typedef enum {
    DIRECTION_LEFT,
    DIRECTION_RIGHT
} Direction;

/* --- Base Entity --- */
typedef struct Entity {
    EntityType type;
    Point pos;
    Point vel;
    int health;
    Animation *anim; // Pointer to animation data (if any)
    void (*update)(struct Entity *self, float dt);
} Entity;

/* --- Entity API --- */
Entity* entityCreate(EntityType type, float x, float y);
void entityDestroy(Entity *e);
void commonEntityUpdate(Entity *self, float dt);
bool isEntityAlive(const Entity *e);
Direction getEntityFacingDirection(const Entity *e);
bool isEntityMoving(const Entity *e);

#endif // ENTITY_H
