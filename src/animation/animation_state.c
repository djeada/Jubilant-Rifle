#include "animation/animation_state.h"
#include <stdlib.h>

void animationStateConstructor(AnimationState *state, int spriteIndex,
                               bool facingLeft, bool walking, bool visible) {
  state->currentSpriteIndex = spriteIndex;
  state->isFacingLeft = facingLeft;
  state->isWalking = walking;
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

void animationStateStop(AnimationState *animationState) {
  if (animationState != NULL) {
    animationState->isWalking = false;
  }
}

void animationStateIncrementSprite(AnimationState *animationState) {
  if (animationState != NULL) {
    animationState->currentSpriteIndex++;
    animationState->currentSpriteIndex %= 4;
  }
}
