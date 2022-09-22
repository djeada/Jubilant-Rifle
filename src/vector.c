#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>

#include "humanoid.h"
#include "vector.h"

void vectorConstructor(Vector *vector, int maxSize, Type type) {
  vector->maxSize = maxSize;
  vector->type = type;
  switch (vector->type) {
  case BULLET:
    vector->data = malloc(sizeof(Bullet) * maxSize);
    break;
  case HUMANOID:
    vector->data = malloc(sizeof(Humanoid) * maxSize);
    break;
  }
}

void append(Vector *vector, void *newElement) {

  if (vector->size >= vector->maxSize) {
    return;
  }

  int i = vector->size;

  switch (vector->type) {
  case BULLET:
    bulletCopyConstructor((Bullet *)(vector + i * sizeof(Bullet)),
                          (Bullet *)newElement);
    break;
  case HUMANOID:
    humanoidCopyConstructor((Humanoid *)(vector + i * sizeof(Humanoid)),
                            (Humanoid *)newElement);
    break;
  }
  vector->size++;
}

void removeFromVector(Vector *vector, int i) {
  printf("removing from vecto i: %d \n", i);
  printf("vector size: %d \n", vector->size);

  if (i < 0 || i >= vector->size) {
    return;
  }

  vector->size--;
}

void clear(Vector *vector) {
  free(vector->data);
  vector->data = NULL;
  vector->size = 0;
  vector->maxSize = 0;
}
