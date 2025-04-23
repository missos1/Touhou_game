#ifndef SCORE_MANAGER_HPP
#define SCORE_MANAGER_HPP

#include <string>
#include <fstream>
#include <sstream>
#include <iostream>

class Game;

class ScoreManager {
public:
	static void readScorefromfile(const std::string& filename);
	static void writeScoretofile(const std::string& filename);
};


#endif // !SCORE_MANAGER_HPP

