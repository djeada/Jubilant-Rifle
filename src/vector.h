typedef enum {
    BULLET,
    HUMANOID
} Type;

typedef struct {
    int size;
    int maxSize;
    Type type;
} Info;

typedef struct {
    int size;
    int maxSize;
    Type type;
    void** data;
} Vector;

//void append(Info *info, void** vector, void* data);

void append(Vector* vector, void* newElement);
