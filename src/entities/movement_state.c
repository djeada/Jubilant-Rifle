#include "entities/movement_state.h"
#include <stdio.h> // for sprintf
void movementStateConstructor(MovementState *state, int posX, int posY,
                              int velX, int velY) {
  state->position.x = posX;
  state->position.y = posY;
  state->velocity.x = velX;
  state->velocity.y = velY;
}

void movementStateMoveHorizontal(MovementState *movement) {

  movement->position.x += movement->velocity.x;
  movement->velocity.x = 0;
}
void movementStateMoveJump(MovementState *movement) {
  // Perform the jump by changing the y position
  movement->position.y += movement->velocity.y;
  movement->velocity.y = 0;
}

void movementStateStop(MovementState *movement) {
  movement->velocity.x = 0;
  movement->velocity.y = 0;
}

void movementStateFall(MovementState *movement) { movement->position.y += 5; }

bool movementStateIsJumping(MovementState *movement) {
  return movement->velocity.y != 0;
}

bool movementStateIsMoving(MovementState *movement) {
  return movement->velocity.x != 0;
}
