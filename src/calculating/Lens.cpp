#include "calculating/Lens.h"
#include "calculating/Ray.h"
#include <cmath>

Lens::Lens(double opt_pow) : _opt_pow(opt_pow) { }

double Lens::getFocus() const { return std::abs(1 / _opt_pow); }
bool Lens::isConv() const { return _opt_pow > 0; }

double Lens::getDistanceToImage(double d) const {
    return d / (d * _opt_pow - 1);
}

double Lens::getDistanceToObject(double f) const {
    return f / (f * _opt_pow - 1);
}

double Lens::getImgHeight(double H, double f, double d) {
    return f * H / d;
}

double Lens::getObjHeight(double H, double f, double d) {
    return H * d / f;
}

double Lens::getOptPow(double d, double f) {
    return 1/d + 1/f;
}

Point Lens::getImage(const Point &point) {
    ;
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