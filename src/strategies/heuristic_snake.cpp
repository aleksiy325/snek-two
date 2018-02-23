#pragma once
#include <utility>
#include <math.h>
#include "../common/defs.cpp"
#include "../common/game_state.cpp"
#include "strategy.cpp"


class HeuristicSnake: public Strategy {
	double health_weight = 6.53956;
	double food_weight = 9.54376;
	double length_weight = -7.61898;
	double free_weight = 10.00;
public:
	HeuristicSnake();
	HeuristicSnake(double health_weight, double food_weight, double length_weight, double free_weight);
	Direction decideMove(GameState gs, snake_index idx);
};

HeuristicSnake::HeuristicSnake(){};

HeuristicSnake::HeuristicSnake(double health_weight, double food_weight, double length_weight, double free_weight){
	this->health_weight = health_weight;
	this->food_weight = food_weight;
	this->length_weight = length_weight;
	this->free_weight = free_weight;
};

Direction HeuristicSnake::decideMove(GameState gs, snake_index idx){
	vector<pair<double, Direction>> move_scores;
	for(auto dir: DIRECTIONS){
		double score = 0;
		GameState new_state = gs;
		new_state.makeMove(dir, idx);
		new_state.cleanup();
		Snake snake = new_state.getSnake(idx);
		if(snake.isAlive()){
			Board board = new_state.getBoard();
			Point head = snake.getHead();
			vector<Path> paths = board.bfsFood(head);
			int free_squares = board.floodFill(head);

			score += exp(health_weight * snake.getHealth() / 100.0);

			score += free_squares * free_weight;
			
			if(paths.size()){
				score += 1.0 / paths[0].length() * food_weight;
			}
			
			score += 1.0 / snake.getSize() * length_weight;
		}

		move_scores.push_back(make_pair(score, dir));
	}
	vector<pair<double, Direction>>::iterator result = max_element(move_scores.begin(), move_scores.end());
	return result->second;
}