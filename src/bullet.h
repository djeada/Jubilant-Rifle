typedef struct {
  float x, y, dx;
} Bullet;

void bulletConstructor(Bullet *bullet, float x, float y, float dx);
void bulletCopyConstructor(Bullet *destination, Bullet *source);
