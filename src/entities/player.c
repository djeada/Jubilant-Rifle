#include "entities/player.h"
#include "entities/humanoid_factories.h"
#include "utils/utils.h"

void playerConstructor(Player *player, int experience, short level, Point pos,
                       Point vel, SDL_Texture *texture) {
  player->experience = experience;
  player->level = level;
  setupHumanoid(&player->base, 0, false, pos, vel, true, texture);
}

void playerFactory(Player *player) {
  Point defaultPos = {100, 100};
  Point defaultVel = {0, 0};
  SDL_Texture *playerTexture = getResourcesInstance()->playerTexture;
  playerConstructor(player, 0, 1, defaultPos, defaultVel, playerTexture);
}

void playerIncreaseExperience(Player *player, int amount) {
  player->experience += amount;
  printf("Experience increased by %d. Current experience: %d\n", amount,
         player->experience);

  // Check if the player has enough experience to level up
  int experience_needed =
      100 * player->level; // Example formula for experience needed to level up
  if (player->experience >= experience_needed) {
    playerLevelUp(player);
  }
}

void playerLevelUp(Player *player) {
  player->level++;
  player->experience = 0; // Reset experience or carry over remaining experience

  player->base.life.current = player->base.life.full; // Restore life to full
  char buffer[100];
  snprintf(buffer, sizeof(buffer),
           "Leveled up! New level: %d. Life restored to full.\n",
           player->level);
  logError(buffer);
}

bool playerIsAlive(Player *player) { return humanoidIsAlive(&player->base); }