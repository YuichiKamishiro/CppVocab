#include <sstream>
#include <fstream>
#include <random>
#include <iostream>
#include <string>
#include "json.hpp"

#define PATH_DB_1 "../db/words1.csv"
#define PATH_DB_2 "../db/words2.csv"
#define PATH_DB_3 "../db/words3.csv"
#define PATH_DB_LEARNED "../db/learned_words.csv"
#define PATH_SETTINGS "../settings/settings.json"

int getDifficulty() {
    int difficulty = 0;
    std::ifstream ifstr(PATH_SETTINGS);

    if (!ifstr.is_open()) {
        std::cout << "Error while loading " << PATH_SETTINGS << "\n";
        exit(EXIT_FAILURE);
    }

    nlohmann::json j;
    ifstr >> j;

    return j["difficulty"];
}

std::string matchDifficulty() {
    switch(getDifficulty()) {
        case 1:
            return PATH_DB_1;
            break;
        case 2:
            return PATH_DB_2;
            break;
        case 3:
            return PATH_DB_3;
            break;
    }
}

void initDifficulty() {
    using namespace nlohmann;
    
    if(getDifficulty() == 0) {
        int newDifficulty = 0;
        while (!(newDifficulty >= 1 && newDifficulty <= 3)) {
            std::cout << "Difficulty not set. Please select difficulty 1[easy], 2[medium], 3[hard]\n";
            std::cin >> newDifficulty;
        }

        json j;
        std::fstream fileSettings(PATH_SETTINGS, std::ios::out | std::ios::in);
        fileSettings >> j;

        j["difficulty"] = newDifficulty;
        fileSettings << j.dump(4);
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

    std::uniform_int_distribution<> dis(0, getLinesCount(pathToFile) - 1);

    return dis(gen);
}

// This function get random line from all_words.csv and then move it to learned words.

void getRandWord() {
    std::string pathToCurrentDB = matchDifficulty();

    std::ifstream allWords(pathToCurrentDB);
    if (allWords.peek() == std::ifstream::traits_type::eof()) {
        std::cout << "No more words for you on this difficulty\n";
        return;
    }

    if (allWords.is_open()) {
        // Get rand number of line
        int lineNumber = getRandLine(pathToCurrentDB);
        std::string line;
        int currentLine = 0;

        std::string buffer;        
        // Open learned_words.csv
        std::ofstream learnedWords("../db/learned_words.csv", std::ios::app);

        while (std::getline(allWords, line)) {
            if (lineNumber != currentLine) {
                // Add to temp all lines except line that user get
                buffer += line += '\n';
            } else {
                // Write to learned_words.csv word that user got
                learnedWords << line << "\n";
                std::cout << "Line into learned words:" << line << "\n";
                std::stringstream sstr(line);

                std::string word, description;
                std::getline(sstr, word, ',');
                std::getline(sstr, description, ',');

                std::cout << "word: "  << word << "\ndescr: " << description << "\n";
            }
            currentLine++;
        }
        std::ofstream ofstrAllWords(pathToCurrentDB, std::ios::trunc);
        ofstrAllWords << buffer;
    }
}

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
            if (ifstr.is_open()) {
                ifstr >> j;
            } else {
                std::cout << "Error while loading file\n";
            }

            if (arg >= 1 && arg <= 3) {
                std::ofstream ofstr(PATH_SETTINGS);
                j["difficulty"] = arg;
                ofstr << j.dump(4);
                std::cout << "Difficulty change on " << arg << "\n";
            } else {
                std::cout << "No matching arguments. Try again!\n";
            }
        } else {
            std::cout << "Try again.\n";
        }
    }
}