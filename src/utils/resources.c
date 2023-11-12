#include "utils/resources.h"
#include "utils/consts.h"

static Resources *instance = NULL;

Resources *getResourcesInstance(void) {
  if (instance == NULL) {
    instance = (Resources *)malloc(sizeof(Resources));
    if (instance != NULL) {
      // Initialize the resources with NULL
      instance->backgroundTexture = NULL;
      instance->bulletTexture = NULL;
      instance->playerTexture = NULL;
      instance->enemyTexture = NULL;
      instance->platformTexture = NULL;
      instance->font = NULL;
    }
  }
  return instance;
}

bool initializeResourcesInstance(SDL_Renderer *renderer) {
  Resources *instance = getResourcesInstance();
  if (instance == NULL) {
    printf("Failed to get the Resources instance.\n");
    return false;
  }

  // Load textures using a hypothetical 'loadTexture' function.
  loadTexture(renderer, BACKGROUND_TEXTURE_PATH, &instance->backgroundTexture);
  loadTexture(renderer, BULLET_TEXTURE_PATH, &instance->bulletTexture);
  loadTexture(renderer, PLAYER_TEXTURE_PATH, &instance->playerTexture);
  loadTexture(renderer, ENEMY_TEXTURE_PATH, &instance->enemyTexture);
  loadTexture(renderer, PLATFORM_TEXTURE_PATH, &instance->platformTexture);

  // Initialize font
  instance->font = TTF_OpenFont("resources/fonts/FreeSans.ttf", 12);
  if (!instance->font) {
    printf("Failed to load font: %s\n", TTF_GetError());
    // If the font fails to load, release all loaded textures before returning
    // false.
    freeResourcesInstance();
    return false;
  }

  // Check if all textures are loaded successfully

  if (!instance->backgroundTexture || !instance->bulletTexture ||
      !instance->playerTexture || !instance->enemyTexture ||
      !instance->platformTexture) {
    printf("Failed to load one or more textures.\n");
    freeResourcesInstance();
    return false;
  }

  return true;
}

void freeResourcesInstance(void) {
  if (instance != NULL) {
    // Free individual resources here, e.g.:
    SDL_DestroyTexture(instance->backgroundTexture);
    SDL_DestroyTexture(instance->bulletTexture);
    SDL_DestroyTexture(instance->playerTexture);
    SDL_DestroyTexture(instance->enemyTexture);
    SDL_DestroyTexture(instance->platformTexture);

    // Close the font
    if (instance->font) {
      TTF_CloseFont(instance->font);
    }

    // Finally, free the instance itself and set the pointer to NULL
    free(instance);
    instance = NULL;
  }
}
