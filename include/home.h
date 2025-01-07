#ifndef DOMOTIC_DEVICES_HOME
#define DOMOTIC_DEVICES_HOME

#include <vector>
#include <memory>
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
            std::vector<Device*> devices_;
            int current_time_;
            double network_power_;
            Logger* logger_;
        public:
            // Constructors
            Home(const double network_power, const char* logfile_path)
            : network_power_{network_power}, logger_{&Logger{logfile_path, this}} {}
            // Getters
            int get_time() const;
            // ! Possible attributes integrity problems
            Logger* get_logger() const;
            // Command interface
            void start_device(const std::string device_name);
            void stop_device(const std::string device_name);
            void set_time(const int time);
            void set_start(const int time, const std::string device_name);
            void set_stop(const int time, const std::string device_name);
            void show() const;
            void show(const std::string device_name) const;
            // Observer pattern
            void subscribe(Device& device);
    };

}

#endif
