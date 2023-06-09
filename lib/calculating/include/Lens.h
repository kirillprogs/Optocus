#ifndef OPTOCUS_LENS_H
#define OPTOCUS_LENS_H

#include <list>
#include <cmath>

#include "Image.h"
#include "Point.h"
#include "Segment.h"

using std::list;

class Lens {
    double _opt_pow;
    double _height;
    double _x;

public:
    Lens(double opt_pow, double x, double height = 1.);
    ~Lens() = default;

    double& x() { return _x; }
    const double& x() const { return _x; }

    double& optPow() { return _opt_pow; }
    const double& optPow() const  { return _opt_pow; }

    double& height() { return _height; }
    const double& height() const { return _height; }

    double getFocus() const;
    double getFocusLength() const;
    void setFocusLength(double length);

    // converging or diffusing
    bool isConverging() const;

    // get distance to image from distance to _object
    double getDistanceToImage(double d) const;
    // get distance to _object from distance to image
    double getDistanceToObject(double f) const;

    static double getImgHeight(double h, double f, double d);
    static double getObjHeight(double imgH, double f, double d);

    // calculate optical power
    static double getOptPow(double d, double f);

    Point getImagePoint(const Point &point);

private:
    Image<Point> convergingReal(const Point &point);
    Image<Point> convergingImag(const Point &point);
    Image<Point> convergingAft(const Point &point);
    Image<Point> divergingFore(const Point &point);
    Image<Point> divergingReal(const Point &point);
    Image<Point> divergingImag(const Point &point);
    Image<Point> convergingNoImage(const Point &point);
    Image<Point> divergingNoImage(const Point &point);

public:
    Image<Point> getImage(const Point &point);
    Image<Segment> getImage(const Segment &segment);
};

#endif