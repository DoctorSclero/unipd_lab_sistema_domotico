/**
 * @author Pietro Ballarin
 * @matricola 2109942
 */

#include "home.h"

#include <algorithm>
#include <iostream>
#include <sstream>

#include "manualdevice.h"
#include "exception.h"
#include "utils.h"

namespace domoticdevices {

    // Utility functions
    
    /**
     * Storts the devices vector by prioirity
     * being the complexity of insertion sort = O(n + k)
     * where n is the dimension of the array and k the number 
     * of possible inversions. Then the total complexity of 
     * sort_device used by home is O(n + c)
     * where c is a constant ranging from 0 to n - 1
     * because only one object's priority has been changed every 
     * this function is called, so the number of possible inversions
     * is, at most, the number of elements before or after the 
     * modified object, which is n - 1
     */
    void sort_devices(std::vector<Device*>& devices) {
        for (int i = 1; i < devices.size(); i++) {
            Device* key = devices[i];
            int j = i - 1;
            while (j >= 0 && devices[j]->get_priority() > key->get_priority()) {
                devices[j + 1] = devices[j];
                j = j - 1;
            }
            devices[j + 1] = key;
        }
    }

    int Home::get_time() const {
        return this->current_time_;
    }

    Home::Logger& Home::get_logger() {
        return this->logger_;
    }

    int Home::get_priority_counter() {
        return this->priority_counter_++;
    }

    void Home::start_device(const std::string device_name) {
        auto device = find_if(
            this->devices_.begin(), 
            this->devices_.end(),
            [device_name] (Device* d) {
                return *d == device_name;
            }
        );

        if (device == this->devices_.end()) {
            throw device_not_found(device_name);
        }

        this->get_logger().log("L'orario attuale e' " + timetostr(this->get_time()));

        (*device)->start();
    }

    void Home::stop_device(const std::string device_name) {
        // Searching for the device with the matching name
        auto device = find_if(
            this->devices_.begin(), 
            this->devices_.end(),
            [device_name] (Device* d) {
                return *d == device_name;
            }
        );

        if (device == this->devices_.end()) {
            throw device_not_found(device_name);
        }

        this->get_logger().log("L'orario attuale e' " + timetostr(this->get_time()));

        (*device)->stop();
    }

    void Home::set_time(const int time) {
        // Throwing an error if the time is not in the correct range
        if (time <= this->get_time() || time > MINUTES_IN_DAY) {
            throw bad_time_range(timetostr(this->get_time()), timetostr(MINUTES_IN_DAY));
        }
        
        this->get_logger().log("L'orario attuale e' " + timetostr(this->get_time()));

        // Creating a copy of the devices_ array to prevent
        // loops caused by device_ reordering during device updates.
        // O(N) time operation, could be transformed in O(1) by
        // keeping a copy of the devices_ array in the class members
        // the impact in the application is negligeable given the
        // low amount of devices the house has from specification.
        std::vector<Device*> devices_copy = devices_;

        // Updating all the subscribed devices minute by minute
        while (this->current_time_ < time) {
            this->current_time_++;
            auto device = devices_copy.begin();
            while (device != devices_copy.end()) {
                (*device++)->update();
            }
        }

        this->logger_.log("L'orario attuale e' " + timetostr(this->get_time()));
    }
    
    void Home::set_timer(const int start_time, const std::string device_name) {
        // Finding the device
        auto device = find_if(
            this->devices_.begin(), 
            this->devices_.end(),
            [device_name] (Device* d) {
                return *d == device_name;
            }
        );

        // If device isn't found throw exception
        if (device == this->devices_.end()) {
            throw device_not_found(device_name);
        }
        
        this->get_logger().log("L'orario attuale e' " + timetostr(this->get_time()));

        (*device)->set_timer(start_time);
    }

    void Home::set_timer(const int start_time, const int stop_time, const std::string device_name) {
        // Finding the device
        auto device = find_if(
            this->devices_.begin(), 
            this->devices_.end(),
            [device_name] (Device* d) {
                return *d == device_name;
            }
        );

        // If device isn't found throw exception
        if (device == this->devices_.end()) {
            throw device_not_found(device_name);
        }

        this->get_logger().log("L'orario attuale e' " + timetostr(this->get_time()));

        ManualDevice* md = dynamic_cast<ManualDevice*>(*device);
        if (md != nullptr) {
            // Throwing an error if the time is not in the correct range
            if (stop_time <= start_time) {
                throw bad_time_range(timetostr(start_time), timetostr(MINUTES_IN_DAY));
            }
            md->set_timer(start_time, stop_time);
        } else {
            throw bad_device_type("Il dispositivo deve essere un dispositivo manuale");
        }
    }

