/**
 * @author Elisa Chiarel
 * @matricola 2112891
 */

#ifndef DOMOTIC_DEVICES_UTILS_H
#define DOMOTIC_DEVICES_UTILS_H

#include <string>

namespace domoticdevices {

    /**
     * Converts a minutes expressed time into
     * a string formatted time hh:mm
     * @param time The time in minutes
     * @return A string in the format hh:mm
     */
    std::string timetostr(const int time);
    
}

#endif