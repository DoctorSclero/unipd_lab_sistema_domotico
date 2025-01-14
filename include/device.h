/**
 * @author Diego Chiesurin
 * @matricola 2111553
 */

#ifndef DOMOTIC_DEVICES_DEVICE_H
#define DOMOTIC_DEVICES_DEVICE_H

#include <string>
#include "home.h"

namespace domoticdevices {
    class Home;
    
    class Device {
        protected:
            /**
             * Used to assign a unique id to 
             * every new object of class Device 
             * that is created
             * Increases by one every time a new Device is created
             */
            static int id_counter_;

            int id_;
            std::string name_;

            /**
             * - > 0 if the device produces power
             * - < 0 if the device consumes power
             */
            double power_;

            /**
             * Determines the shutdown order when the house 
             * exceeds its power limit and the automatic power off is activated.
             * Device are shut following this criteria:
             * - -1: Device should be kept on as much as possible
             * - 0: Device is already turned off
             * - > 0: Device is turned on, the Device with the highest priority is the first to be shut down
             */
            int priority_;
            int start_time_;
            int start_timer_;
            bool running_;
            double total_energy_;
            Home* home_;
            
            /**
             * @param name The name of the device
             * @param power The power of the device (positive for producers, negative for consumers)
             * @param keep_on True if the device should be kept on when auto power off is activated, false otherwise
             */

            Device(const std::string name, const double power, const bool keep_on)
            :   id_{id_counter_++}, 
                running_{false}, 
                name_{name}, 
                start_time_{-1}, 
                start_timer_{-1}, 
                power_{power}, 
                total_energy_{0.0}, 
                priority_{keep_on ? -1 : 0} 
            {}

        public:
            /**
             * Preventing slicing
             */
            Device(const Device&) = delete;
            Device& operator=(const Device&) = delete;

            /**
             * sets the start timers only if the new timer 
             * is greater than the current time and less than or equal to MINUTES_IN_DAY
             * @param start_timer The new start timer to be set
             * @throws bad_time_range, device_not_subscribed
             */
            void set_timer(const int start_timer);

            /**
             * Decresease priority by 1
             * Function used from home to decrement the priority
             * (usually when a different device stops)
             */
            void decrease_priority();

            /**
             * House observer, subscribes an house
             * to handle device status changes
             * (es. device turning on notifications)
             * @param h The home to subscribe
             */
            void subscribe(Home& h);

            /**
             * Retrieves the name of the device
             * @return The name of the device
             */
            std::string get_name() const;

            /**
             * Retrieves the priority of the device
             * @return The priority of the device
             */
            int get_priority() const;

            /**
             * Retrieves the power of the device
             * @return The power of the device
             */
            double get_power() const;

            /**
             * Retrieves the total energy generated/consumed
             * @return The total energy
             */
            double get_total_energy() const;

            /**
             * Retrieves the running state of the Device
             * @return True if the Device is running, false otherwise
             */
            bool is_running() const;

            /**
             * Defines the order between devices based on their priority
             * this < other_device if this.priority_ < other_device.priority_
             * @param other_device The other Device to compare
             * @return true if this < other_device, false otherwise
             */
            bool operator<(const Device& other_device) const;

            /**
             * checks equality between two devices based on their ids
             * @param other_device the other Device wihich is being confronted
             * @return true if the devices have the same id, false otherwise
             */
            bool operator==(const Device& other_device) const;

            /**
             * checks inequality between two devices based on their ids
             * @param other_device the other Device wihich is being confronted
             * @return true if the devices have different id, false otherwise
             */
            bool operator!=(const Device& other_device) const;

            /**
             * checks equality between a device's name and a string
             * @param other_name the string which is supposed to contain a name of a device
             * @return true if the device's name is the same as other_name, false otherwise
             */
            bool operator==(const std::string other_name) const;

            /**
             * starts the device if it isn't already running
             * and updates the home about the status change
             * @throws device_not_subscribed
             */
            void start();

            /**
             * Force stops the device and logs the event.
             * This is necessary for home to implement
             * the automatic shut down logic
             * @throws device_not_subscribed
             */
            void force_stop();

            /**
             * Stops the device only if it is running
             * Calls force_stop() to reuse the code 
             * Updates the home about the status change
             * @throws device_not_subscribed
             */
            void stop();

            /**
             * Resets the device to its inital state and
             * gets turned off.
             * the function stop() is called and
             * total_energy_ is set to 0
             */
            void reset();

            /**
             * Prints information about the device including
             * the total energy consumed/generated in kWh, from 00:00 to the current time
             */
            std::string to_string() const;

            /**
             * removes that start_timer_ by setting it 
             * at -1
             */
            virtual void remove_timers();

            /**
             * Used by the home for notifying time updates.
             * Let devices handle the critieria to start 
             * and stop on their own
             */
            virtual void update() = 0;
    };
}

#endif
