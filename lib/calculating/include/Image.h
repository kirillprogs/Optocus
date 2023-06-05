#ifndef OPTOCUS_IMAGE_H
#define OPTOCUS_IMAGE_H

#include <list>
#include "Point.h"
#include "Segment.h"

using std::list;

template <class T>
class Image {
    const T _object, _image;
    const bool _is_real, _exists;
    list<Segment> _rays;

public:
    Image(const T &object, const T &image, bool is_real = true, bool exists = true)
            : _object(object), _image(image)
            , _is_real(is_real), _exists(exists) { }
    Image(const Image<T>&) = default;
    Image<T> &operator=(const Image<T>&) = delete;
    bool exists() { return _exists; }
    bool is_real() const { return _is_real; }
    void addRay(const Segment&);
    const T &getObject() const { return _object; }
    const T &getImage() const { return _image; }
    const list<Segment> &getRays() const { return _rays; }
};

template<class T>
void Image<T>::addRay(const Segment &segment) {
    _rays.push_back(segment);
}

#endif