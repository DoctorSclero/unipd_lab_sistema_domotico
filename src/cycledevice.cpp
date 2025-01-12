#include "cycledevice.h"
#include "home.h"

namespace domoticdevices {
    /**
    * updates the state of the device:
    *  - turns on if home->get_time() = start_timer_
    *  - turns off if home->get_time() = start_time_ + cycle_duration_
    *  - keeps current state otherwise
    * adds the amount of power consumed in a minute to total_power_
    */
    void CycleDevice::update(){
        if(home_->get_time() == start_timer_){
            start();
        }else if(home_->get_time() == start_time_ + cycle_duration_){
            stop();
        }

        if(running_){
            total_power_ += power_ / 60;
        }
    }

    /**
    * 
    */
    void CycleDevice::remove_timers(){
        start_timer_ = -1;
    }
}