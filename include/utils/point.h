#ifndef POINT_H
#define POINT_H

typedef struct {
  float x;
  float y;
} Point;

// Constructor to initialize a Point.
void pointConstructor(Point *point, float x, float y);

// Copy constructor to copy one Point to another.
void pointCopyConstructor(Point *destination, const Point *source);
Point makePoint(float x, float y);
// Function to add two points.
Point pointAdd(const Point *p1, const Point *p2);

// Function to subtract two points.
Point pointSubtract(const Point *p1, const Point *p2);

// Function to calculate the distance between two points.
float pointDistance(const Point *p1, const Point *p2);

#endif // POINT_H
