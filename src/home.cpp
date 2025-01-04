#include "home.h"
#include <algorithm>
namespace domoticdevices {
    // Getters
    int Home::get_time() const {
        return this->current_time_;
    }

    /*
    Logger Home::get_logger() {
        return this->logger_;
    }
    */

    // Command interface
    void Home::start_device(std::string device_name) {
        auto device = find_if(
            this->devices_.begin(), 
            this->devices_.end(),
            [device_name] (Device& x) {
                return x.get_name() == device_name;
            }
        );

        if (device == this->devices_.end()) {
            // Device not found
            // TODO: launch an exception
            return;
        }

        device->start();
        // TODO: sort devices_
        // TODO: overloading detection and resolution
    }
    void Home::stop_device(std::string device_name){
        // Searching for the device with the matching name
        auto device = find_if(
            this->devices_.begin(), 
            this->devices_.end(),
            // The lambda function attempts to match the
            // passed device name with the one of the currently
            // checked device if true the device iterator is returned
            [device_name] (Device& x) {
                return x.get_name() == device_name;
            }
        );

        if (device == this->devices_.end()) {
            // Device not found
            // TODO: Launch an exception
            return;
        }

        device->stop();
        // TODO: sort devices_
        // TODO: overloading detection and resolution
    }
    void Home::set_time(int time) {
        // Updating all the subscribed devices
        // minute by minute
        while (this->current_time_++ < time) {
            auto device = this->devices_.begin();
            while (device != this->devices_.end()) {
                device->update();
            }
            // TODO: sort devices_
            // TODO: overloading detection and resolution
        }
    }
    void Home::set_start(int time, std::string device_name) {

    }
    void Home::set_stop(int time, std::string device_name) {

    }
    void Home::show() const {

    }
    void Home::show(std::string device_name) const {

    }
    // Observer pattern
    void Home::subscribe(Device& device) {
        if (
            find(
                this->devices_.begin(), 
                this->devices_.end(),
                device // ? How is device checked?
            ) != this->devices_.end()
        ) {
            this->devices_.push_back(device);
        }
    }
}