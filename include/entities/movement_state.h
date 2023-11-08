#ifndef MOVEMENT_STATE_H
#define MOVEMENT_STATE_H
#include "utils/utils.h"

typedef struct {
  Point position;
  Point velocity;
} MovementState;

void movementStateMoveHorizontal(MovementState *movement);
void movementStateMoveJump(MovementState *movement);
void movementStateStop(MovementState *movement);
void movementStateFall(MovementState *movement);
bool movementStateIsJumping(MovementState *movement);
bool movementStateIsMoving(MovementState *movement);
#endif
