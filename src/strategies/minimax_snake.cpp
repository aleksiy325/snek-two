#pragma once
#include <utility>
#include <math.h>
#include <limits>
#include "../common/defs.cpp"
#include "../common/game_state.cpp"
#include "../utils/profile.cpp"
#include "strategy.cpp"
#include <math.h>
#include <ctime>

const int MAX_TIME_MS = 150;

class MinimaxSnake: public Strategy {
public:
    clock_t start;
    int depth = 4;
    double food_weight = 68.60914;
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

    if (!snake.isAlive()) {
        return std::numeric_limits<double>::lowest();
    }

    Point head = snake.getHead();

    // win condition
    if (gs.numAlive() == 1) {
        return std::numeric_limits<double>::max();
    }

    pair<int, int> p = gs.voronoi(idx);
    int free_squares = p.first;
    int owned_food_depth = p.second;
    score += free_squares * free_weight;

    if (!free_squares) {
        return std::numeric_limits<double>::lowest();
    }

    score += 100000 / gs.numAlive();

    vector<Path> paths = gs.bfsFood(head);

    if (paths.size()) {
      // set this to be the distance to the closest food that we control
      // if we don't control any food, set to be distance to closest food in general (to our head)
        int food_length = owned_food_depth;
        if(food_length == -1){
            food_length = paths[0].length();
        }
        if (snake.getHealth() < food_length) {
            return std::numeric_limits<double>::lowest();
        }
        // how much room we have to eat. 0 = we must take exact path to food or we die
        int rope = snake.getHealth() - food_length;
        // using arctan function simulate importance of first few jumps of this value
        score += food_weight * atan(double(rope) / food_exp);
    }

    score += free_squares * free_weight;
    score += 1.0 / snake.getSize() * length_weight;

    return score;
}


pair<double, Direction> MinimaxSnake::alphabeta(GameState gs, snake_index idx, Direction move, double alpha, double beta, int depth, int max_depth, bool max_player) {
    Snake snake = gs.getSnake(idx); 
    int delta = (clock() - start) / (CLOCKS_PER_SEC / 1000);
    if (depth == max_depth || !snake.isAlive() || delta > MAX_TIME_MS) {
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
            Snake new_snake = ns.getSnakes()[idx];
            Point new_head = new_snake.getHead();

            // check whether there's a potential head-on collision

            double scoreAdj = 0;
            vector<Snake> snakes = ns.getSnakes();
            for(int i = 0; i < snakes.size(); i++){
              Snake other_snake = snakes[i];
              if(i != idx && other_snake.isAlive()){
                vector<Point> neighboors = ns.getBoard().expand(other_snake.getHead());
                for(Point neighboor : neighboors){
                  if(neighboor == new_head && new_snake.getSize() <= other_snake.getSize()){
                    scoreAdj += std::numeric_limits<double>::lowest() / 10;
                    break;
                  }
                }
              }

            }
            // check whether a move is on a snake's tail where that snake can eat
            unordered_set<snake_index> occupants = gs.getBoard().getCellOccupants(new_head);
            if(occupants.size() > 0){
              snake_index other_idx = *occupants.begin();
              Snake other_snake = gs.getSnake(other_idx);
              deque<Point> snake_points = other_snake.getPoints();
              Point p1 = snake_points.back();
              snake_points.pop_back();
              Point p2 = snake_points.back();
              if(p1 == p2){
                    scoreAdj += std::numeric_limits<double>::lowest() / 16;
              }
              // vector<Point> expansion = ns.getBoard().expand(other_snake.getHead());
              // for(Point pnt : expansion){
              //   if(gs.getBoard().getCellType(pnt) == CellType::food){
              //     scoreAdj += std::numeric_limits<double>::lowest() / 16;
              //     break;
              //   }
              // }
            }


            pair<double, Direction> min = alphabeta(ns, idx, move, alpha, beta, depth + 1, max_depth, !max_player);
            if(scoreAdj < 0){
              min.first = scoreAdj;
            }
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
    start = clock();
    double alpha = std::numeric_limits<double>::lowest();
    double beta = std::numeric_limits<double>::max();
    pair<double, Direction> move_pair = alphabeta(gs, idx, Direction::North, alpha, beta, 0, depth, true);

    if (move_pair.first < 0) {
        return fallbackMove(gs, idx);
    }
    GameState ct = gs;
    ct.makeMove(move_pair.second, idx);
    ct.cleanup();
    if (!ct.getSnake(idx).isAlive()){
        return fallbackMove(gs, idx);
    }

    return move_pair.second;
}
