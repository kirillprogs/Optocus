#include "calculating/Object.h"

Object::Object(const Point& a, const Point& b, bool isImage) : _a(a), _b(b), _isImage(isImage) { }
Object::Object(double x1, double y1, double x2, double y2,bool isImage)
        : _a(x1, y1), _b(x2, y2), _isImage(isImage) { }