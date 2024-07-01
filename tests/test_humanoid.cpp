extern "C" {
#include "entities/bullet.h"
#include "entities/humanoid.h"
#include "utils/consts.h"
}

#include <climits>
#include <gtest/gtest.h>

const int INITIAL_CAPACITY = NUM_BULLETS;

class HumanoidTest : public ::testing::Test {
protected:
  Humanoid humanoid;
  SDL_Texture *texture;
  AnimationState animState;
  MovementState moveState;

  void SetUp() override {
    animState = {1, true, true};
    moveState = {10, 10, 5, 5};
    humanoidConstructor(&humanoid, animState, moveState, nullptr);
  }

  void TearDown() override { humanoidDestructor(&humanoid); }
};

TEST_F(HumanoidTest, ParameterizedConstructor_ValidArgs) {
  EXPECT_EQ(humanoid.animation.currentSpriteIndex, 1);
  EXPECT_TRUE(humanoid.animation.isFacingLeft);
  EXPECT_TRUE(humanoid.animation.isVisible);
  EXPECT_EQ(humanoid.movement.position.x, 10);
  EXPECT_EQ(humanoid.movement.position.y, 10);
  EXPECT_EQ(humanoid.movement.velocity.x, 5);
  EXPECT_EQ(humanoid.movement.velocity.y, 5);
  EXPECT_EQ(humanoid.life.current, 100);
  EXPECT_TRUE(lifeIsAlive(&humanoid.life));
  EXPECT_EQ(humanoid.bulletManager.bullets.size, NUM_BULLETS);
  EXPECT_EQ(humanoid.bulletManager.bullets.capacity, INITIAL_CAPACITY);
}

TEST_F(HumanoidTest, ParameterizedConstructor_EdgeCases) {
  humanoidConstructor(&humanoid, 1, true, true, INT_MAX, INT_MAX, INT_MAX,
                      INT_MAX, nullptr, INT_MAX, true, true);
}

TEST_F(HumanoidTest, CopyConstructor) {
  humanoidConstructor(&humanoid, animState, moveState, texture);
  humanoid.life.current = 80;
  humanoid.animation.currentSpriteIndex = 5;
  humanoid.movement.position.x = 15;
  humanoid.movement.position.y = 20;

  Humanoid copiedHumanoid;
  humanoidCopyConstructor(&copiedHumanoid, &humanoid);

  EXPECT_EQ(copiedHumanoid.life.current, humanoid.life.current);
  EXPECT_EQ(lifeIsAlive(&copiedHumanoid.life), lifeIsAlive(&humanoid.life));
  EXPECT_EQ(copiedHumanoid.animation.currentSpriteIndex,
            humanoid.animation.currentSpriteIndex);
  EXPECT_EQ(copiedHumanoid.movement.position.x, humanoid.movement.position.x);
  EXPECT_EQ(copiedHumanoid.movement.position.y, humanoid.movement.position.y);
}

TEST_F(HumanoidTest, Destructor) {
  Humanoid *humanoid = new Humanoid;
  humanoidConstructor(humanoid, animState, moveState, texture);

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

  delete humanoid;
}
