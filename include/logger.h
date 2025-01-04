#ifndef DOMOTIC_DEVICES_LOGGER
#define DOMOTIC_DEVICES_LOGGER

#include <fstream>
#include "home.h";

namespace domoticdevices {
    /**
     * Manages the logging of events of a house
     */
    class Logger {
        private:
            Home* p_home;
            std::ofstream p_file;
        public:
            /**
             * Logger constructor
             * @param file_path The log file path
             * @param home A reference to the home for time retrival
             */
            Logger(std::string file_path, Home& home)
            : p_home{&home}, p_file{file_path} {};

            /**
             * Loggs an event by writing it in the console
             * and on the specified log file.
             * @param message The message to print
             */
            void log(std::string message);
    };
}
#endif