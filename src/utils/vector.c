// vector.c
#include "utils/vector.h"
#include <stdlib.h>

static int vectorResize(Vector *vector, size_t newCapacity) {
  void **items = realloc(vector->items, sizeof(void *) * newCapacity);
  if (items) {
    vector->items = items;
    vector->capacity = newCapacity;
    return 0;
  }
  return -1;
}

void vectorInit(Vector *vector, size_t initialCapacity,
                VectorDestructor destructor) {
  vector->size = 0;
  vector->capacity = initialCapacity;
  vector->destructor = destructor;
  vector->items = malloc(sizeof(void *) * vector->capacity);
}

int vectorAppend(Vector *vector, void *item) {
  if (vector->size == vector->capacity) {
    if (vectorResize(vector, vector->capacity * 2) == -1)
      return -1;
  }
  vector->items[vector->size++] = item;
  return 0;
}

void *vectorGet(Vector *vector, size_t index) {
  if (index < vector->size)
    return vector->items[index];
  return NULL;
}

int vectorRemove(Vector *vector, size_t index) {
  if (index >= vector->size)
    return -1;

  if (vector->destructor)
    vector->destructor(vector->items[index]);

  vector->items[index] = NULL;
  for (size_t i = index; i < vector->size - 1; i++) {
    vector->items[i] = vector->items[i + 1];
  }
  vector->size--;

  if (vector->size > 0 && vector->size == vector->capacity / 4) {
    return vectorResize(vector, vector->capacity / 2);
  }
  return 0;
}

void vectorClear(Vector *vector) {
  if (vector->destructor) {
    for (size_t i = 0; i < vector->size; i++) {
      vector->destructor(vector->items[i]);
    }
  }
  vector->size = 0;
}

void vectorFree(Vector *vector) {
  vectorClear(vector);
  free(vector->items);
  vector->items = NULL;
  vector->capacity = 0;
}

Vector *vectorCopy(const Vector *source) {
  // Allocate memory for the new vector
  Vector *copy = malloc(sizeof(Vector));
  if (!copy) {
    return NULL; // Return NULL if the allocation failed
  }

  // Copy vector metadata
  copy->size = source->size;
  copy->capacity = source->capacity;
  copy->destructor = source->destructor;

  // Allocate memory for the new items
  copy->items = malloc(sizeof(void *) * source->capacity);
  if (!copy->items) {
    free(copy);
    return NULL; // Return NULL if the allocation failed
  }

  // Copy items from the source vector to the new vector
  for (size_t i = 0; i < source->size; i++) {
    // If your items are simple data, a direct copy is possible.
    // For complex data types, you would need to duplicate the item properly.
    copy->items[i] = source->items[i];

    // For example, if your items are strings (char*), you would need to
    // duplicate them: copy->items[i] = strdup(source->items[i]);

    // If you have a copy function for your items, use it here:
    // copy->items[i] = your_item_copy_function(source->items[i]);
  }

  // For the rest of the capacity that's not yet utilized, set pointers to NULL
  for (size_t i = source->size; i < source->capacity; i++) {
    copy->items[i] = NULL;
  }

  return copy;
}
