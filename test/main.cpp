#include <iostream>
#include <string>
#include <vector>

/**
 * Tokenizes a string splitting it by spaces,
 * strings encolosed by "" are treated as single tokens
 * @param string The string to tokenize
 */
std::vector<std::string> string_tokenizer(const std::string& string) {
    std::vector<std::string> string_array;

    std::string token;
    auto cursor = string.begin();
    while(cursor != string.end()) {
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

int main() {
    std::string command;
    std::cout << "Inserisci comando: ";
    std::getline(std::cin, command);
    std::cout << command << std::endl;

    std::vector<std::string> command_array = string_tokenizer(command);

    for (std::string& word : command_array) {
        std::cout << word << std::endl;
    }

    return 0;
}