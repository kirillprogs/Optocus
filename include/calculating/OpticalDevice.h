#ifndef OPTOCUS_OPTICALDEVICE_H
#define OPTOCUS_OPTICALDEVICE_H

#include "Point.h"

class OpticalDevice {

public:
    const Point image(const Point&) const;

};

#endif