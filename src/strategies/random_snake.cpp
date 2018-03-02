#pragma once
#include <random>
#include "../common/game_state.cpp"
#include "strategy.cpp"


class RandomSnake: public Strategy {
public:
	RandomSnake();
	Direction decideMove(GameState gs, snake_index idx);
};

RandomSnake::RandomSnake(){};

Direction RandomSnake::decideMove(GameState gs, snake_index idx){
	Snake snake = gs.getSnake(idx);
	Point head = snake.getHead();
	vector<Direction> safe;
	for(auto dir : DIRECTIONS){
		Point p = head.addMove(dir);
		if(gs.isSafe(p, 1)){
			safe.push_back(dir);
		}
	}

	if(safe.size() > 0){
		return safe[rand() % safe.size()];
	}
	return Direction::North;
} 