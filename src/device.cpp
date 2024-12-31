#include "../include/device.h"

/**
* initializes the counter of the id to 0
*/
int Device::id_counter = 0;

/**
* initializes the counter for the priority to 1
*/
int Device::priority_counter = 1;

/**
* set the start time of the Device
* @param time, supposed to be logically correct
*/
void Device::set_start_time(int time){
    start_time = time;
}

void Device::set_home(Home& h){
    home = h;
}

double Device::get_power() const {
    return power;
}

int Device::get_ID() const {
    return ID;
}

std::string Device::get_name() const {
    return name;
}

/**
* defines the order of Devices based on their priority
* @param other_Device, reference to the Device used for the comparison
* @return true if the implicit priority is less than the other Device's priority,
* false otherwise
*/
bool Device::operator<(Device& other_device) const{
    return (this->priority < other_device.priority);
}

void Device::start(){
    if(!running){
        priority = priority_counter++;
        start_time = home.get_time();
        running = true;
    }
}

void Device::stop(){
    if(running){
        priority = 0;
        running = false;
    }
}
