#ifndef PLAYER_H
#define PLAYER_H

#include "entities/humanoid.h"
#include "utils/resources.h"

typedef struct {
  Humanoid base;
  int experience;
  short level;
} Player;

void playerConstructor(Player *player, int experience, short level, Point pos, Point vel, SDL_Texture * texture);
void playerFactory(Player *player);
void playerIncreaseExperience(Player *player, int amount);
void playerLevelUp(Player *player);
bool playerIsAlive(Player *player);

#endif
