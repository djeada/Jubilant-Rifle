#ifndef MOVEMENT_STATE_H
#define MOVEMENT_STATE_H
#include "utils/point.h"
#include <stdbool.h>

typedef struct {
  Point position;
  Point velocity;
} MovementState;

void movementStateConstructor(MovementState *state, Point position,
                              Point velocity);
void movementStateMoveHorizontal(MovementState *movement);
void movementStateMoveJump(MovementState *movement);
void movementStateStop(MovementState *movement);
void movementStateFall(MovementState *movement);
bool movementStateIsJumping(MovementState *movement);
bool movementStateIsMoving(MovementState *movement);

#endif
