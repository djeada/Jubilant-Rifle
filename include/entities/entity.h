#ifndef ENTITY_H
#define ENTITY_H
#include <SDL2/SDL.h>

#include <stdlib.h>
#include "utils/point.h"


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
    int alive;
    int health;
    Animation *anim; // Pointer to animation data (if any)
    void (*update)(struct Entity *self, float dt);
} Entity;

Entity* entity_create(EntityType type, float x, float y);
void entity_destroy(Entity *e);
void common_update(Entity *self, float dt);

#endif // ENTITY_H
