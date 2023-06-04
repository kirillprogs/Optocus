#ifndef OPTOCUS_OPTICAL_CONTROLLER_H
#define OPTOCUS_OPTICAL_CONTROLLER_H

#include "OpticalSystem.h"

class OpticalController {

    static OpticalController *_instance;
    static const int WIDTH = 1200;
    static const int HEIGHT = 720;
    static const int CELL_SIZE = 24;
    OpticalSystem _optics;
    double _scale;

public:
    static OpticalController *instance();
    enum ElementType { LENS, OBJECT, OBJECT_IMAGE, RAY };
private:
    OpticalController() : _scale(1.) { }

public:
    int width() { return WIDTH; }
    int height() { return HEIGHT; }
    int centerX() { return width() / 2; }
    int centerY() { return height() / 2; }
    int cell_size() { return CELL_SIZE; }
    double &scale() { return _scale; }
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

    /* SCALING */
    /* Full screen is 1 meter if _scale is 1.0 */
    /* 1 pixel is (1 meter / _scale) / width() */
    /* X coordinate is (screenX - width() / 2) / (_scale * width())
     * = (1 / _scale) * ((screenX / width()) - 0.5) */
    inline double meters_in_pixel() { return 1 / (_scale * width()); }

    inline double getX(int screenX) {
        return ((double)screenX - 0.5 * width()) * meters_in_pixel();
    }

    inline double getY(int screenY) {
        return ((double)screenY - 0.5 * height()) * meters_in_pixel();
    }

    inline int screenX(double x) {
        return (int)((x / meters_in_pixel()) + 0.5 * width());
    }

    inline int screenY(double y) {
        return (int)((y / meters_in_pixel()) + 0.5 * height());
    }

    Point screenPoint(Point a) { return Point(screenX(a.x()), screenY(a.y())); }
    Segment screenSegment(Segment a) { return Segment(screenPoint(a.start()), screenPoint(a.end())); }
};

#endif //OPTOCUS_OPTICAL_CONTROLLER_H