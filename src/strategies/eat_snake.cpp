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

Direction findMove(Board board, Point head){
	vector<Direction> safe;
	for(auto dir : directions){
		Point p = head.addMove(dir);
		if(board.isSafe(p)){
			safe.push_back(dir);
		}
	}

	if(safe.size() > 0){
		return safe[rand() % safe.size()];
	}
	return Direction::North;
}

Direction RandomSnake::decideMove(GameState gs, snake_index idx){
	Board board = gs.getBoard();
	Snake snake = gs.getSnake(idx);
	Point head = snake.getHead();
	return findMove(board, head);
