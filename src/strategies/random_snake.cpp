#pragma once
#include "../common/game_state.cpp"
#include "strategy.cpp"

class RandomSnake: public Strategy {
public:
	RandomSnake();
	Direction decideMove(GameState gs, snake_index idx);
};

RandomSnake::RandomSnake(){};

Direction RandomSnake::decideMove(GameState gs, snake_index idx){
	return Direction::North;
} 