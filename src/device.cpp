/**
 * @author Diego Chiesurin
 * @matricola 2111553
 */

#include "device.h"
#include "exception.h"
#include "utils.h"

#include <sstream>
#include <cmath>

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
     * sets the start timers only if the new timer 
     * is greater than the current time and less than or equal to MINUTES_IN_DAY
     * @param start_timer The new start timer to be set
     * @throws bad_time_range, device_not_subscribed
     */
    void Device::set_timer(const int start_timer){
        /**
         * The home isn't set in the costructor, so
         * its is mandatory to check wheter the home
         * was set later on or not
         */
        if(!home_) 
            throw device_not_subscribed(name_);

        if (start_timer <= home_->get_time() || start_timer > Home::MINUTES_IN_DAY)
            throw bad_time_range(timetostr(home_->get_time()+1), timetostr(Home::MINUTES_IN_DAY));
        
        start_timer_ = start_timer;

        //logging
        home_->get_logger().log("Impostato un timer per il dispositivo '" + name_ + "' dalle " + timetostr(start_timer));
    }

    /**
     * House observer, subscribes an house
     * to handle device status changes
     * (es. device turning on notifications)
     * @param h The home to subscribe
     */
    void Device::subscribe(Home& h){
        home_ = &h;
    }

    /**
     * Retrieves the name of the device
     * @return The name of the device
     */
    std::string Device::get_name() const {
        return name_;
    }

    /**
     * Retrieves the priority of the device
     * @return The priority of the device
     */
    int Device::get_priority() const{
        return priority_;
    }

    /**
     * Retrieves the running state of the Device
     * @return True if the Device is running, false otherwise
     */
    bool Device::is_running() const {
        return running_;
    }

    /**
     * Checks equality between two devices based on their names.
     * @param other_device The other Device to compare
     * @return true if the devices have the same name, false otherwise
     */
    bool Device::operator<(const Device& other_device) const { 
        return (this->priority_ < other_device.priority_);
    }

    /**
     * checks equality between two devices based on their names
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
     * starts the device if it isn't already running
     * and updates the home about the power change
     * @throws device_not_subscribed
     */
    void Device::start(){
        /**
         * The home isn't set in the costructor, so
         * its is mandatory to check wheter the home
         * was set later on or not
         */
        if(!home_) 
            throw device_not_subscribed(name_);

        if(!running_){
            //Maintaining "keep on" priority 
            if(priority_ != -1)
                priority_ = priority_counter_++;

            start_time_ = home_->get_time();
            running_ = true;

            //logging
            home_->get_logger().log("Il dispositivo '" + name_ + "' si e' acceso");
            //update the home about the power change
            home_->update(-power_);
        }
    }

    /**
     * stops the device only if the device is running
     * and updates the home about the power change
     */
    void Device::stop(){
        /**
         * The home isn't set in the costructor, so
         * its is mandatory to check wheter the home
         * was set later on or not
         */
        if(!home_) 
            throw device_not_subscribed(name_);

        if(running_){
            // Maintaining "keep on" priority
            if (priority_ > 0) 
                priority_ = 0;

            running_ = false;
            start_time_ = -1;

            //logging
            home_->get_logger().log("Il dispositivo '" + name_ + "' si e' spento");
            //update the home about the power change
            home_->update(power_);
        }
    }

    /**
     * Resets the device to its inital state and
     * gets turned off.
     * the function stop() is called and
     * total_energy_ is set to 0
     */
    void Device::reset(){
        stop();
        total_energy_ = 0;
    }

    /**
     * Prints information about the device including
     * the total energy consumed/generated, from 00:00 to the current time
     */
    std::string Device::to_string() const{
        std::stringstream res;

        res << "Il dispositivo '" << name_ << (power_ >= 0 ? "' ha generato " : "' ha consumato ") << abs(total_energy_) << "kWh" << std::endl;

        return res.str();
    }

    /**
     * removes that start_timer_ by setting it 
     * at -1
     */
    void Device::remove_timers(){
        start_timer_ = -1;

        //logging
        home_->get_logger().log("Rimosso il timer dal dispositivo '" + name_ + "'");
    }
}