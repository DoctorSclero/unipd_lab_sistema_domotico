#include "../include/cycledevice.h"

namespace domoticdevices {
    /**
    * updates the state of the device:
    *  - turns on if time = start_time
    *  - turns off if time = start_time + cycle_duration
    *  - keeps current state otherwise
    * @param time, supposed to be logically correct
    * @return bool, true if device is on, false otherwise
    */
    void CycleDevice::update(){
        if(home.get_time() == start_time){
            start();
        }else if(home.get_time() == start_time + cycle_duration){
            stop();
        }
    }
}