#ifndef DOMOTIC_DEVICES_DEVICE_H
#define DOMOTIC_DEVICES_DEVICE_H

#include <string>
#include "home.h"

namespace domoticdevices {
    class Home;
    
    /**
    * base abstract class
    * represents the common caracteristics of a Device
    */
    class Device {
        protected:
            /**
            * Used to assign a unique id to 
            * every new object of class Device 
            * that is created
            */
            static int id_counter_;
            /**
             * Determines the current priority.
             * Increases by one on every device startup
             * Decreases by one on every device shutdown
             */
            static int priority_counter_;
            int id_;
            std::string name_;
            double power_; //both positive or negative
            /**
            * Used to handle the order of shutdown when
            * the maximum power limit of the house is reached.
            * Device are shut following this criteria:
            * - `priority == -1`: indicates that the Device should be kept on as much as possible
            * - `priority == 0`:  indicates the the Device is already turned off
            * - `priority > 0`: indicates that the Device is turned on, the Device with the highest priority is the first to be shut down
            */
            int priority_;
            int start_time_;
            int start_timer_;
            bool running_;
            double total_power_;
            Home* home_;
            
            /**
             * @param name The name of the device
             * @param power The power of the device
             */
            Device(const std::string name, const double power, const int priority)
            : id_{id_counter_++}, running_{false}, name_{name}, priority_{priority}, start_time_{-1}, start_timer_{-1}, power_{power} {}

        public:
            /**
             * Preventing slicing
             */
            Device(const Device&) = delete;
            Device& operator=(const Device&) = delete;
            
            /**
             * Compares two devices by their priority.
             * @param other_device The device to compare
             * @returns true if device < other_device
             */
            bool operator<(const Device& other_device) const;

            /**
             * Checks equality of two devices
             * @param other_device The device to check
             * @returns true if the two devices are the same
             */
            bool operator==(const Device& other_device) const;

            /**
             * Checks if device has a particular name
             * @param other_name The name to check
             * @return true if device has the same name as other name
             */
            bool operator==(const std::string other_name) const;

            /**
             * Retrieves the name of the device
             * @returns The name of the device
             */
            std::string get_name() const;

            /**
             * Retrieves the id of the device
             * @returns The id of the device
             */
            int get_id() const;

            /**
             * Retrieves the power of the device
             * @returns The power of the device
             */
            double get_power() const;

            /**
             * Retrieves the priority of the device
             * @returns The priority of the device
             */
            int get_priority() const;

            /**
             * Checks whether the device is running
             * @returns true if the device is running
             */
            bool is_running() const;

            /**
             * Alters the device starting time
             * @param time The new starting time
             */
            void set_start_timer(const int start_timer);

            /**
             * House observer, subscribes an house
             * to handle device status changes
             * (es. device turning on notifications)
             * @param h The home to subscribe
             */
            void subscribe(Home& h);

            /**
             * Starts the device
             */
            void start();

            /**
             * Stops the device
             */
            void stop();

            /**
             * Creates a string representation
             * of the power consumption of the day
             */
            std::string to_string() const;

            /**
            * 
             */
            void reset();

            /**
             * Used by the home for notifying time updates.
             * Let devices handle starting up and stopping
             * on their own.
             */
            virtual void update() = 0;

            /**
            * 
             */
            virtual void remove_timers() = 0;
            
    };
}

#endif
