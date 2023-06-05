#include "OpticalController.h"

OpticalController *OpticalController::_instance;

OpticalController *OpticalController::instance() {
    if (!OpticalController::_instance)
        OpticalController::_instance = new OpticalController();
    return OpticalController::_instance;
}

const vector<Lens> &OpticalController::get_lenses() const { return _optics.lenses(); }
const list<Segment> &OpticalController::get_rays() const { return _optics.rayTrace(); }

list<Segment> OpticalController::get_image_rays() const {
    list<Segment> list;
    for (const Image<Point> &image : _optics.objectImages())
        for (const Segment &segment : image.getRays())
            list.push_back(segment);
    return list;
}

bool OpticalController::has_object() const { return _optics.hasObject(); }
Segment OpticalController::get_object() const {
    if (_optics.hasObject()) {
        Point end(*_optics.object());
        return Segment(end, Point(end.x(), 0));
    }
    return Segment(Point(), Point());
}

list<Segment> OpticalController::get_images() const {
    list<Segment> list;
    if (_optics.hasObject()) {
        for (const Image<Point> &image : _optics.objectImages()) {
            Segment segment(image.getImage(), Point(image.getImage().x(), 0));
            list.push_back(segment);
        }
    }
    return list;
}

void OpticalController::clear_all() { _optics.clear(); }
void OpticalController::clear_geometry() { _optics.clear_geometry(); }

void OpticalController::add_lens(const Lens &lens) {
    _optics.addLens(lens);
    _optics.evaluate();
}

void OpticalController::add_ray(const Segment &ray) {
    if (ray.startX() < ray.endX())
        _optics.addRay(ray);
    else
        _optics.addRay(Segment(ray.end(), ray.start()));
    _optics.evaluate();
}

void OpticalController::set_object(double x, double y) {
    _optics.set_object(Point(x, y));
    _optics.evaluate();
}

double OpticalController::convert_to_meter(double input, const QString& measure) {
    if (measure == "см") {
        return 0.01 * input;
    }
    else if (measure == "мм") {
        return 0.001 * input;
    }
    else if (measure == "км") {
        return 1000 * input;
    }
    return input;
}

bool OpticalController::lens_on_x_exists(double x) const {
    for(Lens lens : get_lenses()) {
        if(lens.x() == x) {
            return true;
        }
    }
    return false;
}


Point OpticalController::screenPoint(Point a) { return Point(screenX(a.x()), screenY(a.y())); }
Segment OpticalController::screenSegment(Segment a) { return Segment(screenPoint(a.start()), screenPoint(a.end())); }