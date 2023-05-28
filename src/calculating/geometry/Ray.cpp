#include "calculating/geometry/Ray.h"
#include "calculating/Lens.h"

Ray::Ray(double x1, double y1, double x2, double y2) {
    if (x1 < x2) {
        _min = Point(x1, y1);
        _max = Point(x2, y2);
    }
    else {
        _min = Point(x2, y2);
        _max = Point(x1, y1);
    }
    if (x1 == x2) {
        // TODO: maybe exception?
    }
    else if (y1 == y2) {
        this->_k = 0;
        parallel = true;
    }
    else
        this->_k = (y2 - y1) / (x2 - x1);
    this->_b = _k*x1+y1;
}

Ray::Ray(Point p1, Point p2) : Ray(p1.x(), p1.y(), p2.x(), p2.y()) { }

Ray Ray::refractionRay(Lens l) {
    double F = l.getFocusLength();
    if(isParallel()) {
        // parallel
        return focusRefraction(l);
    }
    else if(_max.x() == 0 && _b == 0) {
        // optical center
        return straightRefraction();
    }

    if (l.isConverging()) {
        if (pointIsOnRay(Point(-F, 0))) {
            // focus
            return parallelRefraction(l);
        }
    } else if (pointIsOnRay(Point(F, 0))) {
        return parallelRefraction(l);
    }
    // TODO: exception or calculate uncommon rays
    return {0, 0, 0, 0};
}

void Ray::setMin(const Point& minP) {
    if(pointIsOnRay(minP)) {
        this->_min = minP;
    }
    else {
        // TODO: exception
    }
}

void Ray::setMax(const Point& maxP) {
    if(pointIsOnRay(maxP)) {
        this->_max = maxP;
    }
    else {
        // TODO: exception
    }
}

Ray Ray::parallelRefraction(const Lens &lens) {
    return {_max.x(), _max.y(), lens.getFocusLength(), _max.y()};
}

Ray Ray::focusRefraction(const Lens &lens) {
    double F = lens.getFocusLength();
    if (lens.isConverging()) {
        return {_max, Point(F, 0)};
    }
    else {
        Ray res = Ray(Point(-F, 0), _max);
        // TODO: draw grey ray to show building ???

        res.setMin(_max);
        res.setMax(Point(F, res.getK() * F + res.getB()));
        return res;
    }
}

Ray Ray::straightRefraction() {
    Ray res = *this;
    res.setMin(_max);
    res.setMax(Point(-_min.x(), -_min.y()));
    return res;
}

bool Ray::pointIsOnRay(const Point &point) const {
    return getK() * point.x() + getB() == point.y();
}

Point Ray::intersect(const Ray& ray) const {
    /* y = k1*x + b1
     * y = k2*x + b2
     *
     *  x * (k1 - k2) = b2 - b1
     *  x = (b2 - b1) / (k1 - k2)
     */
    double x = (ray.getB() - getB()) / (getK() - ray.getK());
    double y = getK() * x + getB();
    return Point(x, y);
}