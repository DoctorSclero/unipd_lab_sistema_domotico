#ifndef MANUALDEVICE_H
#define MANUALDEVICE_H

#include "device.h"

class ManualDevice : public Device{
    private:
        int stop_time;
    public:
        ManualDevice(std::string n, double p, int pr)
         : Device{n, p, pr}, stop_time{-1} {}
         
        bool update();
};

#endif
