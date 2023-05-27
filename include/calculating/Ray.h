#ifndef OPTOCUS_RAY_H
#define OPTOCUS_RAY_H

#include "Lens.h"
#include "Point.h"

class Ray {
private:
    double _k, _b;
    // використаємо мін та макс як межі малювання променя. а також для координат променів, які від них заломляться
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

    void setMin(const Point& minP) {
        if(pointIsOnRay(minP)) {
            this->min = minP;
        }
        else {
            // TODO: exception
        }
    }

    void setMax(const Point& maxP) {
        if(pointIsOnRay(maxP)) {
            this->max = maxP;
        }
        else {
            // TODO: exception
        }
    }

    Ray refract(const Lens& lens) const;
    Ray parallelRefraction(const Lens &lens);
    Ray focusRefraction(const Lens &lens);
    Ray straightRefraction();

    Point intersect(const Ray& ray) const;
};

#endif