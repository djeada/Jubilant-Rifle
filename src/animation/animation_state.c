#include "animation/animation_state.h"
#include <stdlib.h>

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
