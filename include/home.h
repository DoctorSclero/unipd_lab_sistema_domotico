#ifndef DOMOTIC_DEVICES_HOME
#define DOMOTIC_DEVICES_HOME

#include <vector>
#include "device.h"
#include "logger.h"

namespace domoticdevices {
    
    /**
     * Home class:
     * provides power and device management and maintains
     * a logger instance for house event logging detaining
     * full responsibility.
     */
    class Home {
        private:
            std::vector<Device> devices_;
            int current_time_;
            double network_power_;
            Logger logger_;
        public:
            // Constructors
            Home(double network_power, std::string logfile_path)
            : network_power_{network_power}, logger_{logfile_path, *this} {}
            // Getters
            int get_time();
            // ! Possible attributes integrity problems
            Logger get_logger();
            // Command interface
            void start_device(std::string device_name);
            void stop_device(std::string device_name);
            void set_time(int time);
            void set_start(int time, std::string device_name);
            void set_stop(int time, std::string device_name);
            void show();
            void show(std::string device_name);
            // Observer pattern
            void subscribe(Device device);
    };

}

#endif
