#include "game/input.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

/* Global instances */
static InputConfig g_inputConfig;
static InputState g_inputState;
static bool g_inputInitialized = false;

/* Action names for display and config files */
static const char *g_actionNames[ACTION_COUNT] = {
    "Move Left",  "Move Right", "Move Up", "Move Down", "Jump",
    "Shoot",      "Pause",      "Menu",    "Confirm",   "Cancel"};

void inputConfigInit(InputConfig *config) {
  if (!config)
    return;

  /* Set default key bindings */
  config->bindings[ACTION_MOVE_LEFT].primaryKey = SDLK_LEFT;
  config->bindings[ACTION_MOVE_LEFT].secondaryKey = SDLK_a;

  config->bindings[ACTION_MOVE_RIGHT].primaryKey = SDLK_RIGHT;
  config->bindings[ACTION_MOVE_RIGHT].secondaryKey = SDLK_d;

  config->bindings[ACTION_MOVE_UP].primaryKey = SDLK_UP;
  config->bindings[ACTION_MOVE_UP].secondaryKey = SDLK_w;

  config->bindings[ACTION_MOVE_DOWN].primaryKey = SDLK_DOWN;
  config->bindings[ACTION_MOVE_DOWN].secondaryKey = SDLK_s;

  config->bindings[ACTION_JUMP].primaryKey = SDLK_SPACE;
  config->bindings[ACTION_JUMP].secondaryKey = SDLK_UNKNOWN;

  config->bindings[ACTION_SHOOT].primaryKey = SDLK_x;
  config->bindings[ACTION_SHOOT].secondaryKey = SDLK_LCTRL;

  config->bindings[ACTION_PAUSE].primaryKey = SDLK_p;
  config->bindings[ACTION_PAUSE].secondaryKey = SDLK_ESCAPE;

  config->bindings[ACTION_MENU].primaryKey = SDLK_ESCAPE;
  config->bindings[ACTION_MENU].secondaryKey = SDLK_UNKNOWN;

  config->bindings[ACTION_CONFIRM].primaryKey = SDLK_RETURN;
  config->bindings[ACTION_CONFIRM].secondaryKey = SDLK_SPACE;

  config->bindings[ACTION_CANCEL].primaryKey = SDLK_ESCAPE;
  config->bindings[ACTION_CANCEL].secondaryKey = SDLK_BACKSPACE;
}

int inputConfigLoad(InputConfig *config, const char *filePath) {
  if (!config || !filePath)
    return -1;

  /* Start with defaults */
  inputConfigInit(config);

  FILE *file = fopen(filePath, "r");
  if (!file) {
    /* File doesn't exist, use defaults (not an error) */
    return 0;
  }

  char line[256];
  while (fgets(line, sizeof(line), file)) {
    /* Skip comments and empty lines */
    if (line[0] == '#' || line[0] == '\n' || line[0] == '\r')
      continue;

    char actionName[64];
    int primary, secondary;

    if (sscanf(line, "%63[^=]=%d,%d", actionName, &primary, &secondary) >= 2) {
      /* Find the action by name */
      for (int i = 0; i < ACTION_COUNT; i++) {
        if (strstr(actionName, g_actionNames[i])) {
          config->bindings[i].primaryKey = (SDL_Keycode)primary;
          config->bindings[i].secondaryKey = (SDL_Keycode)secondary;
          break;
        }
      }
    }
  }

  fclose(file);
  return 0;
}

int inputConfigSave(const InputConfig *config, const char *filePath) {
  if (!config || !filePath)
    return -1;

  FILE *file = fopen(filePath, "w");
  if (!file)
    return -1;

  fprintf(file, "# Jubilant Rifle Input Configuration\n");
  fprintf(file, "# Format: ActionName = PrimaryKey, SecondaryKey\n\n");

  for (int i = 0; i < ACTION_COUNT; i++) {
    fprintf(file, "%s = %d, %d\n", g_actionNames[i],
            config->bindings[i].primaryKey,
            config->bindings[i].secondaryKey);
  }

  fclose(file);
  return 0;
}

void inputSetBinding(InputConfig *config, GameAction action,
                     SDL_Keycode primary, SDL_Keycode secondary) {
  if (!config || action >= ACTION_COUNT)
    return;

  config->bindings[action].primaryKey = primary;
  config->bindings[action].secondaryKey = secondary;
}

const char *inputGetActionName(GameAction action) {
  if (action >= ACTION_COUNT)
    return "Unknown";
  return g_actionNames[action];
}

const char *inputGetKeyName(SDL_Keycode key) {
  return SDL_GetKeyName(key);
}

void inputStateInit(InputState *state) {
  if (!state)
    return;

  memset(state->actionPressed, 0, sizeof(state->actionPressed));
  memset(state->actionHeld, 0, sizeof(state->actionHeld));
  memset(state->actionReleased, 0, sizeof(state->actionReleased));
}

void inputStateClearFrame(InputState *state) {
  if (!state)
    return;

  /* Clear pressed and released states (they are only valid for one frame) */
  memset(state->actionPressed, 0, sizeof(state->actionPressed));
  memset(state->actionReleased, 0, sizeof(state->actionReleased));
}

/**
 * Helper to find which action a key corresponds to.
 * Returns ACTION_COUNT if no action is found.
 */
static GameAction findActionForKey(const InputConfig *config, SDL_Keycode key) {
  for (int i = 0; i < ACTION_COUNT; i++) {
    if (config->bindings[i].primaryKey == key ||
        config->bindings[i].secondaryKey == key) {
      return (GameAction)i;
    }
  }
  return ACTION_COUNT;
}

void inputProcessKeyDown(InputState *state, const InputConfig *config,
                         SDL_Keycode key) {
  if (!state || !config)
    return;

  /* Find all actions bound to this key */
  for (int i = 0; i < ACTION_COUNT; i++) {
    if (config->bindings[i].primaryKey == key ||
        config->bindings[i].secondaryKey == key) {
      if (!state->actionHeld[i]) {
        state->actionPressed[i] = true;
      }
      state->actionHeld[i] = true;
    }
  }
}

void inputProcessKeyUp(InputState *state, const InputConfig *config,
                       SDL_Keycode key) {
  if (!state || !config)
    return;

  /* Find all actions bound to this key */
  for (int i = 0; i < ACTION_COUNT; i++) {
    if (config->bindings[i].primaryKey == key ||
        config->bindings[i].secondaryKey == key) {
      state->actionHeld[i] = false;
      state->actionReleased[i] = true;
    }
  }
}

bool inputIsActionPressed(const InputState *state, GameAction action) {
  if (!state || action >= ACTION_COUNT)
    return false;
  return state->actionPressed[action];
}

bool inputIsActionHeld(const InputState *state, GameAction action) {
  if (!state || action >= ACTION_COUNT)
    return false;
  return state->actionHeld[action];
}

bool inputIsActionReleased(const InputState *state, GameAction action) {
  if (!state || action >= ACTION_COUNT)
    return false;
  return state->actionReleased[action];
}

InputConfig *inputGetGlobalConfig(void) {
  if (!g_inputInitialized) {
    inputConfigInit(&g_inputConfig);
    inputStateInit(&g_inputState);
    g_inputInitialized = true;
  }
  return &g_inputConfig;
}

InputState *inputGetGlobalState(void) {
  if (!g_inputInitialized) {
    inputConfigInit(&g_inputConfig);
    inputStateInit(&g_inputState);
    g_inputInitialized = true;
  }
  return &g_inputState;
}
