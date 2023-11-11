extern "C" {
#include "entities/bullet.h"
#include "entities/humanoid.h"
}

#include <climits>
#include <gtest/gtest.h>

// Mock objects and functions, assuming they exist.
const int INITIAL_CAPACITY = 10;

class HumanoidTest : public ::testing::Test {
protected:
  Humanoid humanoid;
  SDL_Texture *texture;
  AnimationState animState;
  MovementState moveState;

  void SetUp() override {
    animState = {1, true, true, true};
    moveState = {10, 10, 5, 5};
    humanoidConstructor(&humanoid, animState, moveState, nullptr);
  }

  void TearDown() override { humanoidDestructor(&humanoid); }
};

// Test the parameterized constructor with valid arguments.
TEST_F(HumanoidTest, ParameterizedConstructor_ValidArgs) {

  EXPECT_EQ(humanoid.animation.currentSpriteIndex, 1);
  EXPECT_TRUE(humanoid.animation.isFacingLeft);
  EXPECT_TRUE(humanoid.animation.isWalking);
  EXPECT_TRUE(humanoid.animation.isVisible);
  EXPECT_EQ(humanoid.movement.position.x, 10);
  EXPECT_EQ(humanoid.movement.position.y, 10);
  EXPECT_EQ(humanoid.movement.velocity.x, 5);
  EXPECT_EQ(humanoid.movement.velocity.y, 5);
  EXPECT_EQ(humanoid.life, 100);
  EXPECT_TRUE(humanoid.isAlive);
  // Assuming the parameterized constructor also initializes bullets.
  EXPECT_EQ(humanoid.bulletManager.bullets.size, 10);
  EXPECT_EQ(humanoid.bulletManager.bullets.capacity, INITIAL_CAPACITY);
  // Add more assertions as necessary for full coverage.
}

// Test the parameterized constructor with edge case arguments.
TEST_F(HumanoidTest, ParameterizedConstructor_EdgeCases) {
  // Here you would test the constructor with edge case values like extremely
  // high or low numbers, nullptrs, etc. For example:
  humanoidConstructor(&humanoid, 1, true, true, INT_MAX, INT_MAX, INT_MAX,
                      INT_MAX, nullptr, INT_MAX, true, true);
  // Verify the results are as expected.
}

// Test the copy constructor.
TEST_F(HumanoidTest, CopyConstructor) {
  // 1. Create a source Humanoid and initialize with distinct data
  humanoidConstructor(&humanoid, animState, moveState, texture);
  humanoid.life = 80; // Change some data to test copying
  humanoid.isAlive = true;
  humanoid.animation.currentSpriteIndex = 5;
  humanoid.movement.position.x = 15;
  humanoid.movement.position.y = 20;

  // 2. Invoke the copy constructor
  Humanoid copiedHumanoid;
  humanoidCopyConstructor(&copiedHumanoid, &humanoid);

  // 3. Assert equalities for deep copy
  EXPECT_EQ(copiedHumanoid.life, humanoid.life);
  EXPECT_EQ(copiedHumanoid.isAlive, humanoid.isAlive);
  EXPECT_EQ(copiedHumanoid.animation.currentSpriteIndex,
            humanoid.animation.currentSpriteIndex);
  EXPECT_EQ(copiedHumanoid.movement.position.x, humanoid.movement.position.x);
  EXPECT_EQ(copiedHumanoid.movement.position.y, humanoid.movement.position.y);
}

// Test the destructor.
TEST_F(HumanoidTest, Destructor) {
  Humanoid *humanoid = new Humanoid;
  humanoidConstructor(humanoid, animState, moveState, texture);

  // Simulate allocating bullets or other resources.
  humanoid->bulletManager.bullets.items =
      static_cast<void **>(malloc(INITIAL_CAPACITY * sizeof(void *)));
  for (int i = 0; i < INITIAL_CAPACITY; ++i) {
    humanoid->bulletManager.bullets.items[i] = malloc(sizeof(Bullet));
  }
  humanoid->bulletManager.bullets.capacity = INITIAL_CAPACITY;
  humanoid->bulletManager.bullets.size = INITIAL_CAPACITY;

  humanoidDestructor(humanoid);

  EXPECT_EQ(humanoid->bulletManager.bullets.items, nullptr);
  EXPECT_EQ(humanoid->bulletManager.bullets.size, 0);
  EXPECT_EQ(humanoid->bulletManager.bullets.capacity, 0);

  delete humanoid; // Clean up the humanoid.
}
