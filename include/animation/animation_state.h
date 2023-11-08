#ifndef ANIMATION_STATE_H
#define ANIMATION_STATE_H
#include <stdbool.h>

typedef struct {
  int currentSpriteIndex;
  bool isFacingLeft;
  bool isWalking;
  bool isVisible;
} AnimationState;

void animationStateHide(AnimationState *animationState);
void animationStateShow(AnimationState *animationState);
void animationStateStop(AnimationState *animationState);
void animationStateIncrementSprite(AnimationState *animationState);

#endif
