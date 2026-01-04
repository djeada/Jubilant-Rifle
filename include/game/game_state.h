#ifndef GAME_STATE_H
#define GAME_STATE_H

/**
 * @file game_state.h
 * @brief Game state enumeration and management.
 *
 * Defines all possible game states and provides utilities for
 * managing state transitions.
 */

/**
 * @brief All possible game states.
 */
typedef enum GameState {
  STATE_MENU,       /**< Main menu */
  STATE_GAME,       /**< Active gameplay */
  STATE_PAUSED,     /**< Game is paused */
  STATE_GAME_OVER,  /**< Game over screen */
  STATE_CONTINUE,   /**< Continue from save */
  STATE_SCOREBOARD, /**< High score display */
  STATE_SETTINGS,   /**< Settings menu */
  STATE_EXIT        /**< Exit the game */
} GameState;

/**
 * @brief Get a human-readable name for a game state.
 * @param state The game state.
 * @return String name of the state.
 */
const char *gameStateToString(GameState state);

/**
 * @brief Check if a state is a menu state (not gameplay).
 * @param state The game state.
 * @return true if state is a menu state.
 */
int gameStateIsMenu(GameState state);

/**
 * @brief Check if a state allows gameplay updates.
 * @param state The game state.
 * @return true if gameplay should update.
 */
int gameStateIsPlaying(GameState state);

#endif /* GAME_STATE_H */

