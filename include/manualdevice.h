/**
 * @author Diego Chiesurin
 * @matricola 2111553
 */

#ifndef DOMOTIC_DEVICES_MANUALDEVICE_H
#define DOMOTIC_DEVICES_MANUALDEVICE_H

#include "device.h"

namespace domoticdevices {
    class Home;
    class ManualDevice : public Device{
        private:
            int stop_timer_;
        public:
            /**
             * @param name The name of the device
             * @param power The power of the device (< 0 if consumes, > 0 if produces)
             * @param priority True if the device should be kept on when auto power off is activated, false otherwise
             */
            ManualDevice(const std::string name, const double power, const bool keep_on) 
            : Device{name, power, keep_on} {}

            /**
             * updates the state of the device based on its internal timers
             * and the home time
             */
            void update() override;

            /**
             * @param stop_timer The stop timer to be set, its value
             * must be greater than start_timer_ and less or equal than MAX_TIME
             * @throws bad_time_range
             */
            void set_stop_timer(const int stop_timer);

            /**
             * removes both the timers of the device
             * by setting that to -1
             */
            void remove_timers() override;
    };
}

#endif
