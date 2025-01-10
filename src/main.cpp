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
    // Home setup
    constexpr double MAX_HOME_POWER = 3.5;
    constexpr char* LOGFILE_PATH = "#";

    domoticdevices::Home home(MAX_HOME_POWER, LOGFILE_PATH);

    // Asking and interpreting commands until CTRL-C
    while (true) {

        // Reading the command from standard input
        std::string command;
        std::cout << "Inserisci il comando: ";
        std::cin >> command;

        // Parsing the command
        char divisor = ' ';
        std::vector<std::string> tokenized_command = to_array_string(command, divisor);

        // ? What policy for more arguments than needed ?
        // Determing action
        if (tokenized_command[0] == "set") {
            // Da valutare
            if (tokenized_command.size() >= 3 /* && tokenized_command.size() <= 4 */) {
                if (tokenized_command[1] == "time") {
                    try {
                        int time = std::stoi(tokenized_command[2]);
                        home.set_time(time);
                    } catch (std::invalid_argument ia) {

                    }
                }
                if (tokenized_command.size() == 4) {
                    try {
                        int start_time = std::stoi(tokenized_command[2]);
                        int stop_time = std::stoi(tokenized_command[3]);
                        home.set_start(start_time, tokenized_command[1]);
                        home.set_stop(stop_time, tokenized_command[1]);
                    } catch (std::invalid_argument ia) {

                    }
                }
                if (tokenized_command.size() == 3) {
                    if (tokenized_command[2] == "on") {
                        home.start_device(tokenized_command[1]);
                    } else if (tokenized_command[2] == "off") {
                        home.stop_device(tokenized_command[1]);
                    }
                }
            } else {
                // ! Bruttino, al momento non mi viene in mente altro.
                std::cout << "sintassi: set <time <time>|<start [stop]>>" << std::endl;
            }
        } else if (tokenized_command[0] == "reset") {
            if (tokenized_command[1] == "time") {
                // TODO: implement member function "reset_time"
            } else if (tokenized_command[1] == "timers") {
                //TODO: implement member function "reset_timers"
            } else if (tokenized_command[1] == "all") {
                home.reset_all();
            } else {
                std::cout << "sintassi: reset <time|timers|all>" << std::endl;
            }
        } else if (tokenized_command[0] == "show") {
            if (tokenized_command.size() == 2) {
                home.show(tokenized_command[1]);
            } else if (tokenized_command.size() == 1) {
                home.show();
            } else {
                std::cout << "sintassi: show [device_name]" << std::endl;
            }
        } else if (tokenized_command[0] == "rm") {
            home.reset_timer(tokenized_command[1]);
        } else {
            std::cout << "Sistema di gestione casa domotica: " << std::endl
                    << " - set <device_name> on: Accensione dispositivo" << std::endl
                    << " - set <device_name> off: Spegnimento dispositivo" << std::endl
                    << " - set <device_name> <start> [stop]: Impostazione timer accensione + spegnimento (solo dispositivi manuali)" << std::endl
                    << " - set time <time>: Porta il sistema a una specifica ora aggiornando i dispositivi minuto per minuto" << std::endl
                    << " - rm <device_name>: Rimozione timer dispositivo" << std::endl
                    << " - show: Mostra i bilanci energetici della casa e dei dispositivi" << std::endl
                    << " - show <device_name>: Mostra l'impatto energetico di un dispositivo" << std::endl
                    << " - reset time: Ripristina l'orario del sistema e lo stato dei dispositivi (mantiene i timer)" << std::endl
                    << " - reset timers: Rimuove i timer da tutti i dispositivi" << std::endl
                    << " - reset all: Ripristina l'orario del sistema e lo stato dei dispositivi (rimuove i timer)" << std::endl << std::endl;
        }
    }
    
    return 0;
} 