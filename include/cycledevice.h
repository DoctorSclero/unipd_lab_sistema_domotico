/**
 * @author Diego Chiesurin
 * @matricola 2111553
 */

#ifndef DOMOTIC_DEVICES_CYCLEDEVICE_H
#define DOMOTIC_DEVICES_CYCLEDEVICE_H

#include "device.h"
#include <string>

namespace domoticdevices {
    class Home;
    class CycleDevice : public Device{
        private:
            int cycle_duration_;
        public:
            /**
            * @param name The name of the device
            * @param power The power of the device (< 0 if consumes, > 0 if produces)
            * @param keep_on True if the device should be kept on when auto power off is activated, false otherwise
            * @param cycle_duration The cycle duration of the device, must be > 0
            * @throws bad_cycle_duration
            */
            CycleDevice(const std::string name, const double power, const bool keep_on, const int cycle_duration);
                
            /**
            * updates the state of the device:
            *  - turns on if home->get_time() = start_timer_
            *  - turns off if home->get_time() = start_time_ + cycle_duration_
            *  - keeps current state otherwise
            * adds the amount of power consumed in a minute to total_energy_
            */
            void update() override;

            /**
            * removes that start_timer_ by setting it 
            * at -1
            */
            void remove_timers() override;
    };
}

#endif
