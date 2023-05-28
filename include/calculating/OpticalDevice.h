#ifndef OPTOCUS_OPTICAL_DEVICE_H
#define OPTOCUS_OPTICAL_DEVICE_H

#include "geometry/Point.h"

class OpticalDevice {

public:
    const Point image(const Point&) const;

};

#endif