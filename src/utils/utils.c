#include "utils/utils.h"

void loadSurface(const char *file, SDL_Surface **surface) {
  // try loading an image, if failed, print error and exit
  *surface = IMG_Load(file);
  if (!*surface) {
    printf("IMG_Load: %s ", IMG_GetError());
    exit(1);
  }
}

bool loadTexture(SDL_Renderer *renderer, const char *path,
                 SDL_Texture **texture) {
  if (!renderer || !path || !texture) {
    fprintf(stderr, "Invalid parameters passed to loadTexture.\n");
    return false; // Invalid parameters.
  }

  // Load the image at the specified path into an SDL_Surface.
  SDL_Surface *sheet = IMG_Load(path);
  if (!sheet) {
    fprintf(stderr, "Loading image failed: %s\n", IMG_GetError());
    return false; // Loading the image failed.
  }

  // Create a texture from the surface.
  *texture = SDL_CreateTextureFromSurface(renderer, sheet);
  // Check if texture creation was successful.
  if (!*texture) {
    fprintf(stderr, "Create texture from surface failed: %s\n", SDL_GetError());
    SDL_FreeSurface(sheet); // Don't forget to free the surface.
    return false;           // Texture creation failed.
  }

  // The texture was created successfully, the surface can be freed.
  SDL_FreeSurface(sheet);
  return true; // Texture loading was successful.
}

unsigned int numberFromString(const char *line) {
  /* returns all digits from a line, while ingoring all other characters */
  unsigned int number = 0;
  unsigned int i = 0;
  while (line[i] != '\0') {
    if (line[i] >= '0' && line[i] <= '9') {
      number = number * 10 + (line[i] - '0');
      if (i > 0 && line[i - 1] == '-') {
        number *= -1;
      }
    }
    i++;
  }
  return number;
}
char *my_strdup(const char *str) {
  size_t len = strlen(str) + 1; // +1 for the null terminator
  char *new_str = (char *)malloc(len);
  if (new_str == NULL) {
    perror("malloc");
    return NULL;
  }
  memcpy(new_str, str, len);
  return new_str;
}

// Utility function to log SDL errors and handle them accordingly.
void logError(const char *msg) {
  fprintf(stderr, "%s! SDL Error: %s\n", msg, SDL_GetError());
}