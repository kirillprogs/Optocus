#include "OpticalSystem.h"

void OpticalSystem::set_object(const Point &point) {
    if (hasObject())
        remove_object();
    _object = new Point(point);
}

void OpticalSystem::remove_object() {
    delete _object;
    _object = 0;
}

list<Image<Point>> OpticalSystem::evaluate_object(const Point &point) {
    Point current_object(point);
    list<Image<Point>> image_list;
    for (int lens_id(0); lens_id < _lenses.size(); ++lens_id) {
        if (_lenses[lens_id].x() <= _object->x())
            continue;
        image_list.push_back(_lenses[lens_id].getImage(current_object));
        current_object = image_list.back().getImage();
    }
    return image_list;
}

list<Image<Segment>> OpticalSystem::evaluate_ray(const Segment &ray) {
    Segment current_ray(ray);
    list<Image<Segment>> image_list;
    for (int lens_id(0); lens_id < _lenses.size(); ++lens_id) {
        if (_lenses[lens_id].x() <= ray.startX())
            continue;
        image_list.push_back(_lenses[lens_id].getImage(current_ray));
        current_ray = image_list.back().getImage();
    }
    return image_list;
}

list<Segment> OpticalSystem::evaluate_ray_trace(const Segment &ray) {
    list<Segment> result;
    result.push_back(ray);
    for (int lens_id(0); lens_id < _lenses.size(); ++lens_id) {
        if (_lenses[lens_id].x() <= ray.startX())
            continue;
        Segment current_ray = result.back();
        result.pop_back();
        Image<Segment> image = _lenses[lens_id].getImage(current_ray);
        result.push_back(Segment(current_ray.start(), image.getObject().end()));
        result.push_back(image.getImage());
    }
    return result;
}

void OpticalSystem::evaluate() {
    _obj_images.clear();
    _ray_images.clear();
    _ray_trace.clear();
    sort(_lenses.begin(), _lenses.end(),
         [](Lens a, Lens b){ return a.x() < b.x(); });
    if (hasObject())
        _obj_images = evaluate_object(*object());
    for (Segment ray : _rays)
        for (Segment ray_trace : evaluate_ray_trace(ray))
            _ray_trace.push_back(ray_trace);
}

void OpticalSystem::clear_geometry() {
    _obj_images.clear();
    _ray_trace.clear();
    _rays.clear();
    remove_object();
}

void OpticalSystem::clear() {
    clear_geometry();
    _lenses.clear();
}