#include "home.h"

#include <algorithm>
#include <iostream>
#include <sstream>

#include "manualdevice.h"

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

    // Command interface

    void Home::start_device(const std::string device_name) {
        auto device = find_if(
            this->devices_.begin(), 
            this->devices_.end(),
            [device_name] (Device* d) {
                return *d == device_name;
            }
        );

        if (device == this->devices_.end()) {
            // Device not found
            throw std::invalid_argument("cannot find device " + device_name);
            return;
        }

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
            // Device not found
            throw std::invalid_argument("cannot find device " + device_name);
        }

        (*device)->stop();
    }

    /**
     * Moves on the internal clock of the house,
     * informing the device of the passed time.
     * Device recieves updates from current_time_ to
     * time-1 included.
     */
    void Home::set_time(const int time) {
        // Throwing an error if the time is not in the correct range
        if (time <= this->get_time() || time >= 1440) {
            std::stringstream ss;
            ss << "Time must be between " << time/60 << ":" << time%60 << " and 23:59";
            throw std::invalid_argument(ss.str());
        }

        // Updating all the subscribed devices minute by minute
        while (this->current_time_ < time) {
            auto device = this->devices_.begin();
            while (device != this->devices_.end()) {
                (*device)->update();
            }
            this->current_time_++;
        }
    }
    
    void Home::set_start(const int time, const std::string device_name) {
        // Throwing an error if the time is not in the correct range
        // ? Should it be between current time and 1440?
        if (time < 0 || time >= 1440) {
            throw std::invalid_argument("Start time must be between 00:00 and 23:59");
        }

        auto device = find_if(
            this->devices_.begin(), 
            this->devices_.end(),
            [device_name] (Device* d) {
                return *d == device_name;
            }
        );

        // If device isn't found launch 
        if (device == this->devices_.end()) {
            throw std::invalid_argument("cannot find device " + device_name);
        }
        
        (*device)->set_start_time(time);
    }

    void Home::set_stop(const int time, const std::string device_name) {
        // Throwing an error if the time is not in the correct range
        if (time < 0 || time >= 1440) {
            throw std::invalid_argument("Start time must be between 00:00 and 23:59");
        }

        auto device = find_if(
            this->devices_.begin(), 
            this->devices_.end(),
            [device_name] (Device* d) {
                return *d == device_name;
            }
        );
        if (device == this->devices_.end()) {
            throw std::invalid_argument("cannot find device " + device_name);
        }

        ManualDevice* md = dynamic_cast<ManualDevice*>(*device);
        if (md != nullptr) md->set_stop_time(time);
        else {
            // TODO: launch exception not a manual device
        }
    }

    // ? Check for correct functionality
    void Home::show() {
        std::stringstream sstream;
        for (Device* device : this->devices_) {
            sstream << device->to_string();
        }
        this->get_logger().log(sstream.str());
    }

    // ? Check for correct functionality
    void Home::show(const std::string device_name) {
        
        auto device = find_if(
            this->devices_.begin(), 
            this->devices_.end(),
            [device_name] (Device* d) {
                return *d == device_name;
            }
        );

        if (device == this->devices_.end()) {
            throw std::invalid_argument("cannot find device " + device_name);
        }

        this->get_logger().log((*device)->to_string());
    }
    
    void Home::reset_time() {
        // Stopping all devices
        for (Device* device : this->devices_) {
            device->stop();
        }

        // Resetting the time of the house
        this->current_time_ = 0;
    }

    void Home::reset_timer(std::string device_name) {
        auto device = find_if(
            this->devices_.begin(),
            this->devices_.end(),
            [device_name] (Device* device) {
                return *device == device_name;
            }
        );

        (*device)->reset();
    }

    void Home::reset_timers() {
        for (Device* device : this->devices_) {
            device->reset();
        }
    }

    void Home::reset_all() {
        // Resetting time and removing timers
        this->reset_time();
        this->reset_timers();
    }

    // Observer pattern
    void Home::subscribe(Device& device) {
        if (
            // ! All find member function are comparing pointers with actual objects
            find_if(
                this->devices_.begin(), 
                this->devices_.end(),
                [&device] (Device* d) {
                    return *d == device;
                }
            ) != this->devices_.end()
        ) {
            this->devices_.push_back(&device);
            device.subscribe(*this);
            // TODO: subscribe house to device
        }
    }

    /**
     * ! Il metodo ha ripercussioni ricorsive stop_device
     * ! chiama Device::stop() che chiama Home::update()
     */
    void Home::update(const double consumption_delta) {
        
        // Sorting the devices by priority
        sort_devices(this->devices_);

        // Updating the total power consumed
        this->current_load_ += consumption_delta;
        
        // Shutting down devices if house power network is overloaded
        if (this->current_load_ > this->network_power_) {
            auto device = this->devices_.end();
            stop_device((*device)->get_name());
        }

    }

    /******************************************************
     * Logger implementation
     ******************************************************/

    Home::Logger::Logger(const char* file_path, const Home* home)
    : home_{home}, file_{file_path, std::ios_base::app} {
        if (!this->file_.is_open()) {
            throw std::runtime_error("Cannot open log file");
        }
        
    };

    Home::Logger::~Logger() {
        if (this->file_.is_open()) {
            this->file_.close();
        }
    }

    void Home::Logger::log(const std::string message) {
        std::stringstream ss;
        
        // Time formatting [minutes since 00:00] -> hh:mm
        int current_time = this->home_->get_time();
        int current_hour = current_time / 60;
        int current_min = current_time % 60;
        
        ss << "[" << current_hour << ":" << current_min << "] ";
        ss << message;
        
        std::cout << ss.str();
        this->file_ << ss.str();
    }
}