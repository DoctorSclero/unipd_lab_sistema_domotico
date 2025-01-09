#include "home.h"

#include <iostream>
#include <sstream>

namespace domoticdevices {

    Home::Logger::Logger(const char* file_path, const Home* home)
    : home_{home}, file_{file_path} {
        if (!this->file_.is_open()) {
            throw std::runtime_error("Cannot open log file");
        }
        
    };

    Home::Logger::~Logger() {
        if (this->file_.is_open()) {
            this->file_.close();
        }
    }

    void Home::Logger::log(const std::string message) const {
        std::stringstream ss;
        
        // Time formatting [minutes since 00:00] -> hh:mm
        int current_time = this->home_->get_time();
        int current_hour = current_time / 60;
        int current_min = current_time % 60;
        
        ss << "[" << current_hour << ":" << current_min << "] ";
        ss << message;
        
        std::cout << ss.str();
        //this->file_ << ss.str();
    }
}