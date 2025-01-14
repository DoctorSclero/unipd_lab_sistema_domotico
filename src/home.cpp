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

    /******************************************************
     * Home implementation
     ******************************************************/

    // Constructors

    Home::Home(const double network_power, const char* logfile_path)
    : network_power_{network_power}, logger_{logfile_path, this}, current_time_{0}, priority_counter_{1} {
        if (this->network_power_ < 0) throw std::invalid_argument("La potenza della rete della casa deve essere >= 0");
    }

    // Getters

    int Home::get_time() const {
        return this->current_time_;
    }

    Home::Logger& Home::get_logger() {
        return this->logger_;
    }

    int Home::get_priority_counter() {
        return this->priority_counter_++;
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

        if (device == this->devices_.end()) {
            throw device_not_found(device_name);
        }
        
        this->get_logger().log("L'orario attuale e' " + timetostr(this->get_time()));

        (*device)->set_timer(start_time);
    }

    void Home::set_timer(const int start_time, const int stop_time, const std::string device_name) {

        // Finding the device by name
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

        // Attempting ManualDevice conversion
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

        std::stringstream ss;

        // Calculating generation and consumation
        double generated = 0;
        double consumed = 0;
        for (Device* device : this->devices_) {
            if(device->get_total_energy() > 0) generated += device->get_total_energy();
            else consumed -= device->get_total_energy();
        }
        ss << "Attualmente il sistema ha prodotto " << generated << " kWh e consumato " << consumed << " kWh. ";
        ss << "Nello specifico:" << std::endl;

        // Appending devices info
        auto device = this->devices_.begin();
        while (device != this->devices_.end()) {
            ss << "\t- " << (*device)->to_string();
            if (device != this->devices_.end()-1) {
                ss << std::endl;
            }
            device++;
        }

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

        for (Device* device : this->devices_) {
            device->reset();
        }
        
        this->current_time_ = 0;

        this->get_logger().log("L'orario attuale e' " + timetostr(this->get_time()));
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
        std::sort(this->devices_.begin(), this->devices_.end(), [](Device* a, Device* b) {
            return a->get_priority() < b->get_priority();
        });

        // Updating the total power consumed
        if (caller.is_running()) this->current_load_ -= caller.get_power(); // Device was started
        else {                                                              // Device was stopped
            this->current_load_ += caller.get_power();

            // Updating priority of shut non-keep-on devices
            if (caller.get_priority() == 0) {
                this->priority_counter_--;
                auto device = this->devices_.rbegin();
                while ((*device)->is_running() && device != this->devices_.rend()) {
                    caller.decrease_priority();
                    device++;
                }
            }
        }

        // Shutting down devices if house power network is overloaded by priority
        if (this->current_load_ > this->network_power_) {
            auto device = this->devices_.rbegin();

            while (current_load_ > network_power_ && device != this->devices_.rend()) {
                if ((*device)->is_running()) {
                    this->priority_counter_--;
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