#include "entities/bullet_manager.h"
#include "entities/bullet.h"

void bulletManagerConstructor(BulletManager *manager, int bulletNumber,
                              AnimationState animation, MovementState movement,
                              SDL_Texture *texture) {

  vectorInit(&manager->bullets, bulletNumber, sizeof(Bullet),
             bulletConstructorGeneric, animation, movement, texture);

  manager->nextAvailable = 0;
}

void bulletManagerCopyConstructor(BulletManager *dest,
                                  const BulletManager *src) {
  vectorCopyConstructor(&dest->bullets, &src->bullets,
                        bulletCopyConstructorGeneric);
  dest->nextAvailable = src->nextAvailable;
}

void bulletManagerDestructor(BulletManager *manager) {
  vectorFree(&manager->bullets, NULL);
}

void bulletManagerFireBullet(BulletManager *manager, Point position,
                             bool isFacingLeft) {

  Bullet *bullet =
      (Bullet *)vectorGet(&manager->bullets, manager->nextAvailable);
  bullet->animation.isVisible = true;
  bullet->movement.position.x = position.x + (isFacingLeft ? -5 : 25);
  bullet->movement.position.y = position.y + 20;
  bullet->movement.velocity.x = isFacingLeft ? -10 : 10;

  if (++manager->nextAvailable >= manager->bullets.size) {
    manager->nextAvailable = 0;
  }
}
