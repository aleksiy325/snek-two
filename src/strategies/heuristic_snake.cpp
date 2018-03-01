#pragma once
#include <utility>
#include <math.h>
#include <limits>
#include "../common/defs.cpp"
#include "../common/game_state.cpp"
#include "../utils/profile.cpp"
#include "strategy.cpp"


class HeuristicSnake: public Strategy {
public:
    double health_weight = 2.02777;
    double food_weight = 9.00786;
    double length_weight = -3.63638;
    double free_weight = 4.72282;
    HeuristicSnake();
    HeuristicSnake(double health_weight, double food_weight, double length_weight, double free_weight);
    double scoreState(GameState gs, snake_index idx);
    Direction decideMove(GameState gs, snake_index idx);
};

HeuristicSnake::HeuristicSnake() {};

HeuristicSnake::HeuristicSnake(double health_weight, double food_weight, double length_weight, double free_weight) {
    this->health_weight = health_weight;
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

    Board board = gs.getBoard();
    Point head = snake.getHead();
    double score = 0;

    vector<Path> paths = board.bfsFood(head);

    if (paths.size()) {
        score += 1.0 / paths[0].length() * food_weight;
        if (snake.getHealth() < paths[0].length()) {
            return std::numeric_limits<double>::lowest();
        }
    }

    int free_squares = board.floodFill(head);
    if (!free_squares) {
        return std::numeric_limits<double>::lowest();
    }

    score += exp(health_weight * snake.getHealth() / 100.0);

    score += free_squares * free_weight;

    score += 1.0 / snake.getSize() * length_weight;

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
