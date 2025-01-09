#include <iostream>
#include <sstream>
#include <vector>
#include <string>
#include "home.h"


std::vector<std::string> to_array_string(const std::string& string, char divisor) {
    std::vector<std::string> string_array;
    std::stringstream stream(string);
    std::string token;

    while(std::getline(stream, token, divisor)) {
        string_array.push_back(token);
    }
    return string_array;
}

/*int string_to_int(std::string string){
    int str = std::stoi(string);
}*/



int main(){
    constexpr double MAX_HOME_POWER = 3.5;
    constexpr char* LOGFILE_PATH = "#";
    std::string command;
    std::cout << "Inserisci il comando:";
    std::cin >> command;
    char divisor = ' ';
    std::vector<std::string> tokenized_command = to_array_string(command, divisor);
    domoticdevices::Home home(MAX_HOME_POWER, LOGFILE_PATH);

    /*for(const auto& s : result) {
        std:: cout<<s<<std::endl;
    }*/

    while (tokenized_command[0] == "set" || tokenized_command[0] == "reset" || tokenized_command[0] == "show") {
        if (tokenized_command[0] == "set") {
            if (tokenized_command.size() >= 3 && tokenized_command.size() <= 4) {
                if (tokenized_command[1] == "time") {
                    try {
                        int time = std::stoi(tokenized_command[2]);
                        //chiamo set time
                        home.set_time(time);
                    }
                    catch (std::invalid_argument ia) {}
                }
                if (tokenized_command.size() == 4) {
                    try {
                        int start_time = std::stoi(tokenized_command[2]);
                        int stop_time = std::stoi(tokenized_command[3]);
                        home.set_start(start_time, tokenized_command[1]);
                        home.set_stop(stop_time, tokenized_command[1]);
                        // chiami home->set_start(start_time, tokenized_command[1]);
                        //MA DEVO CONTROLLARE CHE TOKENIZED_COMMAND[1] SIA UNA STRINGA 
                        //CHE CORRISPONDA AL NOME DI UNO DEI DISPOSITIVI!
                    }
                    catch (std::invalid_argument ia) {}
                }
                if (tokenized_command.size() == 3) {
                    if (tokenized_command[2] == "on") {
                        home.start_device(device_name);
                    }
                    else if (tokenized_command[2] == "off") {
                        home.stop_device(device_name);
                    }
                }
            }
        }

        //controllo i comandi reset
        if (tokenized_command[0] == "reset") {
            if (tokenized_command[1] == "time") {
                home.set_time(0);
            }
            else if (tokenized_command[1] == "timers") {
                home.set_start(0,device_name);
            }
            else if (tokenized_command[1] == "all") {
                home.reset_all();
            }
        }

        //controllo i comandi show <3 i<3u
        if (tokenized_command[0] == "show") {
            if (tokenized_command.size() == 2) {
                if (tokenized_command[1] == "") {
                    //mostro le caratteristiche del device
                }
            } else if (tokenized_command.size() == 1) {
                //mostro l'elenco dei devices
            }
        }
    }
    //eccezione per comando non valido, o inesistente
    // TODO: too many/few arguments
    std::out_of_range;
    
    
    
    return 0;
} 