#ifndef DOMOTIC_DEVICES_MANUALDEVICE_H
#define DOMOTIC_DEVICES_MANUALDEVICE_H

#include "device.h"

namespace domoticdevices {
    class ManualDevice : public Device{
        private:
            int stop_time;
        public:
            ManualDevice(std::string n, double p, int pr)
            : Device{n, p, pr}, stop_time{-1} {}
            
            bool update();
    };
}

#endif
