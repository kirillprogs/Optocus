#ifndef OPTOCUS_SEGMENT_H
#define OPTOCUS_SEGMENT_H

#include "Point.h"

class Segment {
    Point _a, _b;
    bool _includeL, _includeR;

public:
    Segment(const Point &a, const Point &b, bool includeL = true, bool includeR = true)
            : _a(a), _b(b), _includeL(includeL), _includeR(includeR) { }

    Segment(double x1, double y1, double x2, double y2,
            bool includeL = true, bool includeR = true)
            : _a(x1, y1), _b(x2, y2),
            _includeL(includeL), _includeR(includeR) { }

    Point &start() { return _a; }
    Point &end() { return _b; }

    const Point &start() const { return _a; }
    const Point &end() const { return _b; }

    double &startX() { return start().x(); }
    double &startY() { return start().y(); }
    double &endX() { return end().x(); }
    double &endY() { return end().y(); }

    const double &startX() const { return start().x(); }
    const double &startY() const { return start().y(); }
    const double &endX() const { return end().x(); }
    const double &endY() const { return end().y(); }

    bool &includeL() { return _includeL; }
    bool &includeR() { return _includeR; }
    const bool &includeL() const { return _includeL; }
    const bool &includeR() const { return _includeR; }

    double getCanonicalA() const;
    double getCanonicalB() const;
    double getCanonicalC() const;

    Point projection(Point &point) const;
    Point intersection(const Segment &o) const;
    double intersectsVertical(double x) const;
};

#endif