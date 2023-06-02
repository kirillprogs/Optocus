#include <cmath>
#include "Segment.h"

/* (y - y1) * (x2 - x1) = (y2 - y1) * (x - x1)
 * y * (x2 - x1) + x * (y1 - y2) = y1 * (x2 - x1) + x1 * (y1 - y2) */
double Segment::getCanonicalA() const { return startY() - endY(); }
double Segment::getCanonicalB() const { return endX() - startX(); }
double Segment::getCanonicalC() const {
    return startX() * getCanonicalA() + startY() * getCanonicalB();
}

Point Segment::projection(Point &point) const {
    double a = getCanonicalA();
    double b = getCanonicalB();
    double c = getCanonicalC();
    return Point((b * (b * point.x() - a * point.y()) - a * c) / (a * a + b * b),
                 (a * (-b * point.x() + a * point.y() - b * c)) / (a * a + b * b));
}

Point Segment::intersection(const Segment &o) const
{
    /*
     * a1 * x + b1 * y = c1
     * a2 * x + b2 * y = c2
     *
     * a1 * a2 * x + b1 * a2 * y = c1 * a2
     * a2 * a1 * x + b2 * a1 * y = c2 * a1
     *
     * y = (a1 * c2 - a2 * c1) / (a1 * b2 - a2 * b1)
     * x = (b1 * c2 - b2 * c1) / (b1 * a2 - b2 * a1)
     */
    double a1 = getCanonicalA();
    double b1 = getCanonicalB();
    double c1 = getCanonicalC();
    double a2 = o.getCanonicalA();
    double b2 = o.getCanonicalB();
    double c2 = o.getCanonicalC();
    return Point((b1 * c2 - b2 * c1) / (b1 * a2 - b2 * a1),
                 (a1 * c2 - a2 * c1) / (a1 * b2 - a2 * b1));
}

double Segment::intersectsVertical(double x) const {
    double a = getCanonicalA();
    double b = getCanonicalB();
    double c = getCanonicalC();
    if (b == 0)
        return INFINITY;
    return (c - a * x) / b;
}