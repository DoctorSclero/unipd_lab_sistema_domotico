#ifndef DOMOTIC_DEVICES_CYCLEDEVICE_H
#define DOMOTIC_DEVICES_CYCLEDEVICE_H

#include "device.h"

namespace domoticdevices {
    class CycleDevice : public Device{
        private:
            int cycle_duration;
        public:
            CycleDevice(std::string n, double p, int pr, int cd)
                : Device{n, p, pr}, cycle_duration{cd} {}
                
            void update();
    };
}

#endif
