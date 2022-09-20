#include <stddef.h>
#include <stdlib.h>

#include "humanoid.h"
#include "vector.h"

void vectorConstructor(Vector *vector, int maxSize, Type type) {
  vector->maxSize = maxSize;
  vector->type = type;
  switch (vector->type) {
  case BULLET:
    vector->data = malloc(sizeof(Bullet *) * maxSize);
    break;
  case HUMANOID:
    vector->data = malloc(sizeof(Humanoid *) * maxSize);
    break;
  }
  for (int i = 0; i < maxSize; i++) {
    vector->data[i] = NULL;
  }
}

void append(Vector *vector, void *newElement) {

  if (vector->size >= vector->maxSize) {
    return;
  }

  int i = vector->size;

  switch (vector->type) {
  case BULLET:
    vector->data[i] = malloc(sizeof(Bullet));
    bulletCopyConstructor(vector->data[i], (Bullet *)newElement);
    break;
  case HUMANOID:
    vector->data[i] = malloc(sizeof(Humanoid));
    humanoidCopyConstructor(vector->data[i], (Humanoid *)newElement);
    break;
  }
  vector->size++;
}

void removeFromVector(Vector *vector, int i) {

  if (!vector->data[i]) {
    return;
  }

  switch (vector->type) {
  case BULLET:
    break;
  case HUMANOID:
    humanoidDestructor(vector->data[i]);
    break;
  }

  free(vector->data[i]);
  vector->data[i] = NULL;
  for (int j = i + 1; j < vector->size; j++) {
    vector->data[j - 1] = vector->data[j];
  }
  vector->data[vector->size - 1] = NULL;
  vector->size--;
}

void clear(Vector *vector) {
  for (int i = vector->size - 1; i >= 0; i--)
    removeFromVector(vector, i);

  free(vector->data);
  vector->data = NULL;
  vector->size = 0;
  vector->maxSize = 0;
}