#ifndef DOMOTIC_DEVICES_LOGGER
#define DOMOTIC_DEVICES_LOGGER

#include <fstream>
#include "home.h"

namespace domoticdevices {
    class Home;
    /**
     * Manages the logging of events of a house
     */
    class Logger {
        private:
            const Home* home_;
            std::ofstream file_;
        public:
            /**
             * Logger constructor
             * @param file_path The log file path
             * @param home A reference to the home for time retrival
             * @thorws `runtime_error` if the log file cannot be opened
             */
            Logger(const char* file_path, const Home* home);

            /**
             * Logger destructor, closes file stream
             */
            ~Logger();

            /**
             * Loggs an event by writing it in the console
             * and on the specified log file.
             * @param message The message to print
             */
            void log(const std::string message) const;
    };
}
#endif