#pragma once

#include <vector>
#include <random>
#include <algorithm>
#include "board.cpp"
#include "snake.cpp"


#include <map>
#include <set>
#include <cassert>
#include <tuple>

using namespace std;

const Point DEPTH_MARK = Point(-1, -1);

class GameState {
    int max_food;
    int cur_food;
    int tick;
    Board board;
    vector<Snake> snakes;
public:
    GameState();
    GameState(int height, int width);
    GameState(int height, int width, int num_snakes);
    void makeMove(Direction dir, snake_index idx);
    snake_index addSnake();
    snake_index addSnake(Point start);
    snake_index addSnake(Snake snake);
    void addFood();
    void addFood(Point p);
    Snake& getSnake(snake_index idx);
    void removeFood(Point p);
    void checkCollision(Point cur_point);
    void cleanup();
    bool isValid();
    bool isGivenValid();
    Board getBoard();
    int getHeight();
    int getWidth();
    vector<Snake> getSnakes();
    void printScoreBoard();
    int voronoi(int voronoi_snake_id);
    int getTick();
    bool isSafe(Point p, int distance);
    bool willBeUnnocupied(Point p, int distance);
    vector<Path> bfsFood(Point start);
    snake_index getOpponent(snake_index idx);
    int floodFill(Point start);
};


GameState::GameState() {}

GameState::GameState(int width, int height) {
    board = Board(width, height);
    this->tick = 0;
}

GameState::GameState(int width, int height, int max_food) {
    board = Board(width, height);
    this->max_food = max_food;
    cur_food = 0;
    for (int i = 0; i < max_food; i++) {
        addFood();
    }
    this->tick = 0;
}

snake_index GameState::addSnake() {
    Point start = board.getRandomEmptyPoint();
    Snake snake = Snake(start);
    snake_index idx = snakes.size();
    board.occupyCell(start, idx);
    snakes.push_back(snake);
    return idx;
}

void GameState::addFood() {
    Point p = board.getRandomEmptyPoint();
    board.placeFood(p);
    cur_food++;
}

void GameState::addFood(Point p) {
    assert(board.in(p));
    board.placeFood(p);
    cur_food++;
}

snake_index GameState::addSnake(Point start) {
    Snake snake = Snake(start);
    snake_index idx = snakes.size();
    board.occupyCell(start, idx);
    snakes.push_back(snake);
    return idx;
}

snake_index GameState::addSnake(Snake snake) {
    snake_index idx = snakes.size();
    snakes.push_back(snake);
    for (auto point : snake.getPoints()) {
        board.occupyCell(point, idx);
    }
    return idx;
}

