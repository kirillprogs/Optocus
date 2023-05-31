#include "OpticalController.h"

OpticalController *OpticalController::instance() {
    if (!_instance)
        _instance = new OpticalController();
    return _instance;
}