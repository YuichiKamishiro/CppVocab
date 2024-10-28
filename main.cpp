#include <sstream>
#include <fstream>
#include <random>
#include <iostream>
#include <string>
#include "json.hpp"

#define PATH_DB_1 "../db/words1.csv"
#define PATH_DB_2 "../db/words2.csv"
#define PATH_DB_3 "../db/words3.csv"
#define PATH_SETTINGS "../settings/settings.json"

void matchFileName(int difficulty, std::string &pathToDB) {
    switch (difficulty) {
        case 1:
            pathToDB = PATH_DB_1;
            break;
        case 2:
            pathToDB = PATH_DB_2;
            break;
        case 3:
            pathToDB = PATH_DB_3;
            break;
    }
}

void setDifficulty(std::string &pathToDB) {
    using namespace nlohmann;

    // Transfer all data from settings to json object
    std::fstream fileSettings(PATH_SETTINGS, std::ios::in);
    json j;
    if (fileSettings.is_open()) {
        fileSettings >> j;
        fileSettings.close();
    } else {
        std::cerr << "Error opening " << PATH_SETTINGS << "\n";
        exit(EXIT_FAILURE);
    }
    
    if(j["difficulty"].is_null()) {
        int newDifficulty = 0;
        while (!(newDifficulty >= 1 && newDifficulty <= 3)) {
            std::cout << "Difficulty not set. Please select difficulty 1[easy], 2[medium], 3[hard]\n";
            std::cin >> newDifficulty;
        }

        j["difficulty"] = newDifficulty;
        fileSettings.open(PATH_SETTINGS, std::ios::out | std::ios::trunc);
        fileSettings << j.dump(4);
        fileSettings.close();

        matchFileName(newDifficulty, pathToDB);
    } else {
        int difficulty = j["difficulty"];
        matchFileName(difficulty, pathToDB);
    }
}

// Get all lines from words db
int getLinesCount(std::string pathToFile) {
    std::ifstream ifstr(pathToFile);

    std::string buf;
    int linesCount = 0;

    if(ifstr.is_open()) {
        while(std::getline(ifstr, buf)) {
            linesCount++;
        }
    }
    
    ifstr.close();
    return linesCount;
}

// Get random line from 0..allLines
int getRandLine(std::string pathToFile) {
    std::random_device rd;

    std::mt19937 gen(rd());

    std::uniform_int_distribution<> dis(0, getLinesCount(pathToFile));

    return dis(gen);
}

// This function get random line from all_words.csv and then move it to learned words.

void getRandWord(std::string pathToFile) {
    std::ifstream allWords(pathToFile);
    if (allWords.peek() == std::ifstream::traits_type::eof()) {
        std::cout << "No more words for you on this difficulty\n";
    }

    if (allWords.is_open()) {
        // Get rand number of line
        int lineNumber = getRandLine(pathToFile);

        std::string line;
        int currentLine = 0;

        std::string buffer;        
        // Open learned_words.csv
        std::ofstream learnedWords("../learned_words.csv", std::ios::app);

        while (std::getline(allWords, line)) {
            currentLine++;
            
            if (lineNumber != currentLine) {
                // Add to temp all lines except line that user get
                buffer += line += '\n';
            } else {
                // Write to learned_words.csv word that user got
                learnedWords << line << "\n";
                std::stringstream sstr(line);

                std::string word, description;
                std::getline(sstr, word, ',');
                std::getline(sstr, description, ',');

                std::cout << "word: "  << word << "\ndescr: " << description << "\n";
            }
        }

        std::ofstream ofstrAllWords(pathToFile, std::ios::trunc);
        ofstrAllWords << buffer;
    }
}

int main() {
    std::string pathToDB;
    setDifficulty(pathToDB);
    
    std::string input;

    std::cout << "Hello message\n";
    while(true) {
        getline(std::cin, input);
        std::stringstream ss(input);
        std::string command;

        if (input.empty()) {
            getRandWord(pathToDB);
        } else if (ss >> command; command == "/e") {
            break;
        } else if(command == "/c") {
            int arg;
            ss >> arg; 

            std::ifstream ifstr("../settings.json");
            nlohmann::json j;
            if (ifstr.is_open()) {
                std::cout << "Opened settings\n";
                ifstr >> j;
            } else {
                std::cout << "Error while loading file\n";
            }

            if (arg >= 1 && arg <= 3) {
                std::ofstream ofstr("../settings.json");
                std::cout << "Log1\n";
                j["difficulty"] = arg;
                ofstr << j.dump(4);
            } else {
                std::cout << "No matching arguments. Try again!\n";
            }
        } else {
            std::cout << "Try again.\n";
        }
    }
}