<<< <<< < HEAD
Snake& GameState::getSnake(snake_index idx) {
    assert(idx < snakes.size() && idx >= 0);
    return snakes[idx];
    == == == =
    void GameState::makeMove(Direction dir, snake_index idx) {
        Snake& snake = snakes[idx];
        Point head = snake.makeMove(dir);
        snake.loseHealth();

        //check self collision before occupy
        if (board.isOccupantOf(head, idx)) {
            snakes[idx].setAlive(false);
        }

        board.occupyCell(head, idx);

        CellType type = board.getCellType(head);
        Point old_tail;

        switch (type) {
        case CellType::food:
            // cout << "idx " << idx << " eating food\n";
            snakes[idx].setHealth(MAX_HEALTH);
            break;

        case CellType::wall:
            old_tail = snake.popTail();
            board.vacateCell(old_tail, idx);
            snakes[idx].setAlive(false);
            break;

        case CellType::empty:
            if (snake.getFreeMoves() == 0) {
                old_tail = snake.popTail();
                board.vacateCell(old_tail, idx);
            } else {
                snake.useFreeMove();
            }
            break;
        }
        >>> >>> > aa3856322ec5ef96df181e96183b4d9837c6d5f2
    }

    void GameState::makeMove(Direction dir, snake_index idx) {
        Snake& snake = snakes[idx];
        Point head = snake.makeMove(dir);
        snake.loseHealth();

        //check self collision before occupy
        if (board.isOccupantOf(head, idx)) {
            snakes[idx].setAlive(false);
        }

        board.occupyCell(head, idx);

        CellType type = board.getCellType(head);
        Point old_tail;

        switch (type) {
        case CellType::food:
            snakes[idx].setHealth(MAX_HEALTH);
            break;

        case CellType::wall:
            old_tail = snake.popTail();
            board.vacateCell(old_tail, idx);
            snakes[idx].setAlive(false);
            break;

        case CellType::empty:
            if (snake.getFreeMoves() == 0) {
                old_tail = snake.popTail();
                board.vacateCell(old_tail, idx);
            } else {
                snake.useFreeMove();
            }
            break;
        }
    }

    void GameState::checkCollision(Point cur_point) {
        if (board.cellNumOccupants(cur_point) > 1) {
            unordered_set<snake_index> occupants = board.getCellOccupants(cur_point);
            vector<snake_index> heads;
            vector<size_t> head_lengths;

            for (auto s_index : occupants) {
                Snake cur_snake = snakes[s_index];
                Point head = cur_snake.getHead();
                if (cur_point == head) {
                    heads.push_back(s_index);
                    head_lengths.push_back(cur_snake.getSize());
                }
            }

            assert(head_lengths.size() > 0);
            assert(head_lengths.size() == heads.size());

            // if all occupants are heads and there is more than on head
            if (heads.size() == board.cellNumOccupants(cur_point) && heads.size() > 1) {
                size_t max_len = *max_element(head_lengths.begin(), head_lengths.end());
                int count_max = count(head_lengths.begin(), head_lengths.end(), max_len);

                if (count_max > 1) { // if more than one are max everyone dies
                    max_len = -1;
                }

                for (auto& s_index : heads) {
                    Snake& cur_snake = snakes[s_index];
                    if (cur_snake.getSize() != max_len) {
                        cur_snake.setAlive(false);
                    }
                }

            } else {
                for (auto s_index : heads) {
                    snakes[s_index].setAlive(false);
                }
            }
        }
    }

    void GameState::removeFood(Point p) {
        if (board.getCellType(p) == CellType::food) {
            board.setCellType(p, CellType::empty);
            cur_food--;
        }
    }

    void GameState::cleanup() {
        // check collisions
        for (auto& snake : snakes) {
            if (snake.getHealth() <= 0) {
                snake.setAlive(false);
            }
            if (snake.isAlive()) {
                Point cur_point = snake.getHead();
                checkCollision(cur_point);
                removeFood(cur_point);
            }
        }

        // clean up dead snakes
        for (snake_index i = 0; i < snakes.size(); i++) {
            Snake& snake = snakes[i];
            if (!snake.isAlive()) {
                deque<Point> points = snake.getPoints();
                for (auto point : points) {
                    board.vacateCell(point, i);
                }
                snake.clearPoints();
            }
        }

        while (cur_food < max_food) {
            addFood();
        }

        // assert(isValid());
        tick++;
    }

    bool GameState::isValid() {
        if (cur_food != max_food) {
            cout << cur_food << " != " << max_food << endl;
            return false;
        }
        return isGivenValid();
    }


    bool GameState::isGivenValid() {
        if (!board.isValid()) {
            cout << "Board is not valid" << endl;
            return false;
        }

        for (snake_index i = 0; i < snakes.size(); i++) {
            Snake& snake = snakes[i];
            deque<Point> points = snake.getPoints();
            for (auto point : points) {
                if (snake.isAlive() && !board.isOccupantOf(point, i)) {
                    cout << "snake is alive but doesnt occupy point" << endl;
                    return false;
                }
            }
        }
        return true;
    }

    Board GameState::getBoard() {
        return board;
    }

    int GameState::getHeight() {
        return board.getHeight();
    }

    int GameState::getWidth() {
        return board.getWidth();
    }

    vector<Snake> GameState::getSnakes() {
        return snakes;
    }

    void GameState::printScoreBoard() {
        int i = 0;
        for (auto snake : snakes) {
            Point head = snake.getHead();
            cout << i << " " << snake.getHealth() << " " << snake.isAlive() << " " << head.x << "," << head.y << endl;
            i++;
        }
    }

    int GameState::getTick() {
        return this->tick;
    }

    bool GameState::willBeUnnocupied(Point p, int distance) {
        bool will_be_unoccupied = true;
        if (board.cellNumOccupants(p) > 0) {

            //Find opponent makes this assertion fail
            //assert(board.cellNumOccupants(p) == 1);
            snake_index occupant = *board.getCellOccupants(p).begin();
            will_be_unoccupied = snakes[occupant].getTurnsOccupied(p) <= distance;
        }
        return will_be_unoccupied;
    }

    bool GameState::isSafe(Point p, int distance) {
        return board.getCellType(p) != CellType::wall && willBeUnnocupied(p, distance);
    }

    vector<Path> GameState::bfsFood(Point start) {
        int depth  = 0;
        vector<Path> paths;
        queue<Point> q = queue<Point>();
        unordered_set<Point> visited = unordered_set<Point>();
        unordered_map<Point, Point> parent = unordered_map<Point, Point>();

        visited.insert(start);
        q.push(start);
        q.push(DEPTH_MARK);

        while (!q.empty()) {
            Point cur = q.front();
            q.pop();

            if (cur == DEPTH_MARK) {
                depth++;
                q.push(DEPTH_MARK);
                if (q.front() == DEPTH_MARK) {
                    break;// two marks end cond
                }
            } else {
                for (auto point : board.expand(cur)) {
                    if (board.in(point) && isSafe(point, depth) && visited.find(point) == visited.end()) {
                        parent[point] = cur;
                        visited.insert(point);
                        if (board.getCellType(point) == CellType::food) {
                            Path path = Path();
                            while (start != point) {
                                path.add(point);
                                point = parent[point];
                            }
                            path.add(point);
                            paths.push_back(path);
                        }
                        q.push(point);
                    }
                }
            }
        }
        return paths;
    }

    int GameState::floodFill(Point start) {
        int depth  = 0;
        queue<Point> q = queue<Point>();
        unordered_set<Point> visited = unordered_set<Point>();
        visited.insert(start);
        q.push(start);
        q.push(DEPTH_MARK);

        while (!q.empty()) {
            Point cur = q.front();
            q.pop();

            if (cur == DEPTH_MARK) {
                depth++;
                q.push(DEPTH_MARK);
                if (q.front() == DEPTH_MARK) {
                    break;// two marks end cond
                }
            } else {
                for (auto point : board.expand(cur)) {
                    if (board.in(point) && isSafe(point, depth) && visited.find(point) == visited.end()) {
                        visited.insert(point);
                        q.push(point);
                    }
                }
            }
        }
        return visited.size();
    }

    snake_index GameState::getOpponent(snake_index idx) {
        int depth = 0;
        Snake snake = getSnake(idx);
        Point start = snake.getHead();
        queue<Point> q = queue<Point>();
        unordered_set<Point> visited = unordered_set<Point>();
        visited.insert(start);
        q.push(start);
        q.push(DEPTH_MARK);

        while (!q.empty()) {
            Point cur = q.front();
            q.pop();

            if (cur == DEPTH_MARK) {
                depth++;
                q.push(DEPTH_MARK);
                if (q.front() == DEPTH_MARK) {
                    break;// two marks end cond
                }
            } else {
                for (auto point : board.expand(cur)) {
                    if (board.in(point)) {
                        if (board.cellNumOccupants(cur) > 0) {
                            snake_index occupant = *board.getCellOccupants(cur).begin();
                            if (occupant != idx) {
                                return occupant;
                            }
                        }

                        if (isSafe(point, depth) && visited.find(point) == visited.end()) {
                            visited.insert(point);
                            q.push(point);
                        }
                    }
                }
            }
        }
        return idx;
    }


    int GameState::voronoi(snake_index index) {
        int depth  = 0;
        const pair<Point, snake_index> PAIR_DEPTH_MARK;
        const snake_index MARK = -1;
        queue<pair<Point, snake_index>> q = queue<pair<Point, snake_index>>();
        unordered_map<Point, snake_index> visited = unordered_map<Point, snake_index>();
        vector<int> counts = vector<int>(snakes.size());

        for (int i = 0; i < snakes.size(); i++) {
            Snake snake = snakes[i];
            if (snake.isAlive()) {
                Point head = snake.getHead();
                pair<Point, snake_index> p = pair<Point, snake_index>(head, i);
                q.push(p);
                visited[head] = i;
            }
        }
        q.push(PAIR_DEPTH_MARK);

        while (!q.empty()) {
            pair<Point, snake_index> p = q.front();
            q.pop();

            if (p == PAIR_DEPTH_MARK) {
                depth++;
                q.push(PAIR_DEPTH_MARK);
                if (q.front() == PAIR_DEPTH_MARK) {
                    break;// two marks end cond
                }
            } else {
                Point cur = p.first;
                snake_index idx = p.second;

                for (auto point : board.expand(cur)) {
                    //in map
                    if (visited.find(point) != visited.end()) {
                        snake_index other = visited[point];
                        if (other != MARK && other != idx) {
                            counts[other]--;
                            visited[point] = MARK;
                        }
                    } else {
                        if (board.in(point) && isSafe(point, depth)) {
                            counts[idx]++;
                            pair<Point, snake_index> npair = pair<Point, snake_index>(point, idx);
                            visited[point] = idx;
                            q.push(npair);
                        }
                    }
                }
            }
        }
        return counts[index];
    }
