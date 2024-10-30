#include <myLib.hpp>

int main() {
    initDifficulty();

    std::string input;
    std::cout << "Hello message\n";

    while(true) {
        getline(std::cin, input);
        std::stringstream ss(input);
        std::string command;

        if (input.empty()) {
            getRandWord();
        } else if (ss >> command; command == "/exit") {
            break;
        } else if(command == "/c") {
            int arg;
            ss >> arg;

            std::ifstream ifstr(PATH_SETTINGS);
            nlohmann::json j;
            ifstr >> j;

            if (arg >= 1 && arg <= 3) {
                std::ofstream ofstr(PATH_SETTINGS);
                j["difficulty"] = arg;
                ofstr << j.dump(4);
                std::cout << "Difficulty changed on " << arg << "\n";
            } else {
                std::cout << "No matching arguments. Try again!\n";
            }
        } else {
            std::cout << "Try again.\n";
        }
    }
}
