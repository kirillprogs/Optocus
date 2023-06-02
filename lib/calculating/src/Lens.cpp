#include "Lens.h"

Lens::Lens(double opt_pow, double x, double height)
        : _opt_pow(opt_pow), _x(x), _height(height) { }

double Lens::getFocus() const { return 1 / _opt_pow; }
double Lens::getFocusLength() const { return std::abs(1 / _opt_pow); }
void Lens::setFocusLength(double length) { optPow() = 1 / length; }

bool Lens::isConverging() const { return _opt_pow > 0; }

double Lens::getDistanceToImage(double d) const { return 1 / (optPow() - 1 / d); }
double Lens::getDistanceToObject(double f) const { return 1 / (optPow() - 1 / f); }

double Lens::getImgHeight(double h, double f, double d) { return -h * (f / d); }
double Lens::getObjHeight(double H, double f, double d) { return -H * (d / f); }

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
    Image<Point> imageObj(point, image, true);
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
    Image<Point> imageObj(point, image, false);
    Point pointProjection(x(), point.y());
    Point imageProjection(x(), image.y());
    Point foreFocus(x() - getFocusLength());
    Point opticalCentre(x());
    imageObj.addRay(Segment(point, pointProjection));
    imageObj.addRay(Segment(point, imageProjection));
    imageObj.addRay(Segment(point, opticalCentre));
    imageObj.addRay(Segment(image, point));
    imageObj.addRay(Segment(image, imageProjection));
    imageObj.addRay(Segment(image, pointProjection));
    imageObj.addRay(Segment(foreFocus, point));
    return imageObj;
}

Image<Point> Lens::convergingAft(const Point &point) {
    Point image(getImagePoint(point));
    Image<Point> imageObj(point, image, true);
    Point pointProjection(x(), point.y());
    Point imageProjection(x(), image.y());
    Point foreFocus(x() - getFocusLength());
    Point aftFocus(x() + getFocusLength());
    Point opticalCentre(x());
    imageObj.addRay(Segment(opticalCentre, image));
    imageObj.addRay(Segment(image, point));
    imageObj.addRay(Segment(pointProjection, point));
    imageObj.addRay(Segment(pointProjection, image));
    imageObj.addRay(Segment(image, aftFocus));
    imageObj.addRay(Segment(image, aftFocus));
    imageObj.addRay(Segment(foreFocus, imageProjection));
    imageObj.addRay(Segment(imageProjection, point));
    imageObj.addRay(Segment(imageProjection, image));
    return imageObj;
}

Image<Point> Lens::divergingFore(const Point &point) {
    Point image(getImagePoint(point));
    Image<Point> imageObj(point, image, false);
    Point pointProjection(x(), point.y());
    Point imageProjection(x(), image.y());
    Point foreFocus(x() - getFocusLength());
    Point aftFocus(x() + getFocusLength());
    Point opticalCentre(x());
    imageObj.addRay(Segment(point, pointProjection));
    imageObj.addRay(Segment(point, image));
    imageObj.addRay(Segment(image, opticalCentre));
    imageObj.addRay(Segment(point, imageProjection));
    imageObj.addRay(Segment(foreFocus, image));
    imageObj.addRay(Segment(image, pointProjection));
    imageObj.addRay(Segment(image, imageProjection));
    imageObj.addRay(Segment(imageProjection, aftFocus));
    return imageObj;
}

Image<Point> Lens::divergingReal(const Point &point) {
    Point image(getImagePoint(point));
    Image<Point> imageObj(point, image, true);
    Point pointProjection(x(), point.y());
    Point imageProjection(x(), image.y());
    Point foreFocus(x() - getFocusLength());
    Point aftFocus(x() + getFocusLength());
    Point opticalCentre(x());
    imageObj.addRay(Segment(imageProjection, image));
    imageObj.addRay(Segment(pointProjection, image));
    imageObj.addRay(Segment(opticalCentre, image));
    imageObj.addRay(Segment(opticalCentre, point));
    imageObj.addRay(Segment(pointProjection, point));
    imageObj.addRay(Segment(imageProjection, point));
    imageObj.addRay(Segment(point, aftFocus));
    imageObj.addRay(Segment(foreFocus, pointProjection));
    return imageObj;
}

Image<Point> Lens::divergingImag(const Point &point) {
    Point image(getImagePoint(point));
    Image<Point> imageObj(point, image, false);
    Point pointProjection(x(), point.y());
    Point imageProjection(x(), image.y());
    Point foreFocus(x() - getFocusLength());
    Point aftFocus(x() + getFocusLength());
    Point opticalCentre(x());
    imageObj.addRay(Segment(pointProjection, point));
    imageObj.addRay(Segment(image, foreFocus));
    imageObj.addRay(Segment(foreFocus, pointProjection));
    imageObj.addRay(Segment(image, imageProjection));
    imageObj.addRay(Segment(imageProjection, aftFocus));
    imageObj.addRay(Segment(aftFocus, point));
    imageObj.addRay(Segment(image, opticalCentre));
    imageObj.addRay(Segment(opticalCentre, point));
    return imageObj;
}

Image<Point> Lens::convergingNoImage(const Point &point) {
    return Image<Point>(Point(), Point());
}

Image<Point> Lens::divergingNoImage(const Point &point) {
    return Image<Point>(Point(), Point());
}

Image<Point> Lens::getImage(const Point &point) {
    if (isConverging()) {
        if (point.x() < x() - getFocusLength())
            return convergingReal(point);
        else if (point.x() == x() - getFocusLength())
            return convergingNoImage(point);
        else if (point.x() < x())
            return convergingImag(point);
        return convergingAft(point);
    } else {
        if (point.x() < x())
            return divergingFore(point);
        else if (point.x() < x() + getFocusLength())
            return divergingReal(point);
        else if (point.x() == x() + getFocusLength())
            return divergingNoImage(point);
        return divergingImag(point);
    }
}

Image<Segment> Lens::getImage(const Segment &ray) {
    Point crossLens(x(), ray.intersectsVertical(x()));
    Point crossForeFocal(x() - getFocusLength(),
                         ray.intersectsVertical(x() - getFocusLength()));
    Segment goodRay(crossForeFocal, crossLens);
    if (crossLens.y() == 0) {
        Point backFocal(x() + getFocusLength(), -goodRay.start().y());
        return Image<Segment>(goodRay, Segment(
                crossLens, backFocal, true, false));
    }
    /* Ray goes parallel to the optical line and should be retracted through focus */
    if (goodRay.getCanonicalA() == 0) {
        if (isConverging())
            return Image<Segment>(goodRay, Segment(crossLens,
                                                   Point(x() + getFocusLength()),
                                                   true, false));
        Point backFocal(x() + getFocusLength(), 2 * crossLens.y());
        return Image<Segment>(goodRay, Segment(crossLens, backFocal,
                                               true, false));
    }
    Point crossAxis(goodRay.getCanonicalC() / goodRay.getCanonicalA());
    /* Ray goes through focus and should be retracted parallel to the optical line */
    if (crossAxis.x() == x() - getFocus()) {
        Point backFocal(x() + getFocusLength(), crossLens.y());
        return Image<Segment>(goodRay, Segment(crossLens, backFocal, true, false));
    }
    /* Arbitrary rays */
    Point imagePoint = getImagePoint(crossAxis);
    Segment retractedSegment(imagePoint, crossLens);
    Point backFocal(x() + getFocusLength(),
                    retractedSegment.intersectsVertical(x() + getFocusLength()));
    return Image<Segment>(goodRay, Segment(crossLens, backFocal, true, false));
}