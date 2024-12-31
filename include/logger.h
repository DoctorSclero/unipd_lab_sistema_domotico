#ifndef DOMOTIC_DEVICES_LOGGER
#define DOMOTIC_DEVICES_LOGGER

#include <fstream>
#include "home.h";

namespace domoticdevices {
    class Logger {
        private:
            Home home;
            std::ofstream file;
        public:
    };
}
#endif