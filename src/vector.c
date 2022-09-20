#include <stddef.h>
#include <stdlib.h>

#include "humanoid.h"
#include "vector.h"

void vectorConstructor(Vector *vector, int maxSize, Type type) {
  vector->size = 0;
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
    Bullet *bulletData = (Bullet *)newElement;
    bulletConstructor(vector->data[i], bulletData->x, bulletData->y,
                      bulletData->dx);
    break;
  case HUMANOID:
    vector->data[i] = malloc(sizeof(Humanoid));
    Humanoid *humanoidData = (Humanoid *)newElement;
    humanoidConstructor(vector->data[i], humanoidData->x, humanoidData->y,
                        humanoidData->currentSprite, humanoidData->alive,
                        humanoidData->visible, humanoidData->facingLeft,
                        humanoidData->texture);
    break;
  }
  vector->size++;
}

void removeFromVector(Vector *vector, int i) {

  if (!vector->data[i] || i >= vector->size) {
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
  for (int j = i; j < vector->size; j++) {
    vector->data[j] = vector->data[j + 1];
  }
  vector->data[vector->size-1] = NULL;
  vector->size--;
}

void clear(Vector *vector) {
  for (int i = vector->size-1; i >= 0; i--)
    removeFromVector(vector, i);

  free(vector);
}