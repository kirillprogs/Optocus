#ifndef OPTOCUS_SEGMENT_H
#define OPTOCUS_SEGMENT_H

#include "Point.h"

class Segment {
    Point _a, _b;

public:
    Segment(const Point &a, const Point &b)
            : _a(a), _b(b) { }
    Segment(double x1, double y1, double x2, double y2)
            : _a(x1, y1), _b(x2, y2) { }

    Point &start() { return _a; }
    Point &end() { return _b; }

    const Point &start() const { return _a; }
    const Point &end() const { return _b; }
};

#endif