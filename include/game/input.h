#ifndef INPUT_H
#define INPUT_H

/**
 * @file input.h
 * @brief Input handling and key binding system.
 *
 * Provides a flexible input system with customizable key bindings,
 * allowing players to configure their preferred control scheme.
 */

#include <SDL2/SDL.h>
#include <stdbool.h>

/**
 * @brief Game actions that can be bound to inputs.
 */
typedef enum {
  ACTION_MOVE_LEFT,
  ACTION_MOVE_RIGHT,
  ACTION_MOVE_UP,
  ACTION_MOVE_DOWN,
  ACTION_JUMP,
  ACTION_SHOOT,
  ACTION_PAUSE,
  ACTION_MENU,
  ACTION_CONFIRM,
  ACTION_CANCEL,
  ACTION_COUNT /* Total number of actions */
} GameAction;

/**
 * @brief Key binding for a game action.
 */
typedef struct {
  SDL_Keycode primaryKey;     /**< Primary key for this action */
  SDL_Keycode secondaryKey;   /**< Alternative key for this action */
} KeyBinding;

/**
 * @brief Complete input configuration.
 */
typedef struct {
  KeyBinding bindings[ACTION_COUNT];
} InputConfig;

/**
 * @brief Current input state for all actions.
 */
typedef struct {
  bool actionPressed[ACTION_COUNT];  /**< Whether action key was just pressed */
  bool actionHeld[ACTION_COUNT];     /**< Whether action key is being held */
  bool actionReleased[ACTION_COUNT]; /**< Whether action key was just released */
} InputState;

/**
 * @brief Initialize input configuration with default bindings.
 * @param config The configuration to initialize.
 */
void inputConfigInit(InputConfig *config);

/**
 * @brief Load input configuration from a file.
 * @param config The configuration to populate.
 * @param filePath Path to the configuration file.
 * @return 0 on success, -1 on failure.
 */
int inputConfigLoad(InputConfig *config, const char *filePath);

/**
 * @brief Save input configuration to a file.
 * @param config The configuration to save.
 * @param filePath Path to the output file.
 * @return 0 on success, -1 on failure.
 */
int inputConfigSave(const InputConfig *config, const char *filePath);

/**
 * @brief Set a key binding for an action.
 * @param config The input configuration.
 * @param action The action to bind.
 * @param primary Primary key (or SDLK_UNKNOWN to clear).
 * @param secondary Secondary key (or SDLK_UNKNOWN to clear).
 */
void inputSetBinding(InputConfig *config, GameAction action,
                     SDL_Keycode primary, SDL_Keycode secondary);

/**
 * @brief Get the action name as a string.
 * @param action The action.
 * @return Human-readable action name.
 */
const char *inputGetActionName(GameAction action);

/**
 * @brief Get a key name as a string.
 * @param key The SDL keycode.
 * @return Human-readable key name.
 */
const char *inputGetKeyName(SDL_Keycode key);

/**
 * @brief Initialize input state.
 * @param state The state to initialize.
 */
void inputStateInit(InputState *state);

/**
 * @brief Clear per-frame input state (call at start of each frame).
 * @param state The state to clear.
 */
void inputStateClearFrame(InputState *state);

/**
 * @brief Process a key down event.
 * @param state The input state to update.
 * @param config The input configuration.
 * @param key The key that was pressed.
 */
void inputProcessKeyDown(InputState *state, const InputConfig *config,
                         SDL_Keycode key);

/**
 * @brief Process a key up event.
 * @param state The input state to update.
 * @param config The input configuration.
 * @param key The key that was released.
 */
void inputProcessKeyUp(InputState *state, const InputConfig *config,
                       SDL_Keycode key);

/**
 * @brief Check if an action was just pressed this frame.
 * @param state The input state.
 * @param action The action to check.
 * @return true if the action was just pressed.
 */
bool inputIsActionPressed(const InputState *state, GameAction action);

/**
 * @brief Check if an action is currently held.
 * @param state The input state.
 * @param action The action to check.
 * @return true if the action is being held.
 */
bool inputIsActionHeld(const InputState *state, GameAction action);

/**
 * @brief Check if an action was just released this frame.
 * @param state The input state.
 * @param action The action to check.
 * @return true if the action was just released.
 */
bool inputIsActionReleased(const InputState *state, GameAction action);

/**
 * @brief Get the global input configuration.
 * @return Pointer to the global input configuration.
 */
InputConfig *inputGetGlobalConfig(void);

/**
 * @brief Get the global input state.
 * @return Pointer to the global input state.
 */
InputState *inputGetGlobalState(void);

#endif /* INPUT_H */
