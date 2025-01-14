/**
 * @author Elisa Chiarel
 * @matricola 2112891
 */
#ifndef DOMOTIC_DEVICES_EXCEPTIONS_H
#define DOMOTIC_DEVICES_EXCEPTIONS_H

#include <stdexcept>
#include <string>

namespace domoticdevices {
    
    struct bad_time_format : public std::invalid_argument {
        bad_time_format() 
        : std::invalid_argument{"Formato del tempo non valido [hh:mm]"} {}
    };

    struct bad_time_range : public std::invalid_argument {
        // Not the best but good enough
        bad_time_range(std::string min_time, std::string max_time)
        : std::invalid_argument {"Il tempo deve essere compreso tra " + min_time + " e " + max_time} {}
    };

    struct bad_cycle_duration : public std::invalid_argument {
        bad_cycle_duration()
        : std::invalid_argument{"La durata del ciclo deve essere maggiore di 0"} {}
    };

    struct device_not_found : public std::invalid_argument {
        device_not_found(std::string device_name) 
        : std::invalid_argument{"Il dispositivo " + device_name + " non e' stato trovato"} {}
    };

    struct bad_device_type: public std::invalid_argument {
        bad_device_type(std::string message)
        : std::invalid_argument{message} {}
    };
    struct device_already_present : public std::invalid_argument {
        device_already_present(std::string device_name)
        : std::invalid_argument("Il dispositivo " + device_name + " e' gia' presente") {}
    };

    struct device_not_subscribed : public std::runtime_error {
        device_not_subscribed(std::string device_name) 
        : std::runtime_error{"Il dispositivo " + device_name + " non e' stato assegnato ad una casa"} {}
    };

}

#endif