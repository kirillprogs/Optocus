#ifndef OPTOCUS_LENS_H
#define OPTOCUS_LENS_H

#include "Object.h"

class Lens {
    double _opt_pow;
    double _height;

public:
    explicit Lens(double opt_pow);
    ~Lens() = default;
    double& optPow() { return _opt_pow; }
    const double& optPow() const  { return _opt_pow; }
    double& height() { return _height; }
    const double& height() const { return _height; }

    double getFocus() const;
    // converging or diffusing
    bool isConv() const;

    // get distance to image from distance to object
    double getDistanceToImage(double d) const;
    // get distance to object from distance to image
    double getDistanceToObject(double f) const;

    static double getImgHeight(double H, double f, double d);
    static double getObjHeight(double imgH, double f, double d);

    // calculate optical power
    static double getOptPow(double d, double f);

    Point getImage(const Point& point);
    Object getImage(const Object& obj);
};

#endif