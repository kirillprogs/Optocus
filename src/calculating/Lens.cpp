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

Image<Point> Lens::convergingReal(const Point &point) {
    return Image<Point>();
}

Image<Point> Lens::convergingImag(const Point &point) {
    return Image<Point>();
}

Image<Point> Lens::convergingAft(const Point &point) {
    return Image<Point>();
}

Image<Point> Lens::divergingFore(const Point &point) {
    return Image<Point>();
}

Image<Point> Lens::divergingReal(const Point &point) {
    return Image<Point>();
}

Image<Point> Lens::divergingImag(const Point &point) {
    return Image<Point>();
}

Image<Point> Lens::getImage(const Point &point) {
    Point imagePoint = getImagePoint(point);
    if (isConverging()) {
        if (point.x() < x() - getFocusLength())
            return convergingReal(point);
        else if (point.x() < x())
            return convergingImag(point);
        return convergingAft(point);
    } else {
        if (point.x() < x())
            return divergingFore(point);
        else if (point.x() < x() + getFocusLength())
            return divergingReal(point);
        return divergingImag(point);
    }
}

Image<Segment> Lens::getImage(const Segment &ray) {
    Point crossLens(x(), ray.intersectsVertical(x()));
    Point focusPoint(x() + getFocusLength()); // point where ray intersect back focal plane
    Segment goodRay(Point(
                            x() - getFocusLength(),
                            Segment(ray.start(), ray.end())
                                .intersectsVertical(x() - getFocusLength())
                            ),
                      crossLens);
    if (crossLens.y() == 0)
        return Image<Segment>(goodRay,Segment(
                                        crossLens,
                                        Point(
                                            x() + getFocusLength(),
                                            -goodRay.start().y()
                                        ), true,false));
    /* Ray goes parallel to the optical line and should be retracted through focus */
    if (goodRay.getCanonicalA() == 0) {
        if (!isConverging())
            focusPoint.y() = 2 * crossLens.y();
        Image<Segment> image(goodRay, Segment(crossLens, focusPoint, true, false));
        if (!isConverging())
            image.addRay(Segment(Point(x() + getFocus()), crossLens), false);
        return image;
    }
    Point crossAxis(goodRay.getCanonicalC() / goodRay.getCanonicalA());
    /* Ray goes through focus and should be retracted parallel to the optical line */
    if (crossAxis.x() == x() - getFocus()) {
        focusPoint.y() = crossLens.y();
        Image<Segment> image(goodRay, Segment(crossLens, focusPoint));
        if (!isConverging())
            image.addRay(Segment(crossLens, Point(x() - getFocus())), false);
        return image;
    }

    Point imagePoint = getImagePoint(crossAxis);
    Segment retractedSegment(imagePoint, crossLens);
    if (isConverging()) {
        if (imagePoint.x() >= x()) {
            Segment imageRay(crossLens, imagePoint, true, false);
            Image<Segment> image(goodRay, Segment(crossLens, imagePoint));
        } else {

        }
    } else {
        /* if () {

        } else {

        } */
    }
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