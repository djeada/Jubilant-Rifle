#include "entities/movement_state.h"
#include <stdio.h> // for sprintf

void movementStateMoveHorizontal(MovementState *movement) {

  movement->position.x += movement->velocity.x;
  movement->velocity.x = 0;
}
void movementStateMoveJump(MovementState *movement) {
  /// char logMessage[100]; // Ensure this buffer is large enough for your log
  // messages

  // Log the position before the jump
  /// sprintf(logMessage, "Before JUMP: Position Y: %f VELOCity Y: %f",
  //       movement->position.y, movement->velocity.y);
  /// LogSDLError(logMessage);

  // Perform the jump by changing the y position
  movement->position.y += movement->velocity.y;
  movement->velocity.y = 0;

  // Log the position after the jump
  // sprintf(logMessage, "AFTER JUMP: Position Y: %f VELOCity Y: %f",
  //         movement->position.y, movement->velocity.y);
  // LogSDLError(logMessage);
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
