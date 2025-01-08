#include "logger.h"

#include <iostream>
#include <sstream>

namespace domoticdevices {

    Logger::Logger(const char* file_path, const Home* home)
    : home_{home}, file_{file_path} {
        if (!this->file_.is_open()) {
            throw std::runtime_error("Cannot open log file");
        }
    };

    Logger::~Logger() {
        if (this->file_.is_open()) {
            this->file_.close();
        }
    }

    void Logger::log(const std::string message) {
        std::stringstream ss;
        int current_time = this->home_->get_time();
        int current_hour = current_time / 60;
        int current_min = current_time % 60;
        
        ss << "[" << current_hour << ":" << current_min << "] ";
        ss << message;

        std::cout << ss.str();
        this->file_ << ss.str();
    }
}