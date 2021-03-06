#pragma once
#include "../strategies/strategy.cpp"
#include "../common/game_state.cpp"
#include "../common/board.cpp"
#include "score.cpp"
#include <time.h>

class Game {
public:
	GameState game_state;
	vector<Strategy*> strategies;
	Game();
	Game(int width, int height, int max_food);
	Game(int width, int height, int max_food, int seed);
	void decideMoves();
	void executeTick();
	void addStrategy(Strategy* strategy);
	void addStrategies(vector<Strategy*> strategies);
	Board getBoard();
	GameState getGameState();
	int getWidth();
	int getHeight();
	bool winnerExists();
  	bool isWinner(int i);
	void execute();
	vector<Score> getScores();
  void resetGame(int width, int height, int max_food, int seed);
};

void Game::resetGame(int width, int height, int max_food, int seed){
  game_state = GameState(width, height, max_food);
  srand(seed);
  for(int i = 0; i < strategies.size(); i++){
    game_state.addSnake();
  }
}

Game::Game(){
}

Game::Game(int width, int height, int max_food){
    game_state = GameState(width, height, max_food);
    srand (time(NULL));
}

Game::Game(int width, int height, int max_food, int seed){
    game_state = GameState(width, height, max_food);
    srand(seed);
}

void Game::decideMoves(){
    snake_index idx = 0;
    GameState save_state = game_state;
    for(auto strategy: strategies){
        Snake snake = save_state.getSnake(idx);
        if(snake.isAlive()){
            Direction dir = strategy->decideMove(save_state, idx);
            game_state.makeMove(dir, idx);
        }
        idx++;
    }
}

void Game::executeTick(){
	decideMoves();
    game_state.cleanup();
    if(!game_state.isValid()){
        cout << "Non valid game_state" << endl;
    }
}

void Game::addStrategy(Strategy* strategy){
	game_state.addSnake();
	strategies.push_back(strategy);
}

void Game::addStrategies(vector<Strategy*> strategies){
	this->strategies = strategies;
	for(auto strategy: strategies){
		game_state.addSnake();
	}
}

Board Game::getBoard(){
	return game_state.getBoard();
}

int Game::getWidth(){
	return game_state.getWidth();
}

int Game::getHeight(){
	return game_state.getHeight();
}

bool Game::winnerExists(){
	int alive = 0;
	snake_index idx = 0;
    for(auto strategy: strategies){
        Snake snake = game_state.getSnake(idx);
        if(snake.isAlive()){
           	alive++;
        }
        idx++;    
    }
	return alive == 0 || alive == 1;
}

GameState Game::getGameState(){
	return game_state;
}

void Game::execute(){
	while(!winnerExists()){
		executeTick();
	}
}

vector<Score> Game::getScores(){
	vector<Score> scores;
	for(auto snake: game_state.getSnakes()){
		int ticks = snake.getScore();
		bool isWinner = snake.isAlive();
		Score score = Score(ticks, isWinner);
		scores.push_back(score);
	}
	return scores;
}

bool Game::isWinner(int i){
	return game_state.getSnakes()[i].isAlive();
}

