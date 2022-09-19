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
  int found = -1;
  for (int i = 0; i < 1000; i++) {
    if (vector->data[i] == NULL) {
      found = i;
      break;
    }
  }

  if (found >= 0) {
    int i = found;

    switch (vector->type) {
    case BULLET:
      vector->data[i] = malloc(sizeof(Bullet));
      Bullet *bulletData = (Bullet *)newElement;
      ((Bullet *)vector->data[i])->x = bulletData->x;
      ((Bullet *)vector->data[i])->y = bulletData->y;
      ((Bullet *)vector->data[i])->dx = bulletData->dx;
      break;
    case HUMANOID:
      vector->data[i] = malloc(sizeof(Humanoid));
      break;
    }
  }
}

void removeFromVector(Vector *vector, int i) {

  if (!vector->data[i])
    return;

  free(vector->data[i]);
  vector->data[i] = NULL;
}

void clear(Vector *vector) {
  for (int i = 0; i < vector->size; i++)
    removeFromVector(vector, i);
}