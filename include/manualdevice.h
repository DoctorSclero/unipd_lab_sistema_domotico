#ifndef DOMOTIC_DEVICES_MANUALDEVICE_H
#define DOMOTIC_DEVICES_MANUALDEVICE_H

#include "device.h"

namespace domoticdevices {
    class Device;
    class Logger;
    class CycleDevice;
    class Home;
    class ManualDevice : public Device{
        private:
            int stop_time_;
        public:
            ManualDevice(const std::string name, const double power, const int priority)
            : Device{name, power, priority}, stop_time_{-1} {}

            void set_stop_time(const int time);
            
            void update();
    };
}

#endif