    void Home::show() {
        this->get_logger().log("L'orario attuale e' " + timetostr(this->get_time()));
        // Appending the show of single devices
        std::stringstream ss;
        for (Device* device : this->devices_) {
            ss << device->to_string();
        }
        // Logging
        this->get_logger().log(ss.str());
    }

    void Home::show(const std::string device_name) {
        // Finding the desired device
        auto device = find_if(
            this->devices_.begin(), 
            this->devices_.end(),
            [device_name] (Device* d) {
                return *d == device_name;
            }
        );

        // Device not found
        if (device == this->devices_.end()) {
            throw device_not_found(device_name);
        }

        this->get_logger().log("L'orario attuale e' " + timetostr(this->get_time()));
        this->get_logger().log((*device)->to_string());
    }
    
    void Home::reset_time() {
        this->get_logger().log("L'orario attuale e' " + timetostr(this->get_time()));

        // Stopping all devices
        for (Device* device : this->devices_) {
            device->reset();
        }
        
        // Resetting the time of the house
        this->current_time_ = 0;
    }

    void Home::reset_timer(std::string device_name) {
        // Finding the desired device
        auto device = find_if(
            this->devices_.begin(),
            this->devices_.end(),
            [device_name] (Device* device) {
                return *device == device_name;
            }
        );

        // Device not found
        if (device == this->devices_.end()) {
            throw device_not_found(device_name);
        }

        this->get_logger().log("L'orario attuale e' " + timetostr(this->get_time()));
        (*device)->remove_timers();
    }

    void Home::reset_timers() {
        this->get_logger().log("L'orario attuale e' " + timetostr(this->get_time()));
        for (Device* device : this->devices_) {
            device->remove_timers();
        }
    }

    void Home::reset_all() {
        this->get_logger().log("L'orario attuale e' " + timetostr(this->get_time()));
        // Resetting time and removing timers
        this->reset_time();
        this->reset_timers();
    }

    // Observer pattern
    void Home::subscribe(Device& device) {
        // Detecting duplicate devices
        if (
            find_if(
                this->devices_.begin(), 
                this->devices_.end(),
                [&device] (Device* d) {
                    return *d == device;
                }
            ) == this->devices_.end()
        ) {
            this->devices_.push_back(&device);
            device.subscribe(*this);
        } else {
            throw device_already_present(device.get_name());
        }
    }

    void Home::update(Device& caller) {
        
        // Sorting the devices by priority
        sort_devices(this->devices_);

        // Updating the total power consumed
        if (caller.is_running()) this->current_load_ -= caller.get_power();
        else this->current_load_ += caller.get_power();

        // Updating devices priority of next
        auto device = this->devices_.rbegin();
        while ((*device)->is_running() && device != this->devices_.rend()) {
            caller.decrease_priority();
            device++;
        }

        // Shutting down devices if house power network is overloaded by priority
        if (this->current_load_ > this->network_power_) {
            auto device = this->devices_.rbegin();

            while (current_load_ > network_power_ && device != this->devices_.rend()) {
                if ((*device)->is_running()) {
                    current_load_ += (*device)->get_power();
                    (*device)->force_stop();
                }
                device++;
            }
        }
    }

    /******************************************************
     * Logger implementation
     ******************************************************/

    Home::Logger::Logger(const char* file_path, const Home* home)
    : home_{home}, file_{file_path, std::ios_base::app} {
        // Detecting file problems
        if (!this->file_.is_open()) {
            throw std::runtime_error("Impossibile aprire il file di log");
        }
    };

    Home::Logger::~Logger() {
        // Managing resources
        if (this->file_.is_open()) {
            this->file_.close();
        }
    }

    void Home::Logger::log(const std::string message) {
        std::stringstream ss;

        // Calcola ore e minuti
        ss << "[" << timetostr(this->home_->get_time()) << "] ";
        ss << message;
        std::cout << ss.str() << std::endl;
        this->file_ << ss.str() << std::endl;
    }
}