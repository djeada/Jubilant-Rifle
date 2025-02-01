#include "game/game_state.h"
#include "utils/consts.h"
#include <SDL2/SDL.h>
#include <SDL2/SDL_ttf.h>
#include <math.h>
#include <stdbool.h>
#include <stdio.h>

// --- Aesthetic constants ---
static const SDL_Color COLOR_BG = {50, 50, 50, 255};
static const SDL_Color COLOR_BUTTON = {20, 120, 20, 255};
static const SDL_Color COLOR_BUTTON_HOVER = {40, 200, 40, 255};
static const SDL_Color COLOR_BUTTON_BORDER = {255, 255, 255, 255};
static const SDL_Color COLOR_TEXT = {255, 255, 255, 255};
static const SDL_Color COLOR_TEXT_SHADOW = {0, 0, 0, 150};

typedef struct Button {
  SDL_Rect rect;
  const char *text;
  GameState targetState;
} Button;

SDL_Texture *renderText(SDL_Renderer *renderer, TTF_Font *font,
                        const char *text, SDL_Color color, int *w, int *h) {
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

// Helper: Render text with a slight shadow offset.
void renderTextWithShadow(SDL_Renderer *renderer, TTF_Font *font,
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

void runMainMenu(SDL_Renderer *renderer, GameState *state) {
  printf("Entering runMainMenu()\n");

  SDL_Event event;
  bool menuRunning = true;

  // Window dimensions (using values from consts.h).
  const int windowWidth = WINDOWED_MODE_WIDTH;
  const int windowHeight = WINDOWED_MODE_HEIGHT;

  // Button dimensions and positioning.
  const int buttonWidth = 250;
  const int buttonHeight = 60;
  const int spacing = 20;
  const int buttonX = (windowWidth - buttonWidth) / 2;
  int startY = windowHeight / 2 - ((buttonHeight * 5) + (spacing * 4)) / 2;

  // Define the buttons.
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

  // Load the font using the same FONT_PATH defined in consts.h.
  TTF_Font *font = TTF_OpenFont(FONT_PATH, 28);
  if (!font) {
    fprintf(stderr, "Failed to load font from '%s': %s\n", FONT_PATH,
            TTF_GetError());
    SDL_ShowSimpleMessageBox(SDL_MESSAGEBOX_ERROR, "Font Error",
                             "Failed to load font. Exiting...", NULL);
    *state = STATE_EXIT;
    return;
  }

  // Variables for the button pulse effect.
  Uint32 startTime = SDL_GetTicks();

  // Track the currently selected button (for keyboard navigation).
  int selectedButton = 0;

  // Main loop.
  while (menuRunning) {
    while (SDL_PollEvent(&event)) {
      if (event.type == SDL_QUIT) {
        *state = STATE_EXIT;
        menuRunning = false;
      } else if (event.type == SDL_MOUSEMOTION) {
        // Update the selected button based on mouse movement.
        int mx = event.motion.x;
        int my = event.motion.y;
        for (int i = 0; i < buttonCount; i++) {
          SDL_Rect r = buttons[i].rect;
          if (mx >= r.x && mx <= (r.x + r.w) && my >= r.y &&
              my <= (r.y + r.h)) {
            selectedButton = i;
            break;
          }
        }
      } else if (event.type == SDL_MOUSEBUTTONDOWN &&
                 event.button.button == SDL_BUTTON_LEFT) {
        int mx = event.button.x;
        int my = event.button.y;
        for (int i = 0; i < buttonCount; i++) {
          SDL_Rect r = buttons[i].rect;
          if (mx >= r.x && mx <= (r.x + r.w) && my >= r.y &&
              my <= (r.y + r.h)) {
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

    // Clear the screen with the background color.
    SDL_SetRenderDrawColor(renderer, COLOR_BG.r, COLOR_BG.g, COLOR_BG.b,
                           COLOR_BG.a);
    SDL_RenderClear(renderer);

    // Also get the mouse state so that if the mouse is hovering over a button,
    // we update the selection (this is useful if no MOUSEMOTION event occurs).
    int mouseX, mouseY;
    SDL_GetMouseState(&mouseX, &mouseY);
    for (int i = 0; i < buttonCount; i++) {
      SDL_Rect r = buttons[i].rect;
      if (mouseX >= r.x && mouseX <= (r.x + r.w) && mouseY >= r.y &&
          mouseY <= (r.y + r.h)) {
        selectedButton = i;
        break;
      }
    }

    // Compute pulse factor using a sine wave for a subtle animation.
    Uint32 elapsed = SDL_GetTicks() - startTime;
    float pulse =
        0.05f * sinf(elapsed / 200.0f); // variation between -0.05 and +0.05

    // Render each button.
    for (int i = 0; i < buttonCount; i++) {
      SDL_Rect btnRect = buttons[i].rect;
      // Highlight the button if it is the current selection.
      bool isSelected = (i == selectedButton);

      // Adjust scale for the pulse effect when selected.
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

      // Draw the button background (use the hover color if selected).
      SDL_Color btnColor = isSelected ? COLOR_BUTTON_HOVER : COLOR_BUTTON;
      SDL_SetRenderDrawColor(renderer, btnColor.r, btnColor.g, btnColor.b,
                             btnColor.a);
      SDL_RenderFillRect(renderer, &scaledRect);

      // Draw the button border.
      SDL_SetRenderDrawColor(renderer, COLOR_BUTTON_BORDER.r,
                             COLOR_BUTTON_BORDER.g, COLOR_BUTTON_BORDER.b,
                             COLOR_BUTTON_BORDER.a);
      SDL_RenderDrawRect(renderer, &scaledRect);

      // Render the button text (centered within the button).
      int textW, textH;
      SDL_Texture *tempTex = renderText(renderer, font, buttons[i].text,
                                        COLOR_TEXT, &textW, &textH);
      if (tempTex) {
        SDL_DestroyTexture(tempTex);
      }
      int textX = scaledRect.x + (scaledRect.w - textW) / 2;
      int textY = scaledRect.y + (scaledRect.h - textH) / 2;
      renderTextWithShadow(renderer, font, buttons[i].text, textX, textY);
    }

    SDL_RenderPresent(renderer);
    SDL_Delay(16); // Approximately 60 FPS.
  }

  printf("Exiting runMainMenu()\n");
  TTF_CloseFont(font);
}
