extern "C" {
#include "map/map.h"
#include "utils/utils.h"
// Other C headers
}

#include <cstdio>
#include <gtest/gtest.h>
#include <memory>

// Helper function to create a dummy map file for testing
void createDummyMapFile(const char *filename, const char *content) {
  std::unique_ptr<FILE, decltype(&fclose)> file(fopen(filename, "w"), &fclose);
  if (file) {
    fputs(content, file.get());
  }
}

// Helper function to create valid map content
std::string createValidMapContent() {
  return "background_image = path/to/image.png\n"
         "platform_count = 2\n"
         "platform1 = {100, 200, 50, 20}\n"
         "platform2 = {300, 400, 60, 30}\n";
}

// Helper function to create invalid map content
std::string createInvalidMapContent() {
  return "background_image = path/to/image.png\n"
         "platform_count = -1\n";
}

// Test fixture for Map tests
class MapTest : public ::testing::Test {
protected:
  Map testMap;
  const char *dummyMapFilename = "dummy_map.txt";

  void SetUp() override {
    testMap.backgroundImage = nullptr;
    testMap.platforms = nullptr;
    testMap.platformCount = 0;
  }

  void TearDown() override {
    freeMapResources(&testMap);
    std::remove(dummyMapFilename);
  }
};

// Test case for a valid map file
TEST_F(MapTest, ParsesValidMapFile) {
  createDummyMapFile(dummyMapFilename, createValidMapContent().c_str());

  ASSERT_EQ(parseMapFile(dummyMapFilename, &testMap), 0)
      << "Failed to parse valid map file";

  EXPECT_STREQ(testMap.backgroundImage, "path/to/image.png")
      << "Background image path mismatch";
  ASSERT_EQ(testMap.platformCount, 2) << "Platform count mismatch";
  ASSERT_NE(testMap.platforms, nullptr) << "Platforms should not be null";

  EXPECT_EQ(testMap.platforms[0].x, 100) << "Platform 1 X coordinate mismatch";
  EXPECT_EQ(testMap.platforms[1].y, 400) << "Platform 2 Y coordinate mismatch";
}

// Test case for a map file with invalid platform count
TEST_F(MapTest, HandlesInvalidPlatformCount) {
  createDummyMapFile(dummyMapFilename, createInvalidMapContent().c_str());

  EXPECT_EQ(parseMapFile(dummyMapFilename, &testMap), -1)
      << "Should fail with invalid platform count";
}

// Test case for when map file is not found
TEST_F(MapTest, HandlesMissingMapFile) {
  EXPECT_EQ(parseMapFile("non_existent_map.txt", &testMap), -1)
      << "Should fail when map file is missing";
}

// Test case for platform parsing errors
TEST_F(MapTest, HandlesPlatformParsingErrors) {
  const char *invalidPlatformContent =
      "background_image = path/to/image.png\n"
      "platform_count = 2\n"
      "platform1 = {invalid_data}\n"
      "platform2 = {300, 400, 60, 30}\n"; // valid platform for control

  createDummyMapFile(dummyMapFilename, invalidPlatformContent);

  EXPECT_EQ(parseMapFile(dummyMapFilename, &testMap), -1)
      << "Should fail with platform parsing errors";
}

// Test case for platform count mismatch (more platforms than specified)
TEST_F(MapTest, HandlesPlatformCountMismatchMore) {
  const char *morePlatformsContent =
      "background_image = path/to/image.png\n"
      "platform_count = 1\n" // only one platform specified
      "platform1 = {100, 200, 50, 20}\n"
      "platform2 = {300, 400, 60, 30}\n"; // extra platform

  createDummyMapFile(dummyMapFilename, morePlatformsContent);

  EXPECT_EQ(parseMapFile(dummyMapFilename, &testMap), -1)
      << "Should fail when there are more platforms than specified";
}

// Test case for platform count mismatch (fewer platforms than specified)
TEST_F(MapTest, HandlesPlatformCountMismatchFewer) {
  const char *fewerPlatformsContent =
      "background_image = path/to/image.png\n"
      "platform_count = 3\n"              // three platforms specified
      "platform1 = {100, 200, 50, 20}\n"; // only one platform provided

  createDummyMapFile(dummyMapFilename, fewerPlatformsContent);

  EXPECT_EQ(parseMapFile(dummyMapFilename, &testMap), -1)
      << "Should fail when there are fewer platforms than specified";
}
