#include "OpticalController.h"

OpticalController *OpticalController::_instance;

OpticalController *OpticalController::instance() {
    if (!OpticalController::_instance)
        OpticalController::_instance = new OpticalController();
    return OpticalController::_instance;
}

const vector<Lens> &OpticalController::get_lenses() const { return optics.lenses(); }
const list<Segment> &OpticalController::get_rays() const { return optics.rayImages(); }

list<Segment> OpticalController::get_image_rays() const {
    list<Segment> list;
    for (const Image<Point> &image : optics.objectImages())
        for (const Segment &segment : image.getRays())
            list.push_back(segment);
    return list;
}

bool OpticalController::has_object() const { return optics.hasObject(); }
Segment OpticalController::get_object() const {
    if (optics.hasObject()) {
        Point end(*optics.object());
        return Segment(end, Point(end.x(), 0));
    }
    return Segment(Point(), Point());
}

list<Segment> OpticalController::get_images() const {
    if (optics.hasObject()) {
        list<Segment> list;
        for (const Image<Point> &image : optics.objectImages()) {
            Segment segment(image.getImage(), Point(image.getImage().x(), 0));
            list.push_back(segment);
        }
    }
    return list<Segment>();
}

void OpticalController::clear_all() { optics.clear(); }
void OpticalController::clear_geometry() { optics.clear_geometry(); }

void OpticalController::add_lens(const Lens &lens) {
    optics.addLens(lens);
    optics.evaluate();
}

void OpticalController::set_object(double x, double y) {
    optics.set_object(Point(x, y));
    optics.evaluate();
}