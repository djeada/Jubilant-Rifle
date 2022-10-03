#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>

#include "humanoid.h"
#include "vector.h"

void vectorConstructor(Vector *vector, int maxSize, Type type) {
  vector->maxSize = maxSize;
  vector->type = type;
  vector->size = 0;
  switch (vector->type) {
  case BULLET:
    vector->data = malloc(sizeof(Bullet) * maxSize);
    break;
  case HUMANOID:
    vector->data = malloc(sizeof(Humanoid) * maxSize);
    for (int i = 0; i < maxSize; i++) {
      Humanoid *humanoid = &((Humanoid *)vector->data)[i];
      humanoidDefaultConstructor(humanoid);
    }
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
    bulletCopyConstructor(&((Bullet *)vector->data)[i], (Bullet *)newElement);
    break;
  case HUMANOID:
    humanoidCopyConstructor(&((Humanoid *)vector->data)[i],
                            (Humanoid *)newElement);
    break;
  }
  vector->size++;
}

void removeFromVector(Vector *vector, int i) {

  if (i < 0 || i >= vector->size) {
    return;
  }

  switch (vector->type) {
  case BULLET:
    for (int j = i; j < vector->size - 1; j++) {

      bulletCopyConstructor(&((Bullet *)vector->data)[j],
                            &((Bullet *)vector->data)[j + 1]);
    }

    break;
  case HUMANOID:
    for (int j = i; j < vector->size - 1; j++) {

      humanoidCopyConstructor(&((Humanoid *)vector->data)[j],
                              &((Humanoid *)vector->data)[j + 1]);
    }
    humanoidDestructor(&((Humanoid *)vector->data)[vector->size - 1]);
    break;
  }

  vector->size--;
}

void clear(Vector *vector) {
  switch (vector->type) {
  case BULLET:
    break;
  case HUMANOID:
    for (int i = 0; i < vector->size; i++)
      humanoidDestructor(&((Humanoid *)vector->data)[i]);
    break;
  }

  free(vector->data);
  vector->data = NULL;
  vector->size = 0;
  vector->maxSize = 0;
}
