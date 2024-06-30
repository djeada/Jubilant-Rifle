#include "entities/movement_state.h"
#include "utils/consts.h"
#include <stdio.h> // for sprintf

void movementStateConstructor(MovementState *state, Point position,
                              Point velocity) {

  pointCopyConstructor(&state->position, &position);
  pointCopyConstructor(&state->velocity, &velocity);
}

void movementStateMoveHorizontal(MovementState *movement) {

  movement->position.x += movement->velocity.x;
}
void movementStateMoveJump(MovementState *movement) {
  // Perform the jump by changing the y position
  movement->position.y += movement->velocity.y;
}

void movementStateStop(MovementState *movement) {
  movement->velocity.x = 0;
  movement->velocity.y = 0;
}

void movementStateFall(MovementState *movement) { 
  movement->position.y += FALL_INCREMENT; 
  }

bool movementStateIsJumping(MovementState *movement) {
  return movement->velocity.y != 0;
}

bool movementStateIsMoving(MovementState *movement) {
  return movement->velocity.x != 0;
}
