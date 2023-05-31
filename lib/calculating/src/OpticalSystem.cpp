#include "OpticalSystem.h"

list<Image<Point>> OpticalSystem::evaluate_object() {
   if (!hasObject()) return list<Image<Point>>();
    /* Sort the array of lenses in order of their abscissa */
    Point current_object(*_object);
    list<Image<Point>> image_list;
    for (int lens_id(0); lens_id < _lenses.size(); ++lens_id) {
        // Find the index of first lens in front of the object
        if (_lenses[lens_id].x() <= _object->x())
            continue;
        // push all the images in the list
        image_list.push_back(_lenses[lens_id].getImage(current_object));
        current_object = image_list.back().getImage();
    }
    return image_list;
}

list<Segment> OpticalSystem::evaluate_rays(const Segment &ray) {
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
    sort(_lenses.begin(), _lenses.end(),
         [](Lens a, Lens b){ return a.x() < b.x(); });
    _images = evaluate_object();
    for (Segment ray : _rays)
        for (Segment ray_trace : evaluate_rays(ray))
            _ray_images.push_back(ray_trace);
}