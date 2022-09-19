#include "map.h"

#define DEATH 500

int coordinatesToGroundLevel(int x, int y) {
  if (y < 160) {
    if (x > 80 && x < 300) {
      return 150;
    }
    if (x > 360 && x < 510 && y < 120) {
      return 110;
    }
  }

  if (y > 150 && y < 300) {
    if (x >= 140 && x < 355) {
      return 260;
    }
  }

  if (y > 300 && y < 330) {
    if (x < 180) {
      return 310;
    }

    if (x >= 380) {
      return 300;
    }
  }

  return DEATH;
}
