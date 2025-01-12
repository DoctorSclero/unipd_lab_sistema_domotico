#include <iostream>
#include <sstream>
#include <vector>
#include <string>

#include "home.h"
#include "manualdevice.h"
#include "cycledevice.h"

/**
 * Tokenizes a string splitting it 
 */
std::vector<std::string> to_array_string(const std::string& string, char divisor) {
    std::vector<std::string> string_array;
    std::stringstream stream(string);
    std::string token;

    while(std::getline(stream, token, divisor)) {
        string_array.push_back(token);
    }
    return string_array;
}

int main(){
    // Home setup
    constexpr double MAX_HOME_POWER = 3.5;
    constexpr char* LOGFILE_PATH = "#";

    domoticdevices::Home home(MAX_HOME_POWER, LOGFILE_PATH);

    domoticdevices::ManualDevice impianto_fotovoltaico ("Impianto fotovoltaico", 1.5, -1);
    domoticdevices::CycleDevice lavatrice ("Lavatrice", -2, 0, 110);
    domoticdevices::CycleDevice lavastoviglie ("Lavastoviglie", -1.5, 0, 195);
    domoticdevices::ManualDevice pompa_calore ("Pompa di calore", -2, 0);
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
        std::cin >> command;

        // Parsing the command
        char divisor = ' ';
        std::vector<std::string> tokenized_command = to_array_string(command, divisor);

        // Determing action
        if (tokenized_command[0] == "set") {
            if (tokenized_command[1] == "time") {
                try {
                    int time = std::stoi(tokenized_command[2]);
                    home.set_time(time);
                } catch (std::invalid_argument ia) {
                    std::cout << "sintassi: set time <time>";
                }
            } else {
                if (tokenized_command[2] == "on") {
                    home.start_device(tokenized_command[1]);
                } else if (tokenized_command[2] == "off") {
                    home.stop_device(tokenized_command[1]);
                } else {
                    try {
                        int start_time = std::stoi(tokenized_command[2]);
                        home.set_start_timer(start_time, tokenized_command[1]);
                        if (tokenized_command.size() == 4) {
                            int stop_time = std::stoi(tokenized_command[2]);
                            home.set_stop_timer(stop_time, tokenized_command[1]);
                        }
                    } catch (std::invalid_argument ia) {
                        std::cout << "sintassi: set <device_name> <on|off|<start_time> [stop_time]>";
                    }
                }
            }
        } else if (tokenized_command[0] == "reset") {
            if (tokenized_command[1] == "time") {
                home.reset_time();
            } else if (tokenized_command[1] == "timers") {
                home.reset_timers();
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