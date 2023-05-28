#ifndef OPTOCUS_RAY_H
#define OPTOCUS_RAY_H

#include "Point.h"

class Lens;
class Ray {
    double _k, _b;
    Point min, max;
    bool parallel = false;

public:
    Ray(double x1, double y1, double x2, double y2);
    Ray(Point p1, Point p2);
    Ray refractionRay(Lens l);
    bool isParallel() const { return parallel; }

    double getK() const { return _k; }
    double getB() const { return _b; }

    bool pointIsOnRay(const Point &point) const;

    void setMin(const Point &minP);
    void setMax(const Point &maxP);

    Ray parallelRefraction(const Lens &lens);
    Ray focusRefraction(const Lens &lens);
    Ray straightRefraction();

    Point intersect(const Ray& ray) const;
};

#endif