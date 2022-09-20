#include "utils.h"

typedef struct {
  Point position;
  float dx;
} Bullet;

void bulletConstructor(Bullet *bullet, Point position, float dx);
void bulletCopyConstructor(Bullet *destination, Bullet *source);
