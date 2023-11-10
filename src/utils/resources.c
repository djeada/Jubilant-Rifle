#include "utils/resources.h"
static Resources *instance = NULL;
Resources *GetResourcesInstance(void) {
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

bool InitResourcesInstance(SDL_Renderer *renderer) {
  Resources *instance = GetResourcesInstance();
  if (instance == NULL) {
    printf("Failed to get the Resources instance.\n");
    return false;
  }

  // Load textures using a hypothetical 'loadTexture' function.
  loadTexture(renderer, "resources/textures/background.png",
              &instance->backgroundTexture);
  loadTexture(renderer, "resources/textures/bullet.png",
              &instance->bulletTexture);
  loadTexture(renderer, "resources/textures/player.png",
              &instance->playerTexture);
  loadTexture(renderer, "resources/textures/enemy.png",
              &instance->enemyTexture);
  loadTexture(renderer, "resources/textures/platform.png",
              &instance->platformTexture);

  // Initialize font
  instance->font = TTF_OpenFont("resources/fonts/FreeSans.ttf", 12);
  if (!instance->font) {
    printf("Failed to load font: %s\n", TTF_GetError());
    // If the font fails to load, release all loaded textures before returning
    // false.
    FreeResourcesInstance();
    return false;
  }

  // Check if all textures are loaded successfully
  /*
  if (!instance->backgroundTexture || !instance->bulletTexture ||
      !instance->playerTexture || !instance->enemyTexture ||
      !instance->platformTexture) {
    printf("Failed to load one or more textures.\n");
    FreeResourcesInstance();
    return false;
  }*/

  return true;
}

void FreeResourcesInstance(void) {
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
