#include "animation/animation_state.h"
#include <stdlib.h>

void animationStateConstructor(AnimationState *state, int spriteIndex,
                               bool facingLeft, bool visible) {
  state->currentSpriteIndex = spriteIndex;
  state->isFacingLeft = facingLeft;
  state->isVisible = visible;
}

void animationStateHide(AnimationState *animationState) {
  if (animationState != NULL) {
    animationState->isVisible = false;
  }
}

void animationStateShow(AnimationState *animationState) {
  if (animationState != NULL) {
    animationState->isVisible = true;
  }
}

void animationStateIncrementSprite(AnimationState *animationState) {
  if (animationState != NULL) {
    animationState->currentSpriteIndex++;
    animationState->currentSpriteIndex %= 4;
  }
}

void animationStateStop(AnimationState *animationState) {
  if (animationState != NULL) {
    animationState->currentSpriteIndex = 0;
  }
}
