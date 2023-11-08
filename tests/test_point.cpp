
extern "C" {
#include "utils/point.h"
}

#include <gtest/gtest.h>

// Test constructor
TEST(PointTest, Constructor) {
  Point p;
  pointConstructor(&p, 1.0f, 2.0f);
  EXPECT_FLOAT_EQ(p.x, 1.0f);
  EXPECT_FLOAT_EQ(p.y, 2.0f);
}

// Test copy constructor
TEST(PointTest, CopyConstructor) {
  Point p1, p2;
  pointConstructor(&p1, 3.0f, 4.0f);
  pointCopyConstructor(&p2, &p1);
  EXPECT_FLOAT_EQ(p2.x, p1.x);
  EXPECT_FLOAT_EQ(p2.y, p1.y);
}

// Test point addition
TEST(PointTest, Addition) {
  Point p1, p2, result;
  pointConstructor(&p1, 1.0f, 2.0f);
  pointConstructor(&p2, 3.0f, 4.0f);
  result = pointAdd(&p1, &p2);
  EXPECT_FLOAT_EQ(result.x, 4.0f);
  EXPECT_FLOAT_EQ(result.y, 6.0f);
}

// Test point subtraction
TEST(PointTest, Subtraction) {
  Point p1, p2, result;
  pointConstructor(&p1, 5.0f, 6.0f);
  pointConstructor(&p2, 3.0f, 4.0f);
  result = pointSubtract(&p1, &p2);
  EXPECT_FLOAT_EQ(result.x, 2.0f);
  EXPECT_FLOAT_EQ(result.y, 2.0f);
}

// Test point distance
TEST(PointTest, Distance) {
  Point p1, p2;
  pointConstructor(&p1, 0.0f, 0.0f);
  pointConstructor(&p2, 3.0f, 4.0f);
  float distance = pointDistance(&p1, &p2);
  EXPECT_FLOAT_EQ(
      distance, 5.0f); // Should match the hypotenuse of a 3-4-5 right triangle
}
