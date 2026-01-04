#include "game/game_state.h"

const char *gameStateToString(GameState state) {
  switch (state) {
  case STATE_MENU:
    return "Menu";
  case STATE_GAME:
    return "Game";
  case STATE_PAUSED:
    return "Paused";
  case STATE_GAME_OVER:
    return "Game Over";
  case STATE_CONTINUE:
    return "Continue";
  case STATE_SCOREBOARD:
    return "Scoreboard";
  case STATE_SETTINGS:
    return "Settings";
  case STATE_EXIT:
    return "Exit";
  default:
    return "Unknown";
  }
}

int gameStateIsMenu(GameState state) {
  return state == STATE_MENU || state == STATE_SCOREBOARD ||
         state == STATE_SETTINGS;
}

int gameStateIsPlaying(GameState state) { return state == STATE_GAME; }
