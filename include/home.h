#ifndef DOMOTIC_DEVICES_HOME
#define DOMOTIC_DEVICES_HOME

#include <vector>
#include "device.h"
#include "logger.h"

namespace domoticdevices {
    
    class Home {
        private:
            Logger logger;
            std::vector<Device> devices;
            int current_time;
            double network_power;
        public:
            // Getters
            int get_time();
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
