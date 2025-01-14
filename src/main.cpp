/**
 * @author Elisa Chiarel
 * @matricola 2112891
 */

#include <iostream>
#include <sstream>
#include <vector>
#include <string>

#include "home.h"
#include "manualdevice.h"
#include "cycledevice.h"
#include "exception.h"

/**
 * Tokenizes a string splitting it by spaces,
 * tokens enclosed by "" are treated as a single token
 * @param string The string to tokenize
 */
std::vector<std::string> string_tokenizer(const std::string& string) {
    std::vector<std::string> string_array;
    std::string token;
    auto cursor = string.begin();
    bool inside_quotes = false;

    while (cursor != string.end()) {
        if (*cursor == '"') {
            inside_quotes = !inside_quotes;
            if (!inside_quotes) {
                string_array.push_back(token);
                token.clear();
            }
        } else if (*cursor == ' ' && !inside_quotes) {
            if (!token.empty()) {
                string_array.push_back(token);
                token.clear();
            }
        } else {
            token.push_back(*cursor);
        }
        ++cursor;
    }

    if (!token.empty()) {
        string_array.push_back(token);
    }

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

    // Parsing
    while(std::getline(ss, token, ':')){
        tokens.push_back(token);
    }

    // Attempting conversion
    try {
        int hours = std::stoi(tokens.at(0));
        int minutes = std::stoi(tokens.at(1));
        if(hours < 0 || hours > 23 || minutes < 0 || minutes > 59){
            throw domoticdevices::bad_time_format();
        }
        return hours * 60 + minutes;    
    } catch (std::out_of_range oor) {
        // Conversion failed
        throw domoticdevices::bad_time_format();
    }
}

void print_help() {
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

int main(){
    // Home setup
    constexpr double MAX_HOME_POWER = 0;
    const char* LOGFILE_PATH = "home.log";

    domoticdevices::Home home(MAX_HOME_POWER, LOGFILE_PATH);

    domoticdevices::ManualDevice impianto_fotovoltaico ("Impianto fotovoltaico", 1.5, true);
    domoticdevices::CycleDevice lavatrice ("Lavatrice", -2, false, 110);
    domoticdevices::CycleDevice lavastoviglie ("Lavastoviglie", -1.5, false, 195);
    domoticdevices::ManualDevice pompa_calore ("Pompa di calore con termostato", -2, true);
    domoticdevices::CycleDevice tapparelle ("Tapparelle", -0.3, false, 1);
    domoticdevices::ManualDevice scaldabagno ("Scaldabagno", -1, false);
    domoticdevices::ManualDevice frigorifero ("Frigorifero", -0.4, true);
    domoticdevices::CycleDevice microonde ("Microonde", -0.8, false, 2);
    domoticdevices::CycleDevice asciugatrice ("Asciugatrice", -0.5, false, 60);
    domoticdevices::CycleDevice televisione ("Televisione", -0.2, false, 60);

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

    // Asking and interpreting commands until CTRL-C or 23:59 is reached
    while (home.get_time() < domoticdevices::Home::MINUTES_IN_DAY) {

        // Reading the command from standard input
        std::string command;
        std::cout << "Inserisci il comando: ";
        std::getline(std::cin, command);

        // Parsing the command
        std::vector<std::string> tokenized_command = string_tokenizer(command);

        try {
            if (tokenized_command.size() > 0) {
                // Determining action
                if (tokenized_command.at(0) == "set") {
                    if (tokenized_command.size() == 4) {
                        // set <device_name> <start_time> [stop_time]
                        home.set_timer(
                            to_time(tokenized_command.at(2)), 
                            to_time(tokenized_command.at(3)),
                            tokenized_command.at(1)
                        );
                    } else if (tokenized_command.size() == 3) {
                        if (tokenized_command.at(1) == "time") {
                            home.set_time(to_time(tokenized_command.at(2)));
                        } else {
                            // set <device_name> on
                            if (tokenized_command.at(2) == "on") home.start_device(tokenized_command.at(1));
                            // set <device_name> off
                            else if (tokenized_command.at(2) == "off") home.stop_device(tokenized_command.at(1));
                            // set <device_name> <start_time>
                            else home.set_timer(to_time(tokenized_command.at(2)), tokenized_command.at(1));
                        }
                    } else {
                        std::cout << "Sistema domotico: comando set" << std::endl
                            << " - set <device_name> on: Accensione dispositivo" << std::endl
                            << " - set <device_name> off: Spegnimento dispositivo" << std::endl
                            << " - set <device_name> <(start_timer)hh:mm> [(stop_timer)hh:mm]: Impostazione timer accensione + spegnimento (solo dispositivi manuali)" << std::endl
                            << " - set time <(time)hh:mm>: Porta il sistema a una specifica ora aggiornando i dispositivi minuto per minuto" << std::endl;
                    }
                } else if (tokenized_command.at(0) == "reset") {
                    // reset time
                    if (tokenized_command.at(1) == "time") home.reset_time();
                    // reset timers
                    else if (tokenized_command.at(1) == "timers") home.reset_timers();
                    // reset all
                    else if (tokenized_command.at(1) == "all") home.reset_all();
                    // wrong command
                    else std::cout << "sintassi: reset <time|timers|all>" << std::endl;
                } else if (tokenized_command.at(0) == "show") {
                    // show <device_name>
                    if (tokenized_command.size() == 2) home.show(tokenized_command.at(1));
                    // show
                    else if (tokenized_command.size() == 1) home.show();
                    // wrong command
                    else std::cout << "sintassi: show [device_name]" << std::endl;
                } else if (tokenized_command.at(0) == "rm") {
                    // rm <device_name>
                    home.reset_timer(tokenized_command.at(1));
                } else {
                    print_help();
                }
            } else {
                print_help();
            }

        } catch (const domoticdevices::bad_cycle_duration& bcd) {
            std::cerr << bcd.what() << std::endl;
        } catch (const domoticdevices::bad_device_type& bdt) {
            std::cerr << bdt.what() << std::endl;
        } catch (const domoticdevices::bad_time_format& btf) {
            std::cerr << btf.what() << std::endl;
        } catch (const domoticdevices::bad_time_range& btr) {
            std::cerr << btr.what() << std::endl;
        } catch (const domoticdevices::device_already_present& dap) {
            std::cerr << dap.what() << std::endl;
        } catch (const domoticdevices::device_not_found& dnf) {
            std::cerr << dnf.what() << std::endl;
        } catch (const domoticdevices::device_not_subscribed& dns) {
            std::cerr << dns.what() << std::endl;
        }
    }
    
    return 0;
}