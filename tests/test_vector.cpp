extern "C" {
#include "utils/vector.h"
}
#include <gtest/gtest.h>

// Test fixture for Vector tests
class VectorTest : public ::testing::Test {
protected:
  Vector *v;

  void SetUp() override {
    v = (Vector *)malloc(sizeof(Vector));
    vectorInit(v, 10, sizeof(int), NULL); // Initialize with no constructor
  }

  void TearDown() override {
    free(v->items);
    free(v);
  }
};

// Test initializing the vector
TEST_F(VectorTest, InitializesCorrectly) {
  EXPECT_EQ(v->size, 0);
  EXPECT_EQ(v->capacity, 10);
}

// Test appending to the vector
TEST_F(VectorTest, AppendsCorrectly) {
  int item = 5;
  vectorAppend(v, &item, nullptr);
  EXPECT_EQ(v->size, 1);
  EXPECT_EQ(*((int *)v->items[0]), 5);
}

typedef struct {
  int a;
  int b;
} CustomStruct;

void customStructConstructor(void *obj, va_list args) {
  CustomStruct *customObj = (CustomStruct *)obj;
  customObj->a = va_arg(args, int);
  customObj->b = va_arg(args, int);
}

class VectorCustomStructTest : public ::testing::Test {
protected:
  Vector *v;

  void SetUp() override {
    v = (Vector *)malloc(sizeof(Vector));
    vectorInit(v, 5, sizeof(CustomStruct), customStructConstructor, 10, 20);
  }

  void TearDown() override {
    for (size_t i = 0; i < v->size; i++) {
      free(v->items[i]); // Free individual items
    }
    free(v->items);
    free(v);
  }
};

TEST_F(VectorCustomStructTest, InitializesCustomStructCorrectly) {
  ASSERT_EQ(v->size, 5);
  for (size_t i = 0; i < v->size; i++) {
    CustomStruct *item = (CustomStruct *)v->items[i];
    EXPECT_EQ(item->a, 10);
    EXPECT_EQ(item->b, 20);
  }
}

TEST_F(VectorCustomStructTest, AppendsCustomStructObject) {
  CustomStruct newItem = {30, 40};
  vectorAppend(v, &newItem, nullptr);
  ASSERT_EQ(v->size, 6);

  CustomStruct *item = (CustomStruct *)v->items[5];
  EXPECT_EQ(item->a, 30);
  EXPECT_EQ(item->b, 40);
}
