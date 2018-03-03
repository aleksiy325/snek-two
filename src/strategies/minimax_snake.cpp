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
    int depth = 4;
    double food_weight = 28.60914;
    double length_weight = 0.82267;
    double free_weight = 7.60983;
    double food_exp = 8.51774;
    MinimaxSnake();
    MinimaxSnake(double food_weight, double food_exp, double length_weight, double free_weight);
    Direction fallbackMove(GameState gs, snake_index idx);
    pair<double, Direction> alphabeta(GameState gs, snake_index idx, Direction move, double alpha, double beta, int depth, int max_depth, bool max_player);
    double scoreState(GameState gs, snake_index idx);
    Direction decideMove(GameState gs, snake_index idx);
};

MinimaxSnake::MinimaxSnake() {};

MinimaxSnake::MinimaxSnake(double food_weight, double food_exp, double length_weight, double free_weight) {
    this->food_exp = food_exp;
    this->food_weight = food_weight;
    this->length_weight = length_weight;
    this->free_weight = free_weight;
    this->depth = 2;
}

double MinimaxSnake::scoreState(GameState gs, snake_index idx) {
    double score = 0;
    Snake snake = gs.getSnake(idx);
    Point head = snake.getHead();

    if (!snake.isAlive()) {
        return std::numeric_limits<double>::lowest();
    }

    //win cond
    if (gs.numAlive() == 1) {
        return std::numeric_limits<double>::max();
    }

    score += 100000 / gs.numAlive();

    vector<Path> paths = gs.bfsFood(head);

    if (paths.size()) {
        if (snake.getHealth() < paths[0].length()) {
            return std::numeric_limits<double>::lowest();
        }
        int rope = snake.getHealth() - paths[0].length();
        score += food_weight * atan(double(rope) / food_exp);
    }

    int free_squares = gs.voronoi(idx);
    score += free_squares * free_weight;

    if (!free_squares) {
        return std::numeric_limits<double>::lowest();
    }

    score += free_squares * free_weight;
    score += snake.getSize() * length_weight;

    return score;
}

pair<double, Direction> MinimaxSnake::alphabeta(GameState gs, snake_index idx, Direction move, double alpha, double beta, int depth, int max_depth, bool max_player) {
    Snake snake = gs.getSnake(idx);
    if (depth == max_depth || !snake.isAlive()) {
        return make_pair(scoreState(gs, idx), move);
    }
    Board board = gs.getBoard();
    snake_index opp_idx = gs.getOpponent(idx);
    Snake opp_snake = gs.getSnake(opp_idx);

    if (max_player) {
        double cur_max = std::numeric_limits<double>::lowest();
        Direction cur_move;

        vector<Direction> our_moves = snake.getMoves();
        for (auto move : our_moves) {
            GameState ns = gs;
            ns.makeMove(move, idx);
            pair<double, Direction> min = alphabeta(ns, idx, move, alpha, beta, depth + 1, max_depth, !max_player);
            if (min.first > cur_max) {
                cur_max = min.first;
                cur_move = move;
            }
            if (cur_max > alpha) {
                alpha = cur_max;
            }
            if (beta <= alpha) {
                break;
            }
        }
        return make_pair(cur_max, cur_move);

    } else {
        double cur_min = std::numeric_limits<double>::max();
        Direction cur_move;

        vector<Direction> their_moves = opp_snake.getMoves();
        for (auto move : their_moves) {
            GameState ns = gs;
            if (opp_idx != idx) {
                ns.makeMove(move, opp_idx);
                ns.cleanup();
            }
            pair<double, Direction> max = alphabeta(ns, idx, move, alpha, beta, depth + 1, max_depth, !max_player);
            if (max.first < cur_min) {
                cur_min = max.first;
                cur_move = move;
            }
            if (cur_min < beta) {
                beta = cur_min;
            }
            if (beta <= alpha) {
                break;
            }
        }
        return make_pair(cur_min, cur_move);
    }
}

Direction MinimaxSnake::fallbackMove(GameState gs, snake_index idx) {
    Snake snake = gs.getSnake(idx);
    Point head = snake.getHead();
    vector<Direction> safe;
    for (auto dir : DIRECTIONS) {
        Point p = head.addMove(dir);
        if (gs.isSafe(p, 1)) {
            safe.push_back(dir);
        }
    }
    if (safe.size() > 0) {
        return safe[rand() % safe.size()];
    }
    return Direction::North;
}


Direction MinimaxSnake::decideMove(GameState gs, snake_index idx) {
    //profile prof(__FUNCTION__, __LINE__);
    double alpha = std::numeric_limits<double>::lowest();
    double beta = std::numeric_limits<double>::max();
    pair<double, Direction> move_pair = alphabeta(gs, idx, Direction::North, alpha, beta, 0, depth, true);
    //cout << move_pair.first << endl;
    if (move_pair.first == std::numeric_limits<double>::lowest()) {
        //cout << "Fallback Move" << endl;
        return fallbackMove(gs, idx);
    }

    return move_pair.second;
}