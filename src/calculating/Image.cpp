#include "calculating/Image.h"

void Image::addRay(const Segment &segment) { _rays.push_back(segment); }
const list<Segment> &Image::getRays() const { return _rays; }