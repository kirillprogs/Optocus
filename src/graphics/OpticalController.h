#ifndef OPTOCUS_OPTICAL_CONTROLLER_H
#define OPTOCUS_OPTICAL_CONTROLLER_H

class OpticalController {

    static OpticalController *_instance;

public:
    static OpticalController *instance();
private:
    OpticalController() { _instance = 0; }
public:

};

#endif //OPTOCUS_OPTICAL_CONTROLLER_H