typedef struct {
  float x, y, dx;
} Bullet;


void addBullet(float x, float y, float dx, Bullet **bullets);
void removeBullet(int i, Bullet **bullets);
void cleanupBullets(Bullet **bullets);