#include <sstream>
#include <fstream>
#include <random>
#include <iostream>
#include <string>

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
/*
Steps:
1. Create temp
2. Move all lines to temp file except line that user got
3. Remove all_words.csv
4. Rename temp -> all_words.csv
*/

void getRandWord(std::string pathToFile) {
    std::fstream fstr(pathToFile, std::ios::in | std::ios::out);

    if (fstr.is_open()) {
        // Get rand number of line
        int lineNumber = getRandLine(pathToFile);
        std::cout << "lineNumber:" << lineNumber << "\n";

        std::string line;
        int currentLine = 0;
        
        // Create temp of all_words.csv
        std::ofstream ofstr("../temp.csv");
        // Open learned_words.csv
        std::ofstream ofstrLearned("../learned_words.csv", std::ios::app);

        while (std::getline(fstr, line)) {
            currentLine++;
            
            if (lineNumber != currentLine) {
                // Add to temp all lines except line that user get
                ofstr << line << "\n";
            } else {
                // Write to learned_words.csv word that user got
                ofstrLearned << line << "\n";
                std::stringstream sstr(line);

                std::string word, description;
                std::getline(sstr, word, ',');
                std::getline(sstr, description, ',');

                std::cout << "word: "  << word << "\ndescr: " << description << "\n";
            }
        }
        // Remove all words db
        std::remove(pathToFile.c_str());
        // Rename temp to all words
        std::rename("../temp.csv", pathToFile.c_str());
    }
}

int main() {
    getRandWord("../all_words.csv");
}