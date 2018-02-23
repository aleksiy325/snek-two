#pragma once
#include <random>
#include "../common/game_state.cpp"
#include "strategy.cpp"


class EatSnake: public Strategy {
public:
	EatSnake();
	Direction decideMove(GameState gs, snake_index idx);
};

EatSnake::EatSnake(){};

Direction EatSnake::decideMove(GameState gs, snake_index idx){
	Board board = gs.getBoard();
	Snake snake = gs.getSnake(idx);
	Point head = snake.getHead();
	vector<Path> paths = board.bfsFood(head);
	if(paths.size() > 0){
		return paths[0].getStepDir(0);
	}

	return Direction::North;
}