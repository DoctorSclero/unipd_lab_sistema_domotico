#include "manualdevice.h"
#include "home.h"

namespace domoticdevices{
    /**
    * updates the state of the device:
    *  - turns on if start_time_ = home.get_time()
    *  - turns off if stop_time_ = home.get_time()
    *  - keeps current state otherwise
    * adds the amount of power consumed in a minute in total_power_
    */
    void ManualDevice::update(){
        if(start_time_ == home_->get_time()){
            start();
        }else if(stop_time_ == home_->get_time()){
            stop();
        }

        if(running_){
            total_power_ += power_ / 60;
        }
    }

    void ManualDevice::set_stop_time(const int time){
        stop_time_ = time;
    }
}
