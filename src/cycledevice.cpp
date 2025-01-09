#include "cycledevice.h"
#include "home.h"

namespace domoticdevices {
    /**
    * updates the state of the device:
    *  - turns on if time = start_time_
    *  - turns off if time = start_time_ + cycle_duration_
    *  - keeps current state otherwise
    * addds the amount of power consumed in a minute to total_power_
    */
    void CycleDevice::update(){
        if(home_->get_time() == start_time_){
            start();
        }else if(home_->get_time() == start_time_ + cycle_duration_){
            stop();
        }

        if(running_){
            total_power_ += power_ / 60;
        }
    }
}