#include "home.h"
#include "device.h"
#include "cycledevice.h"
#include "manualdevice.h"
#include <algorithm>


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

    Logger* Home::get_logger() const {
        return this->logger_;
    }

    // Command interface
    void Home::start_device(const std::string device_name) {
        auto device = find_if(
            this->devices_.begin(), 
            this->devices_.end(),
            [device_name] (Device* x) {
                return x->get_name() == device_name;
            }
        );

        if (device == this->devices_.end()) {
            // Device not found
            // TODO: launch an exception
            return;
        }

        (*device)->start();
        sort_devices(this->devices_);
        // TODO: overloading detection and resolution
    }

    void Home::stop_device(const std::string device_name){
        // Searching for the device with the matching name
        auto device = find(
            this->devices_.begin(), 
            this->devices_.end(),
            device_name
        );

        if (device == this->devices_.end()) {
            // Device not found
            // TODO: Launch an exception
            return;
        }

        (*device)->stop();
        sort_devices(this->devices_);
        // TODO: overloading detection and resolution
    }

    void Home::set_time(const int time) {
        // Updating all the subscribed devices
        // minute by minute
        while (this->current_time_++ < time) {
            auto device = this->devices_.begin();
            while (device != this->devices_.end()) {
                (*device)->update();
            }
            sort_devices(this->devices_);
            // TODO: overloading detection and resolution
        }
    }
    
    void Home::set_start(const int time, const std::string device_name) {
        auto device = find(
            this->devices_.begin(),
            this->devices_.end(),
            device_name
        );

        if (device == this->devices_.end()) {
            // TODO: launch exception
        }

        (*device)->set_start_time(time);
    }

    void Home::set_stop(const int time, const std::string device_name) {
        auto device = find(
            this->devices_.begin(),
            this->devices_.end(),
            device_name
        );

        if (device == this->devices_.end()) {
            // TODO: launch exception
        }

        ManualDevice* md = dynamic_cast<ManualDevice*>(*device);
        md->set_stop_time(time);
    }

    void Home::show() const {

    }

    void Home::show(const std::string device_name) const {
        auto device = find(
            this->devices_.begin(),
            this->devices_.end(),
            device_name
        );
        
        if (device == this->devices_.end()) {
            // TODO: launch exception
        }

        this->get_logger()->log((*device)->to_string());
    }
    
    // Observer pattern
    void Home::subscribe(Device& device) {
        if (
            find(
                this->devices_.begin(), 
                this->devices_.end(),
                device
            ) != this->devices_.end()
        ) {
            this->devices_.push_back(&device);
        }
    }
}