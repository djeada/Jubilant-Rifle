typedef enum {
    BULLET,
    HUMANOID
} Type;

typedef struct {
    int size;
    int maxSize;
    Type type;
    void** data;
} Vector;

void append(Vector* vector, void* newElement);
