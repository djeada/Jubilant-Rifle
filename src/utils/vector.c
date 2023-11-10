#include "utils/vector.h"
#include <stdlib.h>

#include <string.h>

static int vectorResize(Vector *vector, size_t newCapacity) {
  void **items = realloc(vector->items, sizeof(void *) * newCapacity);
  if (items) {
    vector->items = items;
    vector->capacity = newCapacity;
    return 0;
  }
  return -1;
}

void vectorInit(Vector *vector, size_t initialCapacity, size_t typeSize,
                ConstructorFunc constructor, ...) {
  vector->size = 0; // Reset size
  vector->capacity = initialCapacity;
  vector->itemSize = typeSize;
  vector->items = malloc(sizeof(void *) * vector->capacity);

  if (vector->items && constructor) {
    va_list args;
    va_start(args, constructor);
    for (size_t i = 0; i < vector->capacity; i++) {
      vector->items[i] = malloc(vector->itemSize);
      if (vector->items[i] != NULL) {
        va_list args_copy;
        va_copy(args_copy, args);
        constructor(vector->items[i], args_copy);
        va_end(args_copy);
        vector->size++; // Update size for each initialized item
      }
    }
    va_end(args);
  }
}

int vectorAppend(Vector *vector, void *item, ConstructorFunc constructor, ...) {
  if (vector->size == vector->capacity) {
    if (vectorResize(vector, vector->capacity * 2) == -1)
      return -1;
  }

  vector->items[vector->size] = malloc(vector->itemSize); // Allocate memory
  if (vector->items[vector->size] == NULL) {
    return -1; // Allocation failed
  }

  va_list args;
  va_start(args, constructor);
  if (constructor) {
    constructor(vector->items[vector->size], args);
  } else {
    memcpy(vector->items[vector->size], item,
           vector->itemSize); // Copy the item
  }
  va_end(args);

  vector->size++;

  return 0;
}

Vector *vectorCopy(const Vector *source) {
  if (!source) {
    return NULL;
  }

  // Allocate memory for the new vector
  Vector *copy = malloc(sizeof(Vector));
  if (!copy) {
    return NULL; // Return NULL if the allocation failed
  }

  // Copy vector metadata
  copy->size = source->size;
  copy->capacity = source->capacity;

  // Allocate memory for the new items
  copy->items = malloc(sizeof(void *) * source->capacity);
  if (!copy->items) {
    free(copy);
    return NULL; // Return NULL if the allocation failed
  }

  // Copy items from the source vector to the new vector
  for (size_t i = 0; i < source->size; i++) {
    // Shallow copy of items
    copy->items[i] = source->items[i];
  }

  // Initialize the rest of the capacity with NULL
  for (size_t i = source->size; i < source->capacity; i++) {
    copy->items[i] = NULL;
  }

  return copy;
}

void *vectorGet(Vector *vector, size_t index) {
  if (index < vector->size)
    return vector->items[index];
  return NULL;
}

int vectorRemove(Vector *vector, size_t index, DestructorFunc destructor) {
  if (index >= vector->size)
    return -1;

  if (destructor)
    destructor(vector->items[index]);

  for (size_t i = index; i < vector->size - 1; i++) {
    vector->items[i] = vector->items[i + 1];
  }
  vector->size--;

  if (vector->size > 0 && vector->size == vector->capacity / 4) {
    return vectorResize(vector, vector->capacity / 2);
  }
  return 0;
}

void vectorClear(Vector *vector, DestructorFunc destructor) {
  if (destructor) {
    for (size_t i = 0; i < vector->size; i++) {
      destructor(vector->items[i]);
    }
  }
  vector->size = 0;
}

void vectorFree(Vector *vector, DestructorFunc destructor) {
  vectorClear(vector, destructor);
  free(vector->items);
  vector->items = NULL;
  vector->capacity = 0;
}
