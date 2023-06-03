#ifndef OPTOCUS_OPTICAL_CONTROLLER_H
#define OPTOCUS_OPTICAL_CONTROLLER_H

#include "OpticalSystem.h"

class OpticalController {

    static OpticalController *_instance;
    OpticalSystem optics;

public:
    static OpticalController *instance();
    enum ElementType { LENS, OBJECT, OBJECT_IMAGE, RAY };
private:
    OpticalController() { }

public:
    const vector<Lens> &get_lenses() const;
    const list<Segment> &get_rays() const;
    list<Segment> get_image_rays() const;
    bool has_object() const;
    Segment get_object() const;
    list<Segment> get_images() const;

    void clear_all();
    void clear_geometry();

    void add_lens(const Lens&);
    void add_ray(const Segment&);
    void set_object(double x, double y);

    /* Somehow organize returning of element on position specified */
    unsigned int elementAt(int x, int y) const;
    ElementType getElementType(int x, int y) const;

    /* Change parameters of selected lens */
    Lens& lens(unsigned int id);

};

#endif //OPTOCUS_OPTICAL_CONTROLLER_H