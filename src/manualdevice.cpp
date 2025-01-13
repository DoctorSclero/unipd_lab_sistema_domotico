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
        /**
         * The home isn't set in the costructor, so
         * its is mandatory to check wheter the home
         * was set later on or not
         */
        if(!home_) 
            throw device_not_subscribed(name_);

        if (start_timer_ == home_->get_time())
            start();
        else if (stop_timer_ == home_->get_time()) 
            stop();
        
        /**
         * Adds the power consumed/generated in a minute,
         * excluding the minute the device is started
         */
        if (running_ && start_time_ != home_->get_time()) 
            total_energy_ += power_ / 60;
    }

    /**
     * @param start_timer The start timer to be set, its value must
     * be greater than the current time and greater than MINUTES_IN_DAY
     * @param stop_timer The stop timer to be set, its value
     * must be greater than start_timer and less than or equal to MINUTES_IN_DAY
     * @throws device_not_subscribed, bad_time_range
     */
    void ManualDevice::set_timer(const int start_timer, const int stop_timer) {
        /**
         * The home isn't set in the costructor, so
         * its is mandatory to check wheter the home
         * was set later on or not
         */
        if(!home_) 
            throw device_not_subscribed(name_);

        if(start_timer <= home_->get_time() || start_timer > Home::MINUTES_IN_DAY)
            throw bad_time_range(timetostr(home_->get_time()+1), timetostr(Home::MINUTES_IN_DAY));

        if(stop_timer <= start_timer || stop_timer > Home::MINUTES_IN_DAY)
            throw bad_time_range(timetostr(start_timer+1), timetostr(Home::MINUTES_IN_DAY));

        start_timer_ = start_timer;
        stop_timer_ = stop_timer;
        
        //logging
        this->home_->get_logger().log("Impostato un timer per il dispositivo '" + name_ + "' dalle " + timetostr(start_timer) + " alle " + timetostr(stop_timer));
    }

    /**
     * Removes both timers of the device by setting:
     *  - start_timer_ to -1
     *  - stop_timer_ to -1
     */
    void ManualDevice::remove_timers() {
        start_timer_ = -1;
        stop_timer_ = -1;

        //logging
        home_->get_logger().log("Rimossi i timer dal dispositivo '" + name_ + "'");
    }
}
