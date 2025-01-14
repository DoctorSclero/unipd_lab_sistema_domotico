/**
 * @author Elisa Chiarel
 * @matricola 2112891
 */
#include "utils.h"

#include <sstream>
#include <iomanip>

namespace domoticdevices {
    std::string timetostr(const int time) {
        // Usa stringstream per formattare il risultato in hh:mm
        std::ostringstream ss;
        ss << std::setw(2) << std::setfill('0') << time / 60 << ":"
            << std::setw(2) << std::setfill('0') << time % 60;
        return ss.str();
    }
}