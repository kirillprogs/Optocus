#ifndef OPTOCUS_SYSTEM_H
#define OPTOCUS_SYSTEM_H

#include <vector>
#include "Lens.h"
#include "RayTrace.h"

using std::vector;

class System {
    struct SLens {
        Lens _lens;
        double _x;
    };

    vector<SLens> _lenses;

public:
    System() = default;
    System(const System& o) = default;
    System& operator=(const System& o) = default;

    RayTrace trace(double k, double a, double x_origin = 0, double y_origin = 0) const;
};

#endif //OPTOCUS_SYSTEM_H