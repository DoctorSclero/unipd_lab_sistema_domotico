#ifndef DOMOTIC_DEVICES_HOME_H
#define DOMOTIC_DEVICES_HOME_H

#include "device.h"

#include <vector>
#include <memory>
#include <fstream>

namespace domoticdevices {

    class Device;
    
    /**
     * Provides power and device management and maintains
     * a logger instance for house event logging detaining
     * full responsibility.
     */
    class Home {

        private:

            /**
             * Helper class to manage the logging of house
             * events
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
                    void log(const std::string message);
            };

            std::vector<Device*> devices_;
            int current_time_;
            double network_power_;
            double current_load_;
            Logger logger_;

        public:

            /***************************************************
             * Constructor
             ***************************************************/

            Home(const double network_power, const char* logfile_path)
            : network_power_{network_power}, logger_{logfile_path, this} {}

            /***************************************************
             * Getters
             ***************************************************/

            /**
             * Retrieves the current time
             * @returns The current time
             */
            int get_time() const;

            /**
             * Retrieves a logger reference for
             * logging in the file and console.
             * @returns A reference to the logger
             */
            Logger& get_logger(); // ? Removed const because prevents file logging

            /****************************************************
             * Command interface
             ****************************************************/

            /**
             * Starts a device by its device name
             * @param device_name The name of the device to start
             * @throws `invalid_argument` if device is not present
             */
            void start_device(const std::string device_name);

            /**
             * Stops a device by its device name
             * @param device_name The name of the device to stop
             * @throws `invalid_argument` if device is not present
             */
            void stop_device(const std::string device_name);

            /**
             * Sets the new current time. Iterates through each minute
             * and informs devices of the passed time.
             * @param time The new target time
             */
            void set_time(const int time);

            /**
             * Sets the start timer for the specified device
             * identified by its name.
             * @param device_name The name of the device to set the start timer to
             * @throws `invalid_argument` if device is not present
             */
            void set_start(const int time, const std::string device_name);

            /**
             * Sets the stop timer for the specified device identified
             * by its name.
             * @param device_name The name of the device to set the stop timer to
             * @throws `invalid_argument` if device is not present
             */
            void set_stop(const int time, const std::string device_name);

            /**
             * Logs the power consumtion of all the devices
             */
            void show(); // ? removed const because prevents file logging

            /**
             * Logs the power consumption of the specified device
             * @param device_name The name of the device
             * @throws `invalid_argument` if device is not present
             */
            void show(const std::string device_name);

            /***************************************
             * Observer methods
             ***************************************/

            /**
             * Connects a device to the house network. Subscribed
             * device recieve a notification on every time update
             * @param device The device to add to the house
             */
            void subscribe(Device& device);

            /**
             * Used by devices to update the current power
             * load of the home.
             * @param consumption_delta The variation in consumption [+ more load, - less load]
             */
            void update(const double consumption_delta);
    };

}

#endif
