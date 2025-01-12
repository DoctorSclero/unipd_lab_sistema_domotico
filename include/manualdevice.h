#ifndef DOMOTIC_DEVICES_MANUALDEVICE_H
#define DOMOTIC_DEVICES_MANUALDEVICE_H

#include "device.h"

namespace domoticdevices {
    class Home;
    class ManualDevice : public Device{
        private:
            int stop_timer_;
        public:
            ManualDevice(const std::string name, const double power, const int priority)
            : Device{name, power, priority}, stop_timer_{-1} {}

            void set_stop_timer(const int stop_timer);
            
            void update();

            void remove_timers();
            
            void reset();
    };
}

#endif
