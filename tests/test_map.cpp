extern "C" {
#include "map/map.h"
}

#include <gtest/gtest.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// Mock JSON content for testing.
const char* mockMapFileContent = R"({
  "background_image": "/home/adam/Jubilant-Rifle/resources/textures/background.png",
  "width": 8000,
  "height": 1000,
  "platforms": [
    { "x": 100, "y": 880, "width": 300, "height": 20 },
    { "x": 450, "y": 760, "width": 250, "height": 20 },
    { "x": 800, "y": 640, "width": 400, "height": 20 },
    { "x": 1250, "y": 520, "width": 350, "height": 20 },
    { "x": 1600, "y": 400, "width": 300, "height": 20 },
    { "x": 2000, "y": 880, "width": 280, "height": 20 },
    { "x": 2400, "y": 760, "width": 320, "height": 20 },
    { "x": 2800, "y": 640, "width": 380, "height": 20 },
    { "x": 3200, "y": 520, "width": 420, "height": 20 },
    { "x": 3600, "y": 400, "width": 350, "height": 20 },
    { "x": 4000, "y": 880, "width": 300, "height": 20 },
    { "x": 4400, "y": 760, "width": 250, "height": 20 },
    { "x": 4800, "y": 640, "width": 400, "height": 20 },
    { "x": 5200, "y": 520, "width": 350, "height": 20 },
    { "x": 5600, "y": 400, "width": 300, "height": 20 },
    { "x": 6000, "y": 880, "width": 280, "height": 20 },
    { "x": 6400, "y": 760, "width": 320, "height": 20 },
    { "x": 6800, "y": 640, "width": 380, "height": 20 },
    { "x": 7200, "y": 520, "width": 420, "height": 20 },
    { "x": 7600, "y": 400, "width": 350, "height": 20 }
  ]
})";

class MapTest : public ::testing::Test {
protected:
    Map map;
    const char* testFileName = "test_map.json";

    void SetUp() override {
        // Write the mock JSON content to a test file.
        FILE *file = fopen(testFileName, "w");
        ASSERT_NE(file, nullptr);
        fprintf(file, "%s", mockMapFileContent);
        fclose(file);

        // Initialize map fields.
        map.backgroundImage = NULL;
        map.platforms = NULL;
        map.platformCount = 0;
        map.rect.x = map.rect.y = map.rect.w = map.rect.h = 0;
    }

    void TearDown() override {
        mapDestructor(&map);
        remove(testFileName);
    }
};

TEST_F(MapTest, AllocatePlatforms) {
    EXPECT_EQ(allocatePlatforms(&map, 10), 0);
    ASSERT_NE(map.platforms, nullptr);
    EXPECT_EQ(map.platformCount, 10);

    // Ensure that reallocating doesn't cause issues.
    EXPECT_EQ(allocatePlatforms(&map, 5), 0);
    ASSERT_NE(map.platforms, nullptr);
    EXPECT_EQ(map.platformCount, 5);
}

TEST_F(MapTest, AllocateBackgroundImagePath) {
    const char* path = "background.png";
    char* allocatedPath = allocateBackgroundImagePath(path);
    ASSERT_NE(allocatedPath, nullptr);
    EXPECT_STREQ(allocatedPath, path);
    free(allocatedPath);
}

TEST_F(MapTest, ParseStringValue) {
    const char* json = "{\"background_image\": \"background.png\"}";
    char value[256];
    EXPECT_EQ(parseStringValue(json, "\"background_image\"", value, sizeof(value)), 0);
    EXPECT_STREQ(value, "background.png");
}

TEST_F(MapTest, ParseIntValue) {
    const char* json = "{\"width\": 800, \"height\": 600}";
    int value = 0;
    EXPECT_EQ(parseIntValue(json, "\"width\"", &value), 0);
    EXPECT_EQ(value, 800);
    EXPECT_EQ(parseIntValue(json, "\"height\"", &value), 0);
    EXPECT_EQ(value, 600);
}

TEST_F(MapTest, ParsePlatforms) {
    const char* json = R"({
        "platforms": [
            {"x": 10, "y": 20, "width": 30, "height": 40},
            {"x": 50, "y": 60, "width": 70, "height": 80}
        ]
    })";
    Platform platforms[2];
    EXPECT_EQ(parsePlatforms(json, platforms, 2), 0);
    EXPECT_EQ(platforms[0].x, 10);
    EXPECT_EQ(platforms[0].y, 20);
    EXPECT_EQ(platforms[0].w, 30);
    EXPECT_EQ(platforms[0].h, 40);
    EXPECT_EQ(platforms[1].x, 50);
    EXPECT_EQ(platforms[1].y, 60);
    EXPECT_EQ(platforms[1].w, 70);
    EXPECT_EQ(platforms[1].h, 80);
}

TEST_F(MapTest, ParseMapFile_Valid) {
    EXPECT_EQ(parseMapFile(testFileName, &map), 0);
    EXPECT_STREQ(map.backgroundImage, "/home/adam/Jubilant-Rifle/resources/textures/background.png");
    EXPECT_EQ(map.rect.w, 8000);
    EXPECT_EQ(map.rect.h, 1000);
    EXPECT_EQ(map.platformCount, 20);

    // Check a couple of platform values.
    EXPECT_EQ(map.platforms[0].x, 100);
    EXPECT_EQ(map.platforms[0].y, 880);
    EXPECT_EQ(map.platforms[0].w, 300);
    EXPECT_EQ(map.platforms[0].h, 20);
    EXPECT_EQ(map.platforms[19].x, 7600);
    EXPECT_EQ(map.platforms[19].y, 400);
    EXPECT_EQ(map.platforms[19].w, 350);
    EXPECT_EQ(map.platforms[19].h, 20);
}

TEST_F(MapTest, Destructor) {
    map.backgroundImage = strdup("background.png");
    ASSERT_EQ(allocatePlatforms(&map, 5), 0);
    mapDestructor(&map);
    EXPECT_EQ(map.backgroundImage, nullptr);
    EXPECT_EQ(map.platforms, nullptr);
    EXPECT_EQ(map.platformCount, 0);
}
