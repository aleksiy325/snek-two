#pragma once
#include <utility>
#include <math.h>
#include <limits>
#include "../common/defs.cpp"
#include "../common/game_state.cpp"
#include "../utils/profile.cpp"
#include "strategy.cpp"
#include <math.h>


class HeuristicSnake: public Strategy {
public:
    double food_weight = 10;
    double length_weight = -0.64057;
    double free_weight = 1;
    double food_exp = 1;
    HeuristicSnake();
  HeuristicSnake(double food_weight, double food_exp, double length_weight, double free_weight);
    double scoreState(GameState gs, snake_index idx);
    Direction decideMove(GameState gs, snake_index idx);
};

HeuristicSnake::HeuristicSnake() {};

HeuristicSnake::HeuristicSnake(double food_weight, double food_exp, double length_weight, double free_weight) {
    this->food_exp = food_exp;
    this->food_weight = food_weight;
    this->length_weight = length_weight;
    this->free_weight = free_weight;
};


double HeuristicSnake::scoreState(GameState gs, snake_index idx) {
    //profile prof(__FUNCTION__, __LINE__);
    Snake snake = gs.getSnake(idx);
    if (!snake.isAlive()) {
        return std::numeric_limits<double>::lowest();
    }
    // cout << "REACH";
    Point head = snake.getHead();
    double score = 0;

    // vector<Path> paths = gs.bfsFood(head);

    // if (paths.size()) {
    //     // score += 1.0 / paths[0].length() * food_weight;
    //     if (snake.getHealth() < paths[0].length()) {
    //         return std::numeric_limits<double>::lowest();
    //     }
    //     int rope = snake.getHealth() - paths[0].length();
    //     // score += food_weight * pow(double(rope),3.0/5.0);
    //     score += food_weight * atan(food_exp * double(rope));
    //     // cout << "score: " << score << "\n";
    // }

    int free_squares = gs.voronoi(idx);

    //int free_squares = gs.voronoi(idx);
    // cerr << "Free_squares" << free_squares;

    // if (!free_squares) {
    //     return std::numeric_limits<double>::lowest();
    // }

    // // score += exp(health_weight * snake.getHealth() / 100.0);

    // score += free_squares * free_weight;

    // score += 1.0 / snake.getSize() * length_weight;

    return score;
}

Direction HeuristicSnake::decideMove(GameState gs, snake_index idx) {
    vector<pair<double, Direction>> move_scores;
    for (auto dir : DIRECTIONS) {
        GameState new_state = gs;
        new_state.makeMove(dir, idx);
        new_state.cleanup();
        double score = scoreState(new_state, idx);
        move_scores.push_back(make_pair(score, dir));
    }
    vector<pair<double, Direction>>::iterator result = max_element(move_scores.begin(), move_scores.end());
    return result->second;
}
