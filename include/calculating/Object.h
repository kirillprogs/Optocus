#ifndef OPTOCUS_OBJECT_H
#define OPTOCUS_OBJECT_H

#include "Point.h"

class Object {
private:
    Point _a, _b;
    bool _isImage;

public:
    Object(const Point& a, const Point& b, bool isImage = false);
    Object(double x1, double y1, double x2, double y2, bool isImage = false);

    Point getA() const { return _a; }
    Point getB() const { return _b; }

    bool isImage() const { return _isImage; }
};

#endif