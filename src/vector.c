#include "stdlib.h"
#include "vector.h"

#include "humanoid.h"

/*
void append(Info *info, void** vector, void* data) {

  int found = -1;
  for (int i = 0; i < 1000; i++) {
    if (vector[i] == NULL) {
      found = i;
      break;
    }
  }

  if (found >= 0) {
    int i = found;

    switch(info->type) {
      case BULLET:
        vector[i] = malloc(sizeof(Bullet));
        Bullet *bulletData = (Bullet*)data;
        ((Bullet*)vector[i])->x = bulletData->x;
        ((Bullet*)vector[i])->y = bulletData->y;
        ((Bullet*)vector[i])->dx = bulletData->dx;
        break;
      case HUMANOID:
        vector[i] = malloc(sizeof(Humanoid));
        break;
    }

  }
}*/

void append(Vector* vector, void* newElement) {
  int found = -1;
  for (int i = 0; i < 1000; i++) {
    if (vector->data[i] == NULL) {
      found = i;
      break;
    }
  }

  if (found >= 0) {
    int i = found;

    switch(vector->type) {
      case BULLET:
        vector->data[i] = malloc(sizeof(Bullet));
        Bullet *bulletData = (Bullet*)newElement;
        ((Bullet*)vector->data[i])->x = bulletData->x;
        ((Bullet*)vector->data[i])->y = bulletData->y;
        ((Bullet*)vector->data[i])->dx = bulletData->dx;
        break;
      case HUMANOID:
        vector->data[i] = malloc(sizeof(Humanoid));
        break;
    }

  }
}