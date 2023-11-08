#include "entities/humanoid.h"
#include "utils/consts.h"

// TODO: USE VECTOR CONSTRUCTOR

void humanoidConstructor(Humanoid *humanoid, int spriteIndex, bool facingLeft,
                         bool walking, int posX, int posY, int startX, int endX,
                         SDL_Texture *tex, short initialLife, bool alive,
                         bool visible) {
  if (!humanoid)
    return;

  humanoid->animation.currentSpriteIndex = spriteIndex;
  humanoid->animation.isFacingLeft = facingLeft;
  humanoid->animation.isWalking = walking;

  humanoid->movement.position.x = posX;
  humanoid->movement.position.y = posY;
  humanoid->movement.velocity.x = startX;
  humanoid->movement.velocity.y = endX;

  humanoid->texture = tex;

  humanoid->bullets.items = NULL;
  humanoid->bullets.size = 0;
  humanoid->bullets.capacity = 0;

  humanoid->life = initialLife;
  humanoid->isAlive = alive;
  humanoid->animation.isVisible = visible;
}

void humanoidDefaultConstructor(Humanoid *humanoid) {
  if (!humanoid)
    return;

  humanoid->animation.currentSpriteIndex = 0;
  humanoid->animation.isFacingLeft = false;
  humanoid->animation.isWalking = false;

  // Set the movement state to some default values
  humanoid->movement.position.x = 0;
  humanoid->movement.position.y = 0;
  humanoid->movement.velocity.x = 0;
  humanoid->movement.velocity.y = 0;

  humanoid->texture = NULL;
  humanoid->bullets.items = NULL;
  humanoid->bullets.size = 0;
  humanoid->bullets.capacity = 0;

  humanoid->life = 100;
  humanoid->isAlive = true;
  humanoid->animation.isVisible = true;
}

void humanoidCopyConstructor(Humanoid *destination, const Humanoid *source) {
  if (!destination || !source)
    return;

  destination->animation = source->animation;
  destination->movement = source->movement;
  destination->texture = source->texture;
  // Copying bullets is more complex and requires a deep copy if Vector is a
  // dynamic array For now, let's assume Vector contains a fixed-size array or
  // you have a function to copy it
  destination->bullets = source->bullets;
  destination->life = source->life;
  destination->isAlive = source->isAlive;
  destination->animation.isVisible = source->animation.isVisible;
}

// Function to deallocate any dynamic memory associated with a Humanoid
void humanoidDestructor(void *data) {
  if (!data)
    return;

  Humanoid *humanoid = (Humanoid *)data;
  // Assuming that bullets is a dynamic array, we need to free it
  free(humanoid->bullets.items);
  humanoid->bullets.items = NULL;
  humanoid->bullets.size = 0;
  humanoid->bullets.capacity = 0;

  // ONLY IF TEXTURES ARE NOT SHARED:
  // SDL_DestroyTexture(humanoid->texture);
  humanoid->texture = NULL;
}

void humanoidDie(Humanoid *humanoid) {
  if (!humanoid)
    return;

  humanoid->isAlive = false;
  animationStateHide(&humanoid->animation);
}

// Function to decrease the life of a humanoid by a certain amount of damage
void humanoidDecreaseLife(Humanoid *humanoid, unsigned int damage) {
  if (!humanoid || !humanoid->isAlive)
    return;

  if ((int)humanoid->life > (int)damage) {
    humanoid->life -= damage;
  } else {
    humanoid->life = 0;
    humanoidDie(humanoid);
  }
}
