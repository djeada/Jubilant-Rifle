#include "entities/bullet.h"

void bullet_update(Entity *self, float dt) {
    common_update(self, dt);
    if(self->pos.x < 0 || self->pos.x > 640 || self->pos.y < 0 || self->pos.y > 480)
        self->alive = 0;
}