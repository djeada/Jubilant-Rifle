#include "game/main_menu.h"
#include "game/game_state.h" // Adjust include path if necessary
#include "utils/consts.h" // For WINDOWED_MODE_WIDTH, WINDOWED_MODE_HEIGHT, FONT_PATH, etc.
#include <SDL2/SDL.h>
#include <SDL2/SDL_ttf.h>
#include <math.h>
#include <stdbool.h>
#include <stdio.h>

// --- Aesthetic Constants ---
static const SDL_Color COLOR_BG = {50, 50, 50, 255};
static const SDL_Color COLOR_BUTTON = {20, 120, 20, 255};
static const SDL_Color COLOR_BUTTON_HOVER = {40, 200, 40, 255};
static const SDL_Color COLOR_BUTTON_BORDER = {255, 255, 255, 255};
static const SDL_Color COLOR_TEXT = {255, 255, 255, 255};
static const SDL_Color COLOR_TEXT_SHADOW = {0, 0, 0, 150};

// --- Internal Types ---
typedef struct Button {
  SDL_Rect rect;
  const char *text;
  GameState targetState;
} Button;

// --- Forward Declarations for Helper Functions ---
static SDL_Texture *renderText(SDL_Renderer *renderer, TTF_Font *font,
                               const char *text, SDL_Color color, int *w,
                               int *h);
static void renderTextWithShadow(SDL_Renderer *renderer, TTF_Font *font,
                                 const char *text, int x, int y);
static bool pointInRect(int x, int y, const SDL_Rect *rect);
static void renderButton(SDL_Renderer *renderer, TTF_Font *font,
                         const Button *button, bool isSelected, float pulse);

// --- Helper Function Definitions ---

// Renders the given text into an SDL_Texture.
static SDL_Texture *renderText(SDL_Renderer *renderer, TTF_Font *font,
                               const char *text, SDL_Color color, int *w,
                               int *h) {
  SDL_Surface *surface = TTF_RenderText_Blended(font, text, color);
  if (!surface) {
    fprintf(stderr, "TTF_RenderText_Blended Error: %s\n", TTF_GetError());
    return NULL;
  }
  SDL_Texture *texture = SDL_CreateTextureFromSurface(renderer, surface);
  if (!texture) {
    fprintf(stderr, "SDL_CreateTextureFromSurface Error: %s\n", SDL_GetError());
  }
  *w = surface->w;
  *h = surface->h;
  SDL_FreeSurface(surface);
  return texture;
}

// Renders text with a slight shadow offset.
static void renderTextWithShadow(SDL_Renderer *renderer, TTF_Font *font,
                                 const char *text, int x, int y) {
  int textW, textH;
  // Render shadow.
  SDL_Texture *shadowTex =
      renderText(renderer, font, text, COLOR_TEXT_SHADOW, &textW, &textH);
  if (shadowTex) {
    SDL_Rect shadowRect = {x + 2, y + 2, textW, textH};
    SDL_RenderCopy(renderer, shadowTex, NULL, &shadowRect);
    SDL_DestroyTexture(shadowTex);
  }
  // Render main text.
  SDL_Texture *textTex =
      renderText(renderer, font, text, COLOR_TEXT, &textW, &textH);
  if (textTex) {
    SDL_Rect textRect = {x, y, textW, textH};
    SDL_RenderCopy(renderer, textTex, NULL, &textRect);
    SDL_DestroyTexture(textTex);
  }
}

// Returns true if point (x,y) lies within the given SDL_Rect.
static bool pointInRect(int x, int y, const SDL_Rect *rect) {
  return (x >= rect->x && x <= (rect->x + rect->w) && y >= rect->y &&
          y <= (rect->y + rect->h));
}

// Renders a single button. If it is selected, apply a pulse effect.
static void renderButton(SDL_Renderer *renderer, TTF_Font *font,
                         const Button *button, bool isSelected, float pulse) {
  SDL_Rect btnRect = button->rect;

  // Apply pulse effect (scale slightly if selected).
  float scale = isSelected ? (1.0f + pulse) : 1.0f;
  int scaledW = (int)(btnRect.w * scale);
  int scaledH = (int)(btnRect.h * scale);
  int offsetX = (scaledW - btnRect.w) / 2;
  int offsetY = (scaledH - btnRect.h) / 2;
  SDL_Rect scaledRect = {btnRect.x - offsetX, btnRect.y - offsetY, scaledW,
                         scaledH};

  // Draw drop shadow.
  SDL_Rect shadowRect = {scaledRect.x + 4, scaledRect.y + 4, scaledRect.w,
                         scaledRect.h};
  SDL_SetRenderDrawColor(renderer, 0, 0, 0, 100);
  SDL_RenderFillRect(renderer, &shadowRect);

  // Draw button background (hover color if selected).
  SDL_Color btnColor = isSelected ? COLOR_BUTTON_HOVER : COLOR_BUTTON;
  SDL_SetRenderDrawColor(renderer, btnColor.r, btnColor.g, btnColor.b,
                         btnColor.a);
  SDL_RenderFillRect(renderer, &scaledRect);

  // Draw button border.
  SDL_SetRenderDrawColor(renderer, COLOR_BUTTON_BORDER.r, COLOR_BUTTON_BORDER.g,
                         COLOR_BUTTON_BORDER.b, COLOR_BUTTON_BORDER.a);
  SDL_RenderDrawRect(renderer, &scaledRect);

  // Center and render the button text.
  int textW, textH;
  SDL_Texture *tempTex =
      renderText(renderer, font, button->text, COLOR_TEXT, &textW, &textH);
  if (tempTex) {
    SDL_DestroyTexture(tempTex);
  }
  int textX = scaledRect.x + (scaledRect.w - textW) / 2;
  int textY = scaledRect.y + (scaledRect.h - textH) / 2;
  renderTextWithShadow(renderer, font, button->text, textX, textY);
}

