#include "../include/device.h"
#include <sstream>

namespace domoticdevices {
    /**
    * initializes the counter of the id to 0
    */
    int Device::id_counter_ = 0;

    /**
    * initializes the counter for the priority to 1
    */
    int Device::priority_counter_ = 1;

    /**
    * sets the start time of the Device
    * @param time supposed to be logically correct
    */
    void Device::set_start_time(const int start_time){
        start_time_ = start_time;
    }

    void Device::set_home(Home& h){
        home_ = &h;
    }

    double Device::get_power() const {
        return power_;
    }

    int Device::get_id() const {
        return id_;
    }

    std::string Device::get_name() const {
        return name_;
    }
    int Device::get_priority() const{
        return priority_;
    }

    /**
    * defines the order of Devices based on their priority
    * @param other_device reference to the Device used for the comparison
    * @return true if the implicit priority is less than the other Device's priority,
    * false otherwise
    */
    bool Device::operator<(const Device& other_device) const { 
        return (this->priority_ < other_device.priority_);
    }

    /**
    * checks equality between two devices based on their name
    * @param other_device the other Device wihich is being confronted
    * @return true if the devices have the same name, false otherwise
    */
    bool Device::operator==(const Device& other_device) const {
        return (this->name_ == other_device.name_);
    }

    /**
    * checks equality between a device's name and a string
    * @param other_name the string which is supposed to contain a name of a device
    * @return true if the device's name is the same as other_name, false otherwise
    */
    bool Device::operator==(const std::string other_name) const {
        return (this->name_ == other_name);
    }

    /**
    * starts the device
    * 
    */
    void Device::start(){
        if(!running_){
            priority_ = priority_counter_++;
            start_time_ = home_->get_time();
            running_ = true;
        }
    }

    void Device::stop(){
        if(running_ && priority_ != -1){
            priority_ = 0;
            running_ = false;
        }
    }

    /**
     * Prints information about the device including
     * the total power consumed
     */
    std::string Device::to_string() const{
        std::stringstream res;

        res << "Il dispositivo " << name_ << " ha consumato " << total_power_ << "kWh" << std::endl;

        return res.str();
    }
}