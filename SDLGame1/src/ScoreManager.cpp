#include "headers/ScoreManager.hpp"
#include "headers/Game.hpp"

void ScoreManager::readScorefromfile(const std::string& filename) {
	std::ifstream file(filename);
	if (!file.is_open()) {
		std::cerr << "Error opening file: " << filename << std::endl;
		return;
	}

	std::string score_str;
	file >> score_str;
	if (score_str.empty()) {
		std::cerr << "Error: No score found in file: " << filename << std::endl;
		return;
	}
	Game::HIGHSCORE = std::stoi(score_str);
	file.close();
}

void ScoreManager::writeScoretofile(const std::string& filename) {
	std::ifstream file(filename);
	int oldHighscore = 0;
	if (!file.is_open()) {
		std::cerr << "Error opening file: " << filename << std::endl;
		return;
	}

	std::string hscore_str;
	file >> hscore_str;

	if (Game::HIGHSCORE > std::stoi(hscore_str)) {
		std::ofstream outfile(filename);
		if (!outfile.is_open()) {
			std::cerr << "Error opening file for writing: " << filename << std::endl;
			return;
		}
		outfile << Game::HIGHSCORE;
		outfile.close();
	}
}