// --- Main Menu Function ---

void runMainMenu(SDL_Renderer *renderer, GameState *state) {
  printf("Entering runMainMenu()\n");
  SDL_Event event;
  bool menuRunning = true;

  // Use window dimensions from your constants.
  const int windowWidth = WINDOWED_MODE_WIDTH;
  const int windowHeight = WINDOWED_MODE_HEIGHT;

  // Button dimensions and positioning.
  const int buttonWidth = 250;
  const int buttonHeight = 60;
  const int spacing = 20;
  const int buttonX = (windowWidth - buttonWidth) / 2;
  int startY = windowHeight / 2 - ((buttonHeight * 5) + (spacing * 4)) / 2;

  // Define the menu buttons.
  Button buttons[] = {{{buttonX, startY + 0 * (buttonHeight + spacing),
                        buttonWidth, buttonHeight},
                       "New Game",
                       STATE_GAME},
                      {{buttonX, startY + 1 * (buttonHeight + spacing),
                        buttonWidth, buttonHeight},
                       "Continue",
                       STATE_CONTINUE},
                      {{buttonX, startY + 2 * (buttonHeight + spacing),
                        buttonWidth, buttonHeight},
                       "Scoreboard",
                       STATE_SCOREBOARD},
                      {{buttonX, startY + 3 * (buttonHeight + spacing),
                        buttonWidth, buttonHeight},
                       "Settings",
                       STATE_SETTINGS},
                      {{buttonX, startY + 4 * (buttonHeight + spacing),
                        buttonWidth, buttonHeight},
                       "Exit",
                       STATE_EXIT}};
  const int buttonCount = sizeof(buttons) / sizeof(buttons[0]);

  // Load the font.
  TTF_Font *font = TTF_OpenFont(FONT_PATH, 28);
  if (!font) {
    fprintf(stderr, "Failed to load font from '%s': %s\n", FONT_PATH,
            TTF_GetError());
    SDL_ShowSimpleMessageBox(SDL_MESSAGEBOX_ERROR, "Font Error",
                             "Failed to load font. Exiting...", NULL);
    *state = STATE_EXIT;
    return;
  }

  Uint32 startTime = SDL_GetTicks();
  int selectedButton = 0; // Tracks the currently selected button.

  while (menuRunning) {
    while (SDL_PollEvent(&event)) {
      if (event.type == SDL_QUIT) {
        *state = STATE_EXIT;
        menuRunning = false;
      } else if (event.type == SDL_MOUSEMOTION) {
        int mx = event.motion.x, my = event.motion.y;
        for (int i = 0; i < buttonCount; i++) {
          if (pointInRect(mx, my, &buttons[i].rect)) {
            selectedButton = i;
            break;
          }
        }
      } else if (event.type == SDL_MOUSEBUTTONDOWN &&
                 event.button.button == SDL_BUTTON_LEFT) {
        int mx = event.button.x, my = event.button.y;
        for (int i = 0; i < buttonCount; i++) {
          if (pointInRect(mx, my, &buttons[i].rect)) {
            *state = buttons[i].targetState;
            menuRunning = false;
            break;
          }
        }
      } else if (event.type == SDL_KEYDOWN) {
        if (event.key.keysym.sym == SDLK_ESCAPE) {
          *state = STATE_EXIT;
          menuRunning = false;
        } else if (event.key.keysym.sym == SDLK_UP) {
          selectedButton = (selectedButton - 1 + buttonCount) % buttonCount;
        } else if (event.key.keysym.sym == SDLK_DOWN) {
          selectedButton = (selectedButton + 1) % buttonCount;
        } else if (event.key.keysym.sym == SDLK_RETURN) {
          *state = buttons[selectedButton].targetState;
          menuRunning = false;
        }
      }
    }

    // Clear the screen.
    SDL_SetRenderDrawColor(renderer, COLOR_BG.r, COLOR_BG.g, COLOR_BG.b,
                           COLOR_BG.a);
    SDL_RenderClear(renderer);

    // Update the current selection based on the mouse position.
    int mouseX, mouseY;
    SDL_GetMouseState(&mouseX, &mouseY);
    for (int i = 0; i < buttonCount; i++) {
      if (pointInRect(mouseX, mouseY, &buttons[i].rect)) {
        selectedButton = i;
        break;
      }
    }

    // Compute a pulse factor (using a sine wave) for the selected button.
    Uint32 elapsed = SDL_GetTicks() - startTime;
    float pulse = 0.05f * sinf(elapsed / 200.0f);

    // Render all buttons.
    for (int i = 0; i < buttonCount; i++) {
      bool isSelected = (i == selectedButton);
      renderButton(renderer, font, &buttons[i], isSelected, pulse);
    }

    SDL_RenderPresent(renderer);
    SDL_Delay(16); // ~60 FPS.
  }

  printf("Exiting runMainMenu()\n");
  TTF_CloseFont(font);
}
