#include "utils/point.h"
#include <math.h>

void pointConstructor(Point *point, float x, float y) {
  if (point) {
    point->x = x;
    point->y = y;
  }
}

void pointCopyConstructor(Point *destination, const Point *source) {
  if (destination && source) {
    *destination = *source;
  }
}

Point makePoint(float x, float y) {
  Point p;
  p.x = x;
  p.y = y;
  return p;
}

Point pointAdd(const Point *p1, const Point *p2) {
  Point result;
  if (p1 && p2) {
    result.x = p1->x + p2->x;
    result.y = p1->y + p2->y;
  }
  return result;
}

Point pointSubtract(const Point *p1, const Point *p2) {
  Point result;
  if (p1 && p2) {
    result.x = p1->x - p2->x;
    result.y = p1->y - p2->y;
  }
  return result;
}

float pointDistance(const Point *p1, const Point *p2) {
  if (p1 && p2) {
    float dx = p1->x - p2->x;
    float dy = p1->y - p2->y;
    return sqrt(dx * dx + dy * dy);
  }
  return 0.0f;
}
