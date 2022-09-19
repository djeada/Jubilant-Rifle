
typedef enum { BULLET, HUMANOID } Type;

typedef struct {
  int size;
  int maxSize;
  Type type;
  void **data;
} Vector;

void vectorConstructor(Vector *vector, int maxSize, Type type);
void append(Vector *vector, void *newElement);
void removeFromVector(Vector *vector, int i);
void clear(Vector *vector);
