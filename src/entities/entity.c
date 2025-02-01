#include "entities/entity.h"

Entity* entity_create(EntityType type, float x, float y) {
    Entity *e = (Entity *)malloc(sizeof(Entity));
    if (!e) return NULL;
    e->type = type;
    e->pos.x = x;
    e->pos.y = y;
    e->vel.x = 0;
    e->vel.y = 0;
    e->alive = 1;
    e->health = 100;
    e->anim = NULL;
    e->update = NULL;
    return e;
}

void entity_destroy(Entity *e) {
    free(e);
}

void common_update(Entity *self, float dt) {
    self->pos.x += self->vel.x * dt;
    self->pos.y += self->vel.y * dt;
}
