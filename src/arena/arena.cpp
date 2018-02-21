#pragma once
#include "../strategies/strategy.cpp"
#include "../strategies/random_snake.cpp"
#include "../common/game_state.cpp"
#include "../common/board.cpp"

class Arena {
public:
	int tick;
	GameState game_state;
	vector<Strategy*> strategies;
	Arena();
	Arena(int width, int height, int max_food);
	void decideMoves();
	void executeTick();
	void addStrategy(Strategy* strategy);
	void addStrategies(vector<Strategy*> strategies);
	Board getBoard();
	GameState getGameState();
	int getWidth();
	int getHeight();
	bool winnerExists();
};

Arena::Arena(){}

Arena::Arena(int width, int height, int max_food){
    game_state = GameState(width, height, max_food);
}

void Arena::decideMoves(){
    snake_index idx = 0;
    for(auto strategy: strategies){
        Snake snake = game_state.getSnake(idx);
        if(snake.isAlive()){
            Direction dir = strategy->decideMove(game_state, idx);
            game_state.makeMove(dir, idx);
        }
        idx++;    
    }
}

void Arena::executeTick(){
	decideMoves();
    game_state.cleanup();
   	tick++;
    if(!game_state.isValid()){
        cout << "Non valid game_state" << endl;
    }
}

void Arena::addStrategy(Strategy* strategy){
	game_state.addSnake();
	strategies.push_back(strategy);
}

void Arena::addStrategies(vector<Strategy*> strategies){
	this->strategies = strategies;
	for(auto strategy: strategies){
		game_state.addSnake();
	}
}

Board Arena::getBoard(){
	return game_state.getBoard();
}

int Arena::getWidth(){
	return game_state.getWidth();
}

int Arena::getHeight(){
	return game_state.getHeight();
}

bool Arena::winnerExists(){
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

GameState Arena::getGameState(){
	return game_state;
}