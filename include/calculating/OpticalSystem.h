#ifndef OPTOCUS_OPTICAL_SYSTEM_H
#define OPTOCUS_OPTICAL_SYSTEM_H

#include <vector>
#include "Lens.h"

using std::vector;

class OpticalSystem {
    struct SLens {
        Lens _lens;
        double _x;
    };

    vector<Lens> _lenses;

public:
    OpticalSystem() = default;
    OpticalSystem(const OpticalSystem& o) = default;
    OpticalSystem& operator=(const OpticalSystem& o) = default;

    const vector<Lens>& devices() const;
    void addDevice(const Lens&);
};

#endif