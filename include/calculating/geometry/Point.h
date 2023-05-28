#ifndef OPTOCUS_POINT_H
#define OPTOCUS_POINT_H

class Point {
private:
    double _x;
    double _y;

public:
    explicit Point(double x = 0, double y = 0) : _x(x), _y(y) { }
    Point(const Point&) = default;
    ~Point() = default;
    Point& operator=(const Point&) = default;
    double& x() { return _x; }
    double& y() { return _y; }
    const double& x() const { return _x; }
    const double& y() const { return _y; }
};

#endif