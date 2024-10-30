#pragma once

#define PATH_DB_1 "../db/words1.csv"
#define PATH_DB_2 "../db/words2.csv"
#define PATH_DB_3 "../db/words3.csv"
#define PATH_DB_LEARNED "../db/learned_words.csv"
#define PATH_SETTINGS "../settings/settings.json"

#include <string>
#include <sstream>
#include <fstream>
#include <iostream>
#include <random>
#include <json.hpp>

int getDifficulty();
std::string matchDifficulty();
void initDifficulty();
int getLinesCount();
int getRandLine(std::string);
void getRandWord();
