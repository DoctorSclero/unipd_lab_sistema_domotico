/**
 * @author Diego Chiesurin
 * @matricola 2111553
 */

#include "manualdevice.h"
#include "home.h"
#include "exception.h"
#include "utils.h"

namespace domoticdevices{
    
    /**
     * updates the state of the device:
     *  - turns on if start_timer_ = home.get_time()
     *  - turns off if stop_timer_ = home.get_time()
     *  - keeps current state otherwise
     * adds the amount of power consumed/generated in a minute in total_energy_
     * @throws device_not_subscribed
     */
    void ManualDevice::update(){
        // Throwing exception if device is not connected to home
        if(!home_) throw device_not_subscribed(name_);

        if (start_timer_ == home_->get_time()) {
            start();
        } else if (stop_timer_ == home_->get_time()) {
            stop();
        }

        if (running_ && start_time_ != home_->get_time()) {
            total_energy_ += power_ / 60;
        }
    }

    /**
     * @param stop_timer The stop timer to be set, its value
     * must be greater than start_timer_ and less than or equal to MINUTES_IN_DAY
     * @throws bad_time_range
     */
    void ManualDevice::set_stop_timer(const int stop_timer) {
        if(stop_timer <= start_timer_ || stop_timer > Home::MINUTES_IN_DAY)
            throw bad_time_range(timetostr(start_timer_), timetostr(Home::MINUTES_IN_DAY));
        stop_timer_ = stop_timer;     
    }

    /**
     * Removes both timers of the device by setting:
     *  - start_timer_ to -1
     *  - stop_timer_ to -1
     */
    void ManualDevice::remove_timers(){
        start_timer_ = -1;
        stop_timer_ = -1;
    }
}
