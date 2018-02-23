#pragma once
#include <utility>
#include <math.h>
#include "../common/defs.cpp"
#include "../common/game_state.cpp"
#include "strategy.cpp"

//interesting
// double health_weight =  -3.12642 ;
// double food_weight =  2.76722;
// double length_weight = 2.72358  ;
// double free_weight = 1.62158;

//good
// double health_weight = -6.01007;
// double food_weight =  2.34089;
// double length_weight = 4.38102  ;
// double free_weight = 5.71191;

class HeuristicSnake: public Strategy {
	double health_weight = 9.53765 ;
	double food_weight =  -5.78759 ;
	double length_weight = 5.99268  ;
	double free_weight = 0.78019 ;
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
			
			score += 1.0 / snake.getSize() * length_weight;

			if(paths.size()){
				score += 1.0 / paths[0].length() * food_weight;
				if(snake.getHealth() < paths[0].length()){
					score = 0;
				}
			}

			if(!free_squares){
				score = 0;
			}

		}

		move_scores.push_back(make_pair(score, dir));
	}
	vector<pair<double, Direction>>::iterator result = max_element(move_scores.begin(), move_scores.end());
	return result->second;
}