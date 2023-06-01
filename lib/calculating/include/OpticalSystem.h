#ifndef OPTOCUS_OPTICAL_SYSTEM_H
#define OPTOCUS_OPTICAL_SYSTEM_H

#include <algorithm>
#include <vector>
#include <list>
#include "Lens.h"
#include "Point.h"
#include "Segment.h"

using std::vector;
using std::list;
using std::sort;

class OpticalSystem {
    vector<Lens> _lenses;
    vector<Segment> _rays;      // stores rays which we add to the system
    list<Image<Point>> _images; // stores all object images
    list<Segment> _ray_images;  // stores all rays which we will draw
    Point *_object;

public:
    OpticalSystem() : _object(0) { }
    OpticalSystem(const OpticalSystem&) = delete;
    OpticalSystem& operator=(const OpticalSystem&) = delete;
    ~OpticalSystem() { delete _object; }

private:
    list<Image<Point>> evaluate_object();
    list<Segment> evaluate_ray(const Segment&);

public:
    /* Call it after every change in the system */
    void evaluate();
    void clear_geometry();
    void clear();

    const vector<Lens> &lenses() const { return _lenses; }
    const vector<Segment> &rays() const { return _rays; }
    void addLens(const Lens &lens) { _lenses.push_back(lens); }

    void set_object(const Point &point);
    void remove_object();
    const Point *object() const { return _object; }
    bool hasObject() const { return _object != 0; }

    const list<Image<Point>> &objectImages() const { return _images; }
    const list<Segment> &rayImages() const { return _ray_images; }
};

#endif