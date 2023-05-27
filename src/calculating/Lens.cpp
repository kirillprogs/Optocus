#include "calculating/Lens.h"

Lens::Lens(double opt_pow, double x, double height)
        : _opt_pow(opt_pow), _x(x), _height(height) { }

double Lens::getFocus() const { return 1 / _opt_pow; }
double Lens::getFocusLength() const { return std::abs(1 / _opt_pow); }

bool Lens::isConv() const { return _opt_pow > 0; }

double Lens::getDistanceToImage(double d) const { return 1 / (optPow() - 1 / d); }
double Lens::getDistanceToObject(double f) const { return 1 / (optPow() - 1 / f); }

double Lens::getImgHeight(double h, double f, double d) { return h * (f / d); }
double Lens::getObjHeight(double H, double f, double d) { return H * (d / f); }

double Lens::getOptPow(double d, double f) { return 1/d + 1/f; }

Point Lens::getImage(const Point &point)
{
    double d = x() - point.x();
    if (d == getFocus()) {
        // NO IMAGE
        return Point(INFINITY, INFINITY);
    } else if (d != 0) {
        double f = getDistanceToImage(d);
        return Point(x() + f, getImgHeight(point.y(), f, d));
    }
    return point;
}

Image Lens::getImageRays(const Point &point)
{
    Image image(point, getImage(point));
    image.addRay(Segment(point, Point(x(), point.y())));
    image.addRay(Segment(point, Point(x(), 0)));
}

Object Lens::getImage(const Object& obj) {
    Point a = obj.getA();
    Point b = obj.getB();
    Ray firstA = Ray(Point(a.x(), a.y()), Point(0, a.y()));
    Ray secondA = Ray(Point(a.x(), a.y()), Point(0, 0));

    Ray refractA1 = firstA.refractionRay(*this);
    Ray refractA2 = secondA.refractionRay(*this);

    // код можна було б спростити, але не знаю, як це в майбутньому буде малюватися, можливо потрібно буде поділити на класи

    Point a_img = refractA1.intersect(refractA2);

    Ray firstB = Ray(Point(b.x(), b.y()), Point(0, b.y()));
    Ray secondB = Ray(Point(b.x(), b.y()), Point(0, 0));

    Ray refractB1 = firstB.refractionRay(*this);
    Ray refractB2 = secondB.refractionRay(*this);

    Point b_img = refractB1.intersect(refractB2);

    return {a_img, b_img, true};
}