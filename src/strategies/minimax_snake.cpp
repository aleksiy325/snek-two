#pragma once
#include <utility>
#include <math.h>
#include <limits>
#include "../common/defs.cpp"
#include "../common/game_state.cpp"
#include "../utils/profile.cpp"
#include "strategy.cpp"
#include <math.h>


class MinimaxSnake: public Strategy {
public:
    double food_weight = 10;
    double length_weight = -0.64057;
    double free_weight = 1;
    double food_exp = 1;
    MinimaxSnake();
  	MinimaxSnake(double food_weight, double food_exp, double length_weight, double free_weight);
  	pair<double, Direction> minimax(GameState gs, snake_index idx, Direction move, int depth, int max_depth, bool max_player);
    double scoreState(GameState gs, snake_index idx);
    Direction decideMove(GameState gs, snake_index idx);
};

MinimaxSnake::MinimaxSnake() {};

MinimaxSnake::MinimaxSnake(double food_weight, double food_exp, double length_weight, double free_weight) {
    this->food_exp = food_exp;
    this->food_weight = food_weight;
    this->length_weight = length_weight;
    this->free_weight = free_weight;
}

double MinimaxSnake::scoreState(GameState gs, snake_index idx){
	double score = 0;
    Snake snake = gs.getSnake(idx);
    Point head = snake.getHead();

    if (!snake.isAlive()) {
        return std::numeric_limits<double>::lowest();
    }


    vector<Path> paths = gs.bfsFood(head);

    if (paths.size()) {
        // score += 1.0 / paths[0].length() * food_weight;
        if (snake.getHealth() < paths[0].length()) {
            return std::numeric_limits<double>::lowest();
        }
        int rope = snake.getHealth() - paths[0].length();
        // score += food_weight * pow(double(rope),3.0/5.0);
        score += food_weight * atan(food_exp * double(rope));
        // cout << "score: " << score << "\n";
    }

    int free_squares = gs.voronoi(idx);
    score += free_squares * free_weight;


    // if (!free_squares) {
    //     return std::numeric_limits<double>::lowest();
    // }

    // score += free_squares * free_weight;
    // score += 1.0 / snake.getSize() * length_weight;

    return score;
}

pair<double, Direction> MinimaxSnake::minimax(GameState gs, snake_index idx, Direction move, int depth, int max_depth, bool max_player){
	Snake snake = gs.getSnake(idx);
	if (depth == max_depth || !snake.isAlive()){
		return make_pair(scoreState(gs, idx), move);
	}
	Board board = gs.getBoard();
	snake_index opp_idx = gs.getOpponent(idx);
	Snake opp_snake = gs.getSnake(opp_idx);
	vector<Direction> our_moves = snake.getMoves();
	vector<Direction> opp_moves = opp_snake.getMoves();
	vector<pair<Direction, Direction>> combos = vector<pair<Direction, Direction>>();

	assert(snake.isAlive());
	assert(opp_snake.isAlive());

	for(auto i : our_moves){
		for(auto j: opp_moves){
			pair<Direction, Direction> p = make_pair(i, j);
			combos.push_back(p);
		}
	}

	vector<pair<double, Direction>> scores = vector<pair<double, Direction>>();
	for(auto move_pair : combos){
		GameState ns = gs;
		ns.makeMove(move_pair.first, idx);
		if(idx != opp_idx){
			ns.makeMove(move_pair.second, opp_idx); // you played yourself
		}
		ns.cleanup();
		// TODO: move dependent on max_player?
		pair<double, Direction> score_pair = minimax(ns, opp_idx, move_pair.first, depth + 1, max_depth, !max_player);
		score_pair.second = move_pair.first;
		scores.push_back(score_pair);
	}
	pair<double, Direction> move_pair;
	if(max_player){
		move_pair = *max_element(scores.begin(), scores.end());
	}else{
		move_pair = *min_element(scores.begin(), scores.end());
	}
	return move_pair;
}	


Direction MinimaxSnake::decideMove(GameState gs, snake_index idx) {
    //profile prof(__FUNCTION__, __LINE__);
	pair<double, Direction> move_pair = minimax(gs, idx, Direction::North, 0, 3, true);	
	return move_pair.second;
}