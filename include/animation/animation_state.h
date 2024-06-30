#ifndef ANIMATION_STATE_H
#define ANIMATION_STATE_H
#include <stdbool.h>

typedef struct {
  int currentSpriteIndex;
  bool isFacingLeft;
  bool isVisible;
} AnimationState;

void animationStateConstructor(AnimationState *state, int spriteIndex,
                               bool facingLeft, bool visible);
void animationStateHide(AnimationState *animationState);
void animationStateShow(AnimationState *animationState);
void animationStateIncrementSprite(AnimationState *animationState);
void animationStateStop(AnimationState *animationState);

#endif
