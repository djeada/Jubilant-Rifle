extern "C" {
#include "map/map.h"
#include "utils/utils.h"
// Other C headers
}

// Now include your C++ headers
#include <cstdio>
#include <gtest/gtest.h>

// Helper function to create a dummy map file for testing
void createDummyMapFile(const char *filename, const char *content) {
  FILE *file = fopen(filename, "w");
  if (file != nullptr) {
    fputs(content, file);
    fclose(file);
  }
}

// Test fixture for Map tests
class MapTest : public ::testing::Test {
protected:
  Map testMap;
  const char *dummyMapFilename = "dummy_map.txt";

  void SetUp() override {
    // Initialize your Map structure if needed
    testMap.backgroundImage = nullptr;
    testMap.platforms = nullptr;
    testMap.platformCount = 0;
  }

  void TearDown() override {
    freeMapResources(&testMap);
    std::remove(
        dummyMapFilename); // Clean up the dummy map file after each test
  }
};

// Test case for a valid map file
TEST_F(MapTest, ParsesValidMapFile) {
  const char *mapContent = "background_image = path/to/image.png\n"
                           "platform_count = 2\n"
                           "platform1 = {100, 200, 50, 20}\n"
                           "platform2 = {300, 400, 60, 30}\n";

  createDummyMapFile(dummyMapFilename, mapContent);

  EXPECT_EQ(parseMapFile(dummyMapFilename, &testMap), 0)
      << "Failed to parse valid map file";
  EXPECT_STREQ(testMap.backgroundImage, "path/to/image.png")
      << "Background image path mismatch";
  EXPECT_EQ(testMap.platformCount, 2) << "Platform count mismatch";
  EXPECT_EQ(testMap.platforms[0].x, 100) << "Platform 1 X coordinate mismatch";
  EXPECT_EQ(testMap.platforms[1].y, 400) << "Platform 2 Y coordinate mismatch";
}

// Test case for a map file with invalid platform count
TEST_F(MapTest, HandlesInvalidPlatformCount) {
  const char *mapContent = "background_image = path/to/image.png\n"
                           "platform_count = -1\n"; // Invalid platform count

  createDummyMapFile(dummyMapFilename, mapContent);

  EXPECT_EQ(parseMapFile(dummyMapFilename, &testMap), -1)
      << "Should fail with invalid platform count";
}

// Additional test cases can be added here to cover more scenarios such as:
// - Map file not found
// - Memory allocation failures
// - Platform parsing errors
// - Platform count mismatches
// - And any other edge cases you expect to handle

int main(int argc, char **argv) {
  ::testing::InitGoogleTest(&argc, argv);
  return RUN_ALL_TESTS();
}
