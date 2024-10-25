#include <sstream>
#include <fstream>
#include <random>
#include <iostream>
#include <string>
#include "json.hpp"

void initState(std::string &allWords) {
    using namespace nlohmann;
    std::ifstream ifstr("../settings.json");

    json j;
    ifstr >> j;

    if (j["difficulty"] == 0) {
        int newDifficulty = 0;
        while(!(newDifficulty >= 1 && newDifficulty <= 3)) {
            std::cout << "Set diff 1,2,3: ";
            std::cin >> newDifficulty;
        }
        j["difficulty"] = newDifficulty;
        std::ofstream ofstr("../settings.json", std::ios::trunc);
        ofstr << j;
    } 
    if (j["difficulty"] != 0) {
        int difficulty = j["difficulty"];
        switch(difficulty) {
            case 1:
                allWords = "../all_words1.csv";
            break;
            case 2:
                allWords = "../all_words2.csv";
            break;
            case 3:
                allWords = "../all_words3.csv";
            break;
        }
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

    if (allWords.is_open()) {
        // Get rand number of line
        int lineNumber = getRandLine(pathToFile);
        std::cout << "lineNumber:" << lineNumber << "\n";

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
    std::string allWords;
    initState(allWords);
    
    std::string input;
    while(input.empty()) {
        getline(std::cin, input);
        getRandWord(allWords);
    }
}