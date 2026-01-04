#include "gtest/gtest.h"

extern "C" {
#include "entities/player.h"
}

// Test fixture for player kneeling tests
class PlayerKneelTest : public ::testing::Test {
protected:
  Player *player;

  void SetUp() override {
    player = playerCreate(100.0f, 100.0f);
  }

  void TearDown() override {
    playerDestroy(player);
  }
};

// Test that player is not kneeling initially
TEST_F(PlayerKneelTest, InitiallyNotKneeling) {
  ASSERT_NE(player, nullptr);
  EXPECT_FALSE(playerIsKneeling(player));
}

// Test that player can start kneeling
TEST_F(PlayerKneelTest, CanStartKneeling) {
  ASSERT_NE(player, nullptr);
  
  playerStartKneeling(player);
  
  EXPECT_TRUE(playerIsKneeling(player));
}

// Test that player can stop kneeling
TEST_F(PlayerKneelTest, CanStopKneeling) {
  ASSERT_NE(player, nullptr);
  
  playerStartKneeling(player);
  EXPECT_TRUE(playerIsKneeling(player));
  
  playerStopKneeling(player);
  EXPECT_FALSE(playerIsKneeling(player));
}

// Test kneeling with null player pointer
TEST_F(PlayerKneelTest, HandleNullPointer) {
  EXPECT_FALSE(playerIsKneeling(nullptr));
  
  // These should not crash
  playerStartKneeling(nullptr);
  playerStopKneeling(nullptr);
}

// Test multiple kneel/stop cycles
TEST_F(PlayerKneelTest, MultipleKneelCycles) {
  ASSERT_NE(player, nullptr);
  
  for (int i = 0; i < 5; i++) {
    playerStartKneeling(player);
    EXPECT_TRUE(playerIsKneeling(player));
    
    playerStopKneeling(player);
    EXPECT_FALSE(playerIsKneeling(player));
  }
}

// Test that kneeling state persists
TEST_F(PlayerKneelTest, KneelingStatePersists) {
  ASSERT_NE(player, nullptr);
  
  playerStartKneeling(player);
  EXPECT_TRUE(playerIsKneeling(player));
  
  // Check multiple times to ensure state persists
  EXPECT_TRUE(playerIsKneeling(player));
  EXPECT_TRUE(playerIsKneeling(player));
}
