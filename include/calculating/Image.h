#ifndef OPTOCUS_IMAGE_H
#define OPTOCUS_IMAGE_H

#include <list>
#include "geometry/Point.h"
#include "geometry/Segment.h"

using std::list;

template <class T>
class Image {

    const T _object, _image;
    list<Segment> _real_rays;
    list<Segment> _imag_rays;

public:
    Image(const T &object, const T &image)
            : _object(object), _image(image) { }
    void addRay(const Segment&, bool isReal = true);
    const T &getObject() const { return _object; }
    const T &getImage() const { return _image; }
    const list<Segment> &getRealRays() const { return _real_rays; };
    const list<Segment> &getImagRays() const { return _imag_rays; }
};

template<class T>
void Image<T>::addRay(const Segment &segment, bool isReal)
{
    isReal ? _real_rays.push_back(segment) : _imag_rays.push_back(segment);
}

#endif