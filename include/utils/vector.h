#ifndef VECTOR_H
#define VECTOR_H

#include <stdarg.h>
#include <stddef.h>

// Function pointer types for destructor and constructor
typedef void (*DestructorFunc)(void *);
typedef void (*ConstructorFunc)(void *, va_list args);
typedef void (*CopyConstructorFunc)(void *, void *);

// Vector structure
typedef struct {
  void **items;
  size_t size;
  size_t capacity;
  size_t itemSize; // Size of the type stored in the vector
} Vector;

// Function declarations
void vectorInit(Vector *vector, size_t initialCapacity, size_t typeSize,
                ConstructorFunc constructor, ...);
void vectorCopyConstructor(Vector *dest, const Vector *src,
                           CopyConstructorFunc constructor);
int vectorAppend(Vector *vector, void *item, ConstructorFunc constructor, ...);
void *vectorGet(Vector *vector, size_t index);
int vectorRemove(Vector *vector, size_t index, DestructorFunc destructor);
void vectorClear(Vector *vector, DestructorFunc destructor);
void vectorFree(Vector *vector, DestructorFunc destructor);

#endif // VECTOR_H
