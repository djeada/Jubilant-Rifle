
extern "C" {
#include "entities/movement_state.h"
}

#include <gtest/gtest.h>

// Test fixture for MovementState tests
class MovementStateTest : public ::testing::Test {
protected:
  MovementState state;

  void SetUp() override {
    // Initialize with some default values
    movementStateConstructor(&state, 10, 10, 5, 5);
  }
};

// Test constructor initialization
TEST_F(MovementStateTest, ConstructorInitializesValues) {
  EXPECT_EQ(state.position.x, 10);
  EXPECT_EQ(state.position.y, 10);
  EXPECT_EQ(state.velocity.x, 5);
  EXPECT_EQ(state.velocity.y, 5);
}

// Test horizontal movement
TEST_F(MovementStateTest, MovesHorizontalCorrectly) {
  movementStateMoveHorizontal(&state);
  EXPECT_EQ(state.position.x, 15);
  EXPECT_EQ(state.velocity.x, 0);
}

// Test jump movement
TEST_F(MovementStateTest, MovesJumpCorrectly) {
  movementStateMoveJump(&state);
  EXPECT_EQ(state.position.y, 15);
  EXPECT_EQ(state.velocity.y, 0);
}

// Test stop function
TEST_F(MovementStateTest, StopsCorrectly) {
  movementStateStop(&state);
  EXPECT_EQ(state.velocity.x, 0);
  EXPECT_EQ(state.velocity.y, 0);
}

// Test fall function
TEST_F(MovementStateTest, FallsCorrectly) {
  int initialY = state.position.y;
  movementStateFall(&state);
  EXPECT_EQ(state.position.y, initialY + 5);
}

// Test jumping state
TEST_F(MovementStateTest, ReportsJumpingCorrectly) {
  EXPECT_TRUE(movementStateIsJumping(&state));
  movementStateMoveJump(&state); // After jump, velocity.y should be 0
  EXPECT_FALSE(movementStateIsJumping(&state));
}

// Test moving state
TEST_F(MovementStateTest, ReportsMovingCorrectly) {
  EXPECT_TRUE(movementStateIsMoving(&state));
  movementStateMoveHorizontal(&state); // After move, velocity.x should be 0
  EXPECT_FALSE(movementStateIsMoving(&state));
}
