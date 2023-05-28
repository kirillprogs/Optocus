#include "calculating/Lens.h"
#include "calculating/geometry/Ray.h"

Lens::Lens(double opt_pow, double x, double height)
        : _opt_pow(opt_pow), _x(x), _height(height) { }

double Lens::getFocus() const { return 1 / _opt_pow; }
double Lens::getFocusLength() const { return std::abs(1 / _opt_pow); }
void Lens::setFocusLength(double length) { optPow() = 1 / length; }

bool Lens::isConverging() const { return _opt_pow > 0; }

double Lens::getDistanceToImage(double d) const { return 1 / (optPow() - 1 / d); }
double Lens::getDistanceToObject(double f) const { return 1 / (optPow() - 1 / f); }

double Lens::getImgHeight(double h, double f, double d) { return h * (f / d); }
double Lens::getObjHeight(double H, double f, double d) { return H * (d / f); }

double Lens::getOptPow(double d, double f) { return 1/d + 1/f; }

Point Lens::getImagePoint(const Point &point)
{
    double d = x() - point.x();
    if (d == getFocus()) {
        return Point(INFINITY, INFINITY); // NO IMAGE
    } else if (d != 0) {
        double f = getDistanceToImage(d);
        return Point(x() + f, getImgHeight(point.y(), f, d));
    }
    return point;
}

Image<Point> Lens::getImage(const Point &point)
{
    Image<Point> image(point, getImagePoint(point));
    image.addRay(Segment(point, Point(x(), point.y())));
    image.addRay(Segment(point, Point(x(), 0)));
    // TODO here just add all the rays
    return image;
}

Image<Segment> Lens::getImage(const Segment &segment) {
    Image<Point> image1 = getImage(segment.start());
    Image<Point> image2 = getImage(segment.end());
    Image<Segment> image(segment, Segment(image1.getImage(), image2.getImage()));
    for (Segment s : image1.getRealRays())
        image.addRay(s);
    for (Segment s : image2.getRealRays())
        image.addRay(s);
    for (Segment s : image1.getImagRays())
        image.addRay(s, false);
    for (Segment s : image2.getImagRays())
        image.addRay(s, false);
    // TODO what if segment intersects focus point?
    return image;
}

Image<Ray> Lens::getImage(const Ray &ray) {
    Point crossLens(x(), ray.getK() * x() + ray.getB());
    Point focusPoint(x() + getFocusLength()); // point where ray intersect back focal plane
    Ray goodRay = Ray(Segment(
                            ray.min(),
                            ray.max()
                            ).intersectsVertical(x() - getFocusLength()),
                      crossLens);
    if (crossLens.y() == 0)
        return Image<Ray>(goodRay, Ray(crossLens,
                                       Point(x() + getFocusLength(), -goodRay.min().y())));
    /* Ray goes parallel to the optical line and should be retracted through focus */
    if (goodRay.getK() == 0) {
        if (!isConverging())
            focusPoint.y() = 2 * crossLens.y();
        Image<Ray> image(goodRay, Ray(crossLens, focusPoint));
        if (!isConverging())
            image.addRay(Segment(Point(x() + getFocus()), crossLens), false);
        return image;
    }
    Point crossAxis(-goodRay.getB() / goodRay.getK());
    /* Ray goes through focus and should be retracted parallel to the optical line */
    if (crossAxis.x() == x() - getFocus()) {
        focusPoint.y() = crossLens.y();
        Image<Ray> image(goodRay, Ray(crossLens, focusPoint));
        if (!isConverging())
            image.addRay(Segment(crossLens, Point(x() - getFocus())), false);
        return image;
    }
    // TODO arbitrary rays
}

Object Lens::getImage(const Object& obj) {
    Point a = obj.getA();
    Point b = obj.getB();
    Ray firstA = Ray(Point(a.x(), a.y()), Point(0, a.y()));
    Ray secondA = Ray(Point(a.x(), a.y()), Point(0, 0));

    Ray refractA1 = firstA.refractionRay(*this);
    Ray refractA2 = secondA.refractionRay(*this);

    Point a_img = refractA1.intersect(refractA2);

    Ray firstB = Ray(Point(b.x(), b.y()), Point(0, b.y()));
    Ray secondB = Ray(Point(b.x(), b.y()), Point(0, 0));

    Ray refractB1 = firstB.refractionRay(*this);
    Ray refractB2 = secondB.refractionRay(*this);

    Point b_img = refractB1.intersect(refractB2);

    return {a_img, b_img, true};
}