#ifndef DOMOTIC_DEVICES_CYCLEDEVICE_H
#define DOMOTIC_DEVICES_CYCLEDEVICE_H

#include "device.h"

namespace domoticdevices {
    class Home;
    class CycleDevice : public Device{
        private:
            int cycle_duration_;
        public:
            CycleDevice(const std::string name, const double power, const int priority, const int cycle_duration)
                : Device{name, power, priority}, cycle_duration_{cycle_duration} {}
                
            void update();
    };
}

#endif
