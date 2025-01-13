/**
 * @author Diego Chiesurin
 * @matricola 2111553
 */

#include "cycledevice.h"
#include "home.h"
#include "exception.h"

namespace domoticdevices {
    /**
     * @param name The name of the device
     * @param power The power of the device (< 0 if consumes, > 0 if produces)
     * @param keep_on True if the device should be kept on when auto power off is activated, false otherwise
     * @param cycle_duration The cycle duration of the device, must be > 0
     * @throws bad_cycle_duration
     */
    CycleDevice::CycleDevice(const std::string name, const double power, const bool keep_on, const int cycle_duration)
    : Device{name, power, keep_on} {
        if(cycle_duration > 0)
            cycle_duration_ = cycle_duration;
        else    
            throw bad_cycle_duration();
    }

    /**
     * updates the state of the device:
     *  - turns on if home->get_time() = start_timer_
     *  - turns off if home->get_time() = start_time_ + cycle_duration_
     *  - keeps current state otherwise
     * adds the amount of power consumed/generated in a minute to total_energy_
     * @throws device_not_subscribed
     */
    void CycleDevice::update(){
        /**
         * The home isn't set in the costructor, so
         * its is mandatory to check wheter the home
         * was set later on or not
         */
        if(!home_) 
            throw device_not_subscribed(name_);
        
        if (home_->get_time() == start_timer_) 
            start();
        else if (home_->get_time() == start_time_ + cycle_duration_) 
            stop();
        
        /**
         * Adds the power consumed/generated in a minute,
         * excluding the minute the device is started
         */
        if(running_ && start_time_ != home_->get_time())
            total_energy_ += power_ / 60;
    }
}