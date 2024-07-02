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

float pointDistanceX(const Point *p1, const Point *p2) {
  if (p1 && p2) {
    return fabs(p1->x - p2->x);
  }
  return 0.0f;
}

float pointDistanceY(const Point *p1, const Point *p2) {
  if (p1 && p2) {
    return fabs(p1->y - p2->y);
  }
  return 0.0f;
}

float pointDotProduct(const Point *p1, const Point *p2) {
  if (p1 && p2) {
    return (p1->x * p2->x + p1->y * p2->y);
  }
  return 0.0f;
}

float pointCrossProduct(const Point *p1, const Point *p2) {
  if (p1 && p2) {
    return (p1->x * p2->y - p1->y * p2->x);
  }
  return 0.0f;
}

int pointOrientation(const Point *p, const Point *q, const Point *r) {
  if (p && q && r) {
    float val = (q->y - p->y) * (r->x - q->x) - (q->x - p->x) * (r->y - q->y);
    if (val > 0) {
      return 1; // Counterclockwise
    } else if (val < 0) {
      return -1; // Clockwise
    } else {
      return 0; // Collinear
    }
  }
  return 0;
}

int isPointLeftOf(const Point *p1, const Point *p2) {
  if (p1 && p2) {
    return (p1->x < p2->x) ? 1 : 0;
  }
  return 0;
}

int isPointRightOf(const Point *p1, const Point *p2) {
  if (p1 && p2) {
    return (p1->x > p2->x) ? 1 : 0;
  }
  return 0;
}
