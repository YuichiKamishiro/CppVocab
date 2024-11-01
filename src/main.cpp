#include <myLib.hpp>

int main() {
    initDifficulty();

    std::string input;
    std::cout << "CppVocabulary Usage:\n";
    std::cout << "Commands:\n /c <1, 2 or 3> Change difficulty\n";
    std::cout << " /exit Exit the program\n";
    std::cout << " Press enter to learn next word\n";

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
