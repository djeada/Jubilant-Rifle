#include "game/game_logic.h"
#include "entities/bullet.h"
#include "entities/enemy.h"
#include "utils/consts.h"
#include "utils/resources.h"
#include "utils/time_manager.h"

// Update the animation state of the humanoid player
void updateAnimation(Humanoid *player) {
    if (movementStateIsMoving(&player->movement) && timeManagerGetTime() % 3 == 0) {
        animationStateIncrementSprite(&player->animation);
    }
}

// Update the horizontal position of the humanoid player
void updatePosition(Humanoid *player) {
    movementStateMoveHorizontal(&player->movement);
}

// Collision detection between a bullet and an enemy
bool checkCollisionWithBullet(const Enemy *enemy, const Bullet *bullet) {
    // Assume bullet and enemy have bounding boxes defined as rectangles
    int leftB = bullet->movement.position.x;
    int rightB = leftB + BULLET_WIDTH;
    int topB = bullet->movement.position.y;
    int bottomB = topB + BULLET_HEIGHT;
    int leftE = enemy->base.movement.position.x;
    int rightE = leftE + HUMANOID_FRAME_WIDTH;
    int topE = enemy->base.movement.position.y;
    int bottomE = topE + HUMANOID_FRAME_HEIGHT;

    // Basic AABB collision detection
    return !(bottomB <= topE || topB >= bottomE || rightB <= leftE || leftB >= rightE);
}


// Handle the shooting mechanics for the humanoid player
void handleShooting(Humanoid *player, Vector *enemies) {
    for (size_t i = 0; i < player->bulletManager.bullets.size; ++i) {
        Bullet *bullet = (Bullet *)player->bulletManager.bullets.items[i];

        // We only want moving bullets; movementStateIsMoving should be used here
        if (movementStateIsMoving(&bullet->movement)) {
            movementStateMoveHorizontal(&bullet->movement);

            // Check if collision with any of the enemies, if so decrease health
            for (size_t j = 0; j < enemies->size; ++j) {
                Enemy *enemy = (Enemy *)enemies->items[j];
                if (checkCollisionWithBullet(enemy, bullet)) {
                    humanoidDecreaseLife(&enemy->base, 1000);
                    if (!enemy->base.isAlive) {
                        // Handle enemy death if necessary
                        humanoidDie(&enemy->base);
                    }
                    // Stop and hide the bullet after a collision
                    movementStateStop(&bullet->movement);
                    animationStateHide(&bullet->animation);
                    break;
                }
            }

            // Check if bullet has exceeded its range
            if (pointDistance(&player->movement.position, &bullet->movement.position) > 500) {
                movementStateStop(&bullet->movement);
                animationStateHide(&bullet->animation);
            }
        }
    }
}


// Handle collisions with platforms and adjust the player's position accordingly
bool handleCollisions(Humanoid *player, const Map *map) {
    for (size_t i = 0; i < map->platformCount; ++i) {
        if (checkCollision(player, &map->platforms[i])) {
            player->movement.position.y = map->platforms[i].y - 0.95 * HUMANOID_FRAME_HEIGHT;
            return true;
        }
    }
    return false;
}

// Apply gravity to the humanoid player if they are not on a platform
void applyGravity(Humanoid *player, bool onPlatform) {
    if (onPlatform) {
        movementStateMoveJump(&player->movement);
    } else if (!movementStateIsJumping(&player->movement)) {
        movementStateFall(&player->movement);
    }
}

// Check and constrain the player's position within the world bounds
void checkWorldBounds(Humanoid *player) {
    if (player->movement.position.x < 0) {
        player->movement.position.x = 0;
    }
    if (player->movement.position.x > WORLD_WIDTH) {
        player->movement.position.x = WORLD_WIDTH;
    }
    if (player->movement.position.y < 0) {
        player->movement.position.y = 0;
    }
    if (player->movement.position.y > WORLD_HEIGHT) {
        humanoidDie(player);
    }
}

// Update the state of the humanoid player
void updateHumanoid(Humanoid *player, Vector *enemies, Map *map) {
    updateAnimation(player);
    updatePosition(player);
    handleShooting(player, enemies);
    checkWorldBounds(player);

    bool onPlatform = handleCollisions(player, map);
    applyGravity(player, onPlatform);

    movementStateStop(&player->movement);
}

// Basic AABB collision detection between a humanoid and a platform
bool checkCollision(const Humanoid *humanoid, const Platform *platform) {
    int leftA = humanoid->movement.position.x;
    int rightA = leftA + HUMANOID_FRAME_WIDTH / 2;
    int topA = humanoid->movement.position.y;
    int bottomA = topA + HUMANOID_FRAME_HEIGHT;
    int leftB = platform->x;
    int rightB = leftB + platform->width;
    int topB = platform->y;
    int bottomB = topB + platform->height;

    return !(bottomA <= topB || topA >= bottomB || rightA <= leftB || leftA >= rightB);
}

// Update the state of the enemies
void updateEnemies(Vector *enemies) {
    for (size_t i = 0; i < enemies->size; ++i) {
        Enemy *enemy = (Enemy *)enemies->items[i];

        enemy->base.movement.position.x += enemy->base.animation.isFacingLeft ? -1 : 1;

        if ((enemy->base.animation.isFacingLeft && enemy->base.movement.position.x < enemy->patrolStart.x) ||
            (!enemy->base.animation.isFacingLeft && enemy->base.movement.position.x > enemy->patrolEnd.x)) {
            enemy->base.animation.isFacingLeft = !enemy->base.animation.isFacingLeft;
        }
    }
}
