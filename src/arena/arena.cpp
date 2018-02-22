#pragma once
#include "score.cpp"
#include "game.cpp"


class Arena{
public:
	int width;
	int height;
	int max_food;
	int iterations;
	vector<Strategy*> strategies;
	Arena(int width, int height, int max_food, int iterations);
	void addStrategy(Strategy* strat);
	void execute();
};

Arena::Arena(int width, int height, int max_food, int iterations){
	this->width = width;
	this->height = height;
	this->max_food = max_food;
	this->iterations = iterations;
}

void Arena::addStrategy(Strategy* strat){
	strategies.push_back(strat);
}

void Arena::execute(){
	for(int i = 0; i < iterations; i++){
		Game game = Game(width, height, max_food);
		game.addStrategies(strategies);
		game.execute();
		vector<Score> scores = game.getScores();
		for(auto score: scores){
			cout << " " << score.ticks << " " << score.isWinner << endl;
		}
		cout << "------" << endl;
	}
}

