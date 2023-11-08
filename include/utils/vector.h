// vector.h
#ifndef VECTOR_H
#define VECTOR_H

#include <stddef.h>

typedef void (*VectorDestructor)(void *);

typedef struct {
  void **items;
  size_t size;
  size_t capacity;
  VectorDestructor destructor;
} Vector;

void vectorInit(Vector *vector, size_t initialCapacity,
                VectorDestructor destructor);
int vectorAppend(Vector *vector, void *item);
void *vectorGet(Vector *vector, size_t index);
int vectorRemove(Vector *vector, size_t index);
void vectorClear(Vector *vector);
void vectorFree(Vector *vector);
Vector *vectorCopy(const Vector *source);

#endif // VECTOR_H
