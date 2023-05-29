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
    Point image(getImagePoint(point));
    Image<Point> imageObj(point, image);
    Point pointProjection(x(), point.y());
    Point imageProjection(x(), image.y());
    Point foreFocus(x() - getFocusLength());
    Point aftFocus(x() + getFocusLength());
    Point opticalCentre(x());
    imageObj.addRay(Segment(point, pointProjection));
    imageObj.addRay(Segment(point, foreFocus));
    imageObj.addRay(Segment(point, opticalCentre));
    imageObj.addRay(Segment(foreFocus, imageProjection));
    imageObj.addRay(Segment(pointProjection, aftFocus));
    imageObj.addRay(Segment(aftFocus, image));
    imageObj.addRay(Segment(opticalCentre, image));
    imageObj.addRay(Segment(imageProjection, image));
    return imageObj;
}

Image<Point> Lens::convergingImag(const Point &point) {
    Point image(getImagePoint(point));
    Image<Point> imageObj(point, image);
    Point pointProjection(x(), point.y());
    Point imageProjection(x(), image.y());
    Point foreFocus(x() - getFocusLength());
    Point opticalCentre(x());
    imageObj.addRay(Segment(point, pointProjection));
    imageObj.addRay(Segment(point, imageProjection));
    imageObj.addRay(Segment(point, opticalCentre));
    imageObj.addRay(Segment(image, point), false);
    imageObj.addRay(Segment(image, imageProjection), false);
    imageObj.addRay(Segment(image, pointProjection), false);
    imageObj.addRay(Segment(foreFocus, point));
    return imageObj;
}

Image<Point> Lens::convergingAft(const Point &point) {
    Point image(getImagePoint(point));
    Image<Point> imageObj(point, image);
    Point pointProjection(x(), point.y());
    Point imageProjection(x(), image.y());
    Point foreFocus(x() - getFocusLength());
    Point aftFocus(x() + getFocusLength());
    Point opticalCentre(x());
    imageObj.addRay(Segment(opticalCentre, image));
    imageObj.addRay(Segment(image, point), false);
    imageObj.addRay(Segment(pointProjection, point), false);
    imageObj.addRay(Segment(pointProjection, image));
    imageObj.addRay(Segment(image, aftFocus));
    imageObj.addRay(Segment(image, aftFocus), false);
    imageObj.addRay(Segment(foreFocus, imageProjection), false);
    imageObj.addRay(Segment(imageProjection, point), false);
    imageObj.addRay(Segment(imageProjection, image));
    return imageObj;
}

Image<Point> Lens::divergingFore(const Point &point) {
    Point image(getImagePoint(point));
    Image<Point> imageObj(point, image);
    Point pointProjection(x(), point.y());
    Point imageProjection(x(), image.y());
    Point foreFocus(x() - getFocusLength());
    Point aftFocus(x() + getFocusLength());
    Point opticalCentre(x());
    imageObj.addRay(Segment(point, pointProjection));
    imageObj.addRay(Segment(point, image));
    imageObj.addRay(Segment(image, opticalCentre));
    imageObj.addRay(Segment(point, imageProjection));
    imageObj.addRay(Segment(foreFocus, image), false);
    imageObj.addRay(Segment(image, pointProjection), false);
    imageObj.addRay(Segment(image, imageProjection), false);
    imageObj.addRay(Segment(imageProjection, aftFocus), false);
    return imageObj;
}

Image<Point> Lens::divergingReal(const Point &point) {
    Point image(getImagePoint(point));
    Image<Point> imageObj(point, image);
    Point pointProjection(x(), point.y());
    Point imageProjection(x(), image.y());
    Point foreFocus(x() - getFocusLength());
    Point aftFocus(x() + getFocusLength());
    Point opticalCentre(x());
    imageObj.addRay(Segment(imageProjection, image));
    imageObj.addRay(Segment(pointProjection, image));
    imageObj.addRay(Segment(opticalCentre, image));
    imageObj.addRay(Segment(opticalCentre, point), false);
    imageObj.addRay(Segment(pointProjection, point), false);
    imageObj.addRay(Segment(imageProjection, point), false);
    imageObj.addRay(Segment(point, aftFocus), false);
    imageObj.addRay(Segment(foreFocus, pointProjection), false);
    return imageObj;
}

Image<Point> Lens::divergingImag(const Point &point) {
    Point image(getImagePoint(point));
    Image<Point> imageObj(point, image);
    Point pointProjection(x(), point.y());
    Point imageProjection(x(), image.y());
    Point foreFocus(x() - getFocusLength());
    Point aftFocus(x() + getFocusLength());
    Point opticalCentre(x());
    imageObj.addRay(Segment(pointProjection, point), false);
    imageObj.addRay(Segment(image, foreFocus), false);
    imageObj.addRay(Segment(foreFocus, pointProjection), false);
    imageObj.addRay(Segment(image, imageProjection), false);
    imageObj.addRay(Segment(imageProjection, aftFocus), false);
    imageObj.addRay(Segment(aftFocus, point), false);
    imageObj.addRay(Segment(image, opticalCentre), false);
    imageObj.addRay(Segment(opticalCentre, point), false);
    return imageObj;
}

Image<Point> Lens::getImage(const Point &point) {
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