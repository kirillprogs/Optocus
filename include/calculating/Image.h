#ifndef OPTOCUS_IMAGE_H
#define OPTOCUS_IMAGE_H

#include <list>
#include "Point.h"
#include "Segment.h"

using std::list;

class Image {

    Point _point, _image;
    list<Segment> _rays;

public:
    Image(Point point, Point image)
            : _point(point), _image(image) { }
    void addRay(const Segment &);
    const list<Segment>& getRays() const;
};

#endif