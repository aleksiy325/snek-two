#pragma once
#include <random>
#include "../common/game_state.cpp"
#include "strategy.cpp"

vector<Direction> directions = {Direction::North, Direction::South, Direction::East, Direction::West};

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
	Path path = board.bfsFood(head);
	if(path.exists()){
		return path.getStepDir(0);
	}

	return Direction::North;
}