#ifndef POINT_H
#define POINT_H

typedef struct {
    float x, y;
} Point;

void pointConstructor(Point *point, float x, float y);
void pointCopyConstructor(Point *destination, const Point *source);
Point makePoint(float x, float y);
Point pointAdd(const Point *p1, const Point *p2);
Point pointSubtract(const Point *p1, const Point *p2);
float pointDistance(const Point *p1, const Point *p2);
float pointDistanceX(const Point *p1, const Point *p2);
float pointDistanceY(const Point *p1, const Point *p2);
float pointDotProduct(const Point *p1, const Point *p2);
float pointCrossProduct(const Point *p1, const Point *p2);
int pointOrientation(const Point *p, const Point *q, const Point *r);
int isPointLeftOf(const Point *p1, const Point *p2);
int isPointRightOf(const Point *p1, const Point *p2);

#endif // POINT_H
