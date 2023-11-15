#ifndef PLAYER_H
#define PLAYER_H

#include "entities/humanoid.h"

typedef struct {
  Humanoid base;
  int experience;
  // TODO SkillSet skills;
} Player;

#endif
