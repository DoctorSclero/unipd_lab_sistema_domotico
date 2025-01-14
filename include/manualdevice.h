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
             * @param power The power of the device (positive for producers, negative for consumers)
             * @param priority True if the device should be kept on when auto power off is activated, false otherwise
             */
            ManualDevice(const std::string name, const double power, const bool keep_on) 
            : Device{name, power, keep_on} {}

            /**
             * updates the state of the device based on its internal timers
             * and the home time
             * @throws device_not_subscribed
             */
            void update() override;

            /**
             * @param start_timer The start timer to be set, its value must
             * be greater than the current time and less than or equal to MINUTES_IN_DAY
             * @param stop_timer The stop timer to be set, its value
             * must be greater than start_timer and less than or equal to MINUTES_IN_DAY
             * @throws device_not_subscribed, bad_time_range
             */
            void set_timer(const int start_timer, const int stop_timer);

            /**
             * Removes both timers of the device by setting:
             *  - start_timer_ to -1
             *  - stop_timer_ to -1
             */
            void remove_timers() override;
    };
}

#endif
