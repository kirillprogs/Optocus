#ifndef OPTOCUS_OPTICAL_SYSTEM_H
#define OPTOCUS_OPTICAL_SYSTEM_H

#include <algorithm>
#include <vector>
#include <tuple>
#include <list>
#include "Lens.h"
#include "Point.h"
#include "Segment.h"

using std::vector;
using std::tuple;
using std::list;
using std::sort;
using std::make_tuple;

class OpticalSystem {
    vector<Lens> _lenses;
    vector<Segment> _rays;      // stores rays which we add to the
    list<Image<Point>> _images; // stores all object images
    list<Segment> _ray_images;  // stores all rays which we will draw
    Point *_object;

public:
    enum ElementType { LENS, OBJECT, OBJECT_IMAGE, RAY };

    OpticalSystem() : _object(0) { }
    OpticalSystem(const OpticalSystem&) = delete;
    OpticalSystem& operator=(const OpticalSystem&) = delete;
    ~OpticalSystem() { delete _object; }

private:
    list<Image<Point>> evaluate_object();
    list<Segment> evaluate_rays(const Segment&);

public:
    /* Call it after every change in the system */
    void evaluate();

    const vector<Lens>& lenses() const { return _lenses; }
    void addLens(const Lens &lens) { _lenses.push_back(lens); }

    void addObject(const Point &point) {
        _object = new Point(point);
    }

    void removeObject() {
        delete _object;
        _object = 0;
    }

    const Point *object() { return _object; }
    bool hasObject() const { return _object != 0; }

    const list<Image<Point>> &objectImages() const { return _images; }
    const list<Segment> &rayImages() const { return _ray_images; }

    /* Somehow organize returning of element on position specified */
    unsigned int elementAt(int x, int y) const;
    ElementType getElementType(int x, int y) const;

    /* Change parameters of selected lens */
    Lens& lens(unsigned int id);
};

#endif