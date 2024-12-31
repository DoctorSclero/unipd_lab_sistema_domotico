#ifndef CYCLEDEVICE_H
#define CYCLEDEVICE_H

#include "device.h"

class CycleDevice : public Device{
    private:
        int cycle_duration;
    public:
        CycleDevice(std::string n, double p, int pr, int cd)
            : Device{n, p, pr}, cycle_duration{cd} {}
            
        void update();
};

#endif
