#ifndef OPTOCUS_OPTICAL_SYSTEM_H
#define OPTOCUS_OPTICAL_SYSTEM_H

#include <vector>
#include "Lens.h"
#include "OpticalDevice.h"

using std::vector;

class OpticalSystem {
    struct SLens {
        Lens _lens;
        double _x;
    };

    vector<OpticalDevice> _lenses;

public:
    OpticalSystem() = default;
    OpticalSystem(const OpticalSystem& o) = default;
    OpticalSystem& operator=(const OpticalSystem& o) = default;

    const vector<OpticalDevice>& device() const;
    void addDevice(const OpticalDevice&);
};

#endif