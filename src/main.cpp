/**
 * @author Elisa Chiarel
 * @matricola
 */

#include <iostream>
#include <sstream>
#include <vector>
#include <string>

#include "home.h"
#include "manualdevice.h"
#include "cycledevice.h"

/**
 * Tokenizes a string splitting it by spaces,
 * tokens encolosed by "" are treated as a single token
 * @param string The string to tokenize
 */
std::vector<std::string> string_tokenizer(const std::string& string) {
    std::vector<std::string> string_array;

    std::string token;
    auto cursor = string.begin();
    while(cursor != string.end()) {
        //counts the characters between "" as a single token
        if (*cursor == '"') {
            while (*(++cursor) != '"' && cursor != string.end()) token.push_back(*cursor);
        } else if (*cursor == ' ') {
            string_array.push_back(token);
            token.clear();
        } else {
            token.push_back(*cursor);
        }
        cursor++;
    }
    string_array.push_back(token);

    return string_array;
}

/**
 * Converts a hh::mm formatted time
 * to the number of minutes from 00:00
 * @param str The string containing the time in hh:mm format
 * @return the number of minutes from 00:00 after conversion
 */
int to_time(std::string& str){
    std::stringstream ss(str);
    std::string token;
    std::vector<std::string> tokens;
    while (std::getline(ss, token, ':')) {
        tokens.push_back(token);
    }
    if (tokens.size() != 2) {
        throw std::invalid_argument("Il tempo non e' nel formato corretto");
    }
    return std::stoi(tokens.at(0)) * 60 + std::stoi(tokens.at(1));
}

int main(){
    // Home setup
    constexpr double MAX_HOME_POWER = 3.5;
    const char* LOGFILE_PATH = "home.log";

    domoticdevices::Home home(MAX_HOME_POWER, LOGFILE_PATH);

    domoticdevices::ManualDevice impianto_fotovoltaico ("Impianto fotovoltaico", 1.5, -1);
    domoticdevices::CycleDevice lavatrice ("Lavatrice", -2, 0, 110);
    domoticdevices::CycleDevice lavastoviglie ("Lavastoviglie", -1.5, 0, 195);
    domoticdevices::ManualDevice pompa_calore ("Pompa di calore con termostato", -2, 0);
    domoticdevices::CycleDevice tapparelle ("Tapparelle", -0.3, 0, 1);
    domoticdevices::ManualDevice scaldabagno ("Scaldabagno", -1, 0);
    domoticdevices::ManualDevice frigorifero ("Frigorifero", -0.4, -1);
    domoticdevices::CycleDevice microonde ("Microonde", -0.8, 0, 2);
    domoticdevices::CycleDevice asciugatrice ("Asciugatrice", -0.5, 0, 60);
    domoticdevices::CycleDevice televisione ("Televisione", -0.2, 0, 60);

    // Connecting devices to the house
    home.subscribe(impianto_fotovoltaico);
    home.subscribe(lavatrice);
    home.subscribe(lavastoviglie);
    home.subscribe(pompa_calore);
    home.subscribe(tapparelle);
    home.subscribe(scaldabagno);
    home.subscribe(frigorifero);
    home.subscribe(microonde);
    home.subscribe(asciugatrice);
    home.subscribe(televisione);

    // Asking and interpreting commands until CTRL-C
    while (true) {

        // Reading the command from standard input
        std::string command;
        std::cout << "Inserisci il comando: ";
        std::getline(std::cin, command);

        // Parsing the command
        std::vector<std::string> tokenized_command = string_tokenizer(command);

        // Determining action
        if (tokenized_command.at(0) == "set") {
            if (tokenized_command.at(1) == "time") {
                try {
                    home.set_time(to_time(tokenized_command.at(2)));
                } catch (const std::invalid_argument& ia) {
                    // Bad time format
                    std::cout << "sintassi: set time <(time)hh:mm>" << std::endl;
                }
            } else {
                if (tokenized_command.at(2) == "on") {
                    try {
                        home.start_device(tokenized_command.at(1));
                    } catch (const std::invalid_argument& ia) {
                        // Device not found
                        std::cerr << ia.what() << std::endl;
                    }
                } else if (tokenized_command.at(2) == "off") {
                    try {
                        home.stop_device(tokenized_command.at(1));
                    } catch (const std::invalid_argument& ia) {
                        // Device not found
                        std::cerr << ia.what() << std::endl;
                    }
                } else {
                    try {
                        if (tokenized_command.size() == 4) {
                            // Setting both timers if indicated
                            home.set_timers(
                                to_time(tokenized_command.at(2)), 
                                to_time(tokenized_command.at(3)),
                                tokenized_command.at(1)
                            );
                        } else if (tokenized_command.size() == 3) {
                            home.set_start_timer(to_time(tokenized_command.at(2)), tokenized_command.at(1));
                        } else {
                            std::cerr << "sintassi: set <device_name> <on|off|<(start_timer)hh:mm> [(stop_timer)hh:mm]>" << std::endl;
                        }
                    } catch (const std::invalid_argument& ia) {
                        // Bad time formatting
                        std::cerr << ia.what() << std::endl;
                    }
                }
            }
        } else if (tokenized_command.at(0) == "reset") {
            if (tokenized_command.at(1) == "time") {
                home.reset_time();
            } else if (tokenized_command.at(1) == "timers") {
                home.reset_timers();
            } else if (tokenized_command.at(1) == "all") {
                home.reset_all();
            } else {
                std::cout << "sintassi: reset <time|timers|all>" << std::endl;
            }
        } else if (tokenized_command.at(0) == "show") {
            if (tokenized_command.size() == 2) {
                try {
                    home.show(tokenized_command.at(1));
                } catch (const std::invalid_argument& ia) {
                    // Cannot find device
                    std::cerr << ia.what() << std::endl;
                }
            } else if (tokenized_command.size() == 1) {
                home.show();
            } else {
                std::cout << "sintassi: show [device_name]" << std::endl;
            }
        } else if (tokenized_command.at(0) == "rm") {
            try {
                home.reset_timer(tokenized_command.at(1));
            } catch (const std::invalid_argument& ia) {
                // Cannot find device
                std::cerr << ia.what() << std::endl;
            }
        } else {
            std::cout << "Sistema di gestione casa domotica: " << std::endl
                    << " - set <device_name> on: Accensione dispositivo" << std::endl
                    << " - set <device_name> off: Spegnimento dispositivo" << std::endl
                    << " - set <device_name> <(start_timer)hh:mm> [(stop_timer)hh:mm]: Impostazione timer accensione + spegnimento (solo dispositivi manuali)" << std::endl
                    << " - set time <(time)hh:mm>: Porta il sistema a una specifica ora aggiornando i dispositivi minuto per minuto" << std::endl
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