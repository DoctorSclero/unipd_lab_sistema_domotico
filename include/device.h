#ifndef DOMOTIC_DEVICES_DEVICE_H
#define DOMOTIC_DEVICES_DEVICE_H

#include <string>
#include "home.h"

namespace domoticdevices {
    /**
    * base abstract class
    * represents the common caracteristics of a Device
    */
    class Device {
        protected:
            /**
            * this is used to assign a unique id to 
            * every new object of class Device 
            * that is created
            */
            static int id_counter_;
            /**
            * this is used to assign 
            */
            static int priority_counter_;
            int id_;
            std::string name_;
            double power_; //both positive or negative
            /**
            * this is used to manage the priority logic
            * which manages the order of shut down when
            * the maximum power limit of the house is reached:
            * - -1 indicates that the Device should
            *   never be turned off automatically
            * - 0 indicates the the Device is turned off
            * - > 0 indicates that the Device is turned on, 
            *   the Device with the highest priority is the first
            *   to be shut down
            */
            int priority_;
            int start_time_;
            bool running_;
            double total_power_;
            Home* home_; // ? Can we use smart pointers to prevent dangling pointers
            
            Device(const std::string name, const double power, const int priority)
            : id_{id_counter_++}, running_{false}, name_{name}, priority_{priority}, start_time_{-1}, power_{power} {}
        public:
            Device(const Device&) = delete;
            Device& operator=(const Device&) = delete;
            
            bool operator<(const Device& other_device) const;
            bool operator==(const Device& other_device) const;
            bool operator==(const std::string other_name) const;
            std::string get_name() const;
            int get_id() const;
            double get_power() const;
            int get_priority() const;
            bool is_running() const;
            void set_start_time(const int time);
            void set_home(Home& h);
            void start();
            void stop();
            std::string to_string() const;

            virtual void update() = 0;
    };
}

#endif
