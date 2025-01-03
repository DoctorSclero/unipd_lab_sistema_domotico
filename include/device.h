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
            static int id_counter;
            /**
            * this is used to assign 
            */
            static int priority_counter;
            int ID;
            std::string name;
            double power; //both positive or negative
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
            int priority;
            int start_time;
            bool running;
            Home home;
            
            Device(std::string n, double p, int pr)
            : ID{id_counter++}, running{false}, name{n}, priority{pr}, start_time{-1}, power{p} {}
        public:
            Device(const Device&) = delete;
            Device& operator=(const Device&) = delete;
            
            bool operator<(Device& other_device) const;
            std::string get_name() const;
            int get_ID() const;
            double get_power() const;
            void set_start_time(int time);
            void set_home(Home& h);
            void start();
            void stop();
            
            virtual void update() = 0;
    };
}

#endif
