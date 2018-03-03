#pragma once

#include <deque>
#include "point.cpp"

using namespace std;

class Snake {
    bool alive;
    int health;
    int score;
    int free_moves;
    string id;
public:
    deque<Point> points;
    Snake(Point start);
    Snake(Point start, int free_moves);
    Snake(int health, int score, int free_moves, string id);
    void setHealth(int new_health);
    int getHealth();
    int loseHealth();
    Point getHead();
    bool isAlive();
    void setAlive(bool alive);
    Point makeMove(Direction dir);
    Point popTail();
    size_t getSize();
    deque<Point> getPoints();
    void clearPoints();
    int getScore();
    int getFreeMoves();
    void useFreeMove();
    int getTurnsOccupied(Point p);
    vector<Direction> getMoves();
    void addPoint(Point p);
    bool inSnake(Point p);
    string getID();
};


Snake::Snake(Point start) {
    health = MAX_HEALTH;
    setAlive(true);
    score = 0;
    points.push_back(start);
    this->free_moves = FREE_MOVES;
}

Snake::Snake(Point start, int free_moves) {
    health = MAX_HEALTH;
    setAlive(true);
    score = 0;
    points.push_back(start);
    this->free_moves = free_moves;
}

Snake::Snake(int health, int score, int free_moves, string id) {
    this->health = health;
    this->score = score;
    this->free_moves = free_moves;
    this->id = id;
    setAlive(true);
}

Point Snake::getHead() {
    return *points.begin();
}

void Snake::setHealth(int new_health) {
    health = new_health;
}

int Snake::getHealth() {
    return health;
}

int Snake::loseHealth() {
    health -= HEALTH_LOSS;
    return health;
}

bool Snake::isAlive() {
    return alive;
}

void Snake::setAlive(bool alive) {
    this->alive = alive;
}

Point Snake::makeMove(Direction dir) {
    Point head = *points.begin();
    Point new_head = head.addMove(dir);
    points.push_front(new_head);
    score++;
    return new_head;
}

Point Snake::popTail() {
    Point back = points.back();
    // cout << "popping back snake" << <<  ": " << back.x << "," << back.y << "\n";
    points.pop_back();
    return back;
}


size_t Snake::getSize() {
    return points.size();
}

deque<Point> Snake::getPoints() {
    return points;
}

void Snake::clearPoints() {
    points.clear();
}

int Snake::getScore() {
    return score;
}

int Snake::getFreeMoves() {
    return free_moves;
}

void Snake::useFreeMove() {
    free_moves--;
}

int Snake::getTurnsOccupied(Point p) {
    int index = std::find(points.begin(), points.end(), p) - points.begin();
    assert(index != points.size());
    return points.size() - index;
}

vector<Direction> Snake::getMoves() {
    vector<Direction> moves = vector<Direction>();
    Point head = getHead();
    for (auto dir : DIRECTIONS) {
        Point p = head.addMove(dir);
        if (getSize() > 1) {
            if (points[1] != p) {
                moves.push_back(dir);
            }
        } else {
            moves.push_back(dir);
        }
    }
    return moves;
}

void Snake::addPoint(Point p) {
    points.push_back(p);
}

bool Snake::inSnake(Point p) {
    return std::find(points.begin(), points.end(), p) != points.end();
}

string Snake::getID() {
    return id;
}
