#pragma once
#include "../common/game_state.cpp"
#include "../common/defs.cpp"

class Strategy{
public:
	virtual Direction decideMove(GameState gs, snake_index idx) = 0;
};