#include "game/game.h"
#include <stdlib.h>
#include <time.h>

int main(int argc, char *argv[]) {
  srand(time(NULL));
  (void)argv;
  (void)argc;

  runGame();

  return 0;
}
