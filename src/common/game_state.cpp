#pragma once

#include <vector>
#include <random>
#include <algorithm>
#include "board.cpp"
#include "snake.cpp"

using namespace std;


class GameState {
	int cur_food;
public:
	Board board;
	vector<Snake> snakes;
	int max_food;
	GameState();
	GameState(int height, int width);
	GameState(int height, int width, int num_snakes);
	void makeMove(Direction dir, snake_index idx);
	snake_index addSnake();
	snake_index addSnake(Point start);
	void addFood();
	Snake& getSnake(snake_index idx);
	void removeFood(Point p);
	void checkCollision(Point cur_point);
	void cleanup();
	bool isValid();
	Board getBoard();
	int getHeight();
	int getWidth();
	vector<Snake> getSnakes();
};


GameState::GameState(){}

GameState::GameState(int width, int height){
	board = Board(width, height);
}

GameState::GameState(int width, int height, int max_food){
	board = Board(width, height);
	this->max_food = max_food;
	cur_food = 0;
	for(int i = 0; i < max_food; i++){
    	addFood();
    }
}

snake_index GameState::addSnake(){
	Point start = board.getRandomEmptyPoint();
	Snake snake = Snake(start);
	snake_index idx = snakes.size();
	board.occupyCell(start, idx);
	snakes.push_back(snake);
	return idx;
}

void GameState::addFood(){
	Point p = board.getRandomEmptyPoint();
	board.placeFood(p);
	cur_food++;
}

snake_index GameState::addSnake(Point start){
	Snake snake = Snake(start);
	snake_index idx = snakes.size();
	board.occupyCell(start, idx);
	snakes.push_back(snake);
	return idx;
}

Snake& GameState::getSnake(snake_index idx){
	return snakes[idx];
}

void GameState::makeMove(Direction dir, snake_index idx){
	Snake& snake = snakes[idx];
	Point head = snake.makeMove(dir);
	snake.loseHealth();

	//check self collision before occupy
	if(board.isOccupantOf(head, idx)){
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
			old_tail = snake.popTail();
			board.vacateCell(old_tail, idx);
			break;
	}
}

void GameState::checkCollision(Point cur_point){
	if(board.cellNumOccupants(cur_point) > 1){
		unordered_set<snake_index> occupants = board.getCellOccupants(cur_point);
		vector<snake_index> heads;
		vector<size_t> head_lengths;

		for(auto& s_index: occupants){
			Snake cur_snake = snakes[s_index];
			Point head = cur_snake.getHead();
			if(cur_point == head){
				heads.push_back(s_index);
				head_lengths.push_back(cur_snake.getSize());
			}
		}

		if(heads.size() > 1){
			size_t max_len = *max_element(head_lengths.begin(), head_lengths.end());
			int count_max = count(head_lengths.begin(), head_lengths.end(), max_len);

			if(count_max > 1){ // if more than one are max everyone dies
				max_len = -1;
			}

			for(auto& s_index: heads){
				Snake& cur_snake = snakes[s_index];
				if(cur_snake.getSize() != max_len){
					cur_snake.setAlive(false);
				}
			}

		}else{
			for(auto& s_index: heads){
				snakes[s_index].setAlive(false);
			}
		}
	}
}

void GameState::removeFood(Point p){
	if(board.getCellType(p) == CellType::food){
		board.setCellType(p, CellType::empty);
		cur_food--;
	}
}

void GameState::cleanup(){
	// check collisions
	for(auto& snake: snakes){
		if(snake.getHealth() <= 0){
			snake.setAlive(false);
		}
		if(snake.isAlive()){
			Point cur_point = snake.getHead();
			checkCollision(cur_point);
			removeFood(cur_point);
		}
	}

	// clean up dead snakes
	for(snake_index i = 0; i < snakes.size(); i++){
		Snake& snake = snakes[i];
		if(!snake.isAlive()){
			deque<Point> points = snake.getPoints();
			for(auto point: points){
				board.vacateCell(point, i);
			}
			snake.clearPoints();
		}
	}

	while(cur_food < max_food){
		addFood();
	}

	if(!isValid()){
		board.print();
	}
	//assert(isValid());
}

bool GameState::isValid(){
	if(cur_food != max_food){
		cout << cur_food << " != " << max_food << endl;
		return false;
	}

	if(!board.isValid()){
		cout << "Board is not valid" << endl;
		return false;
	}

	for(snake_index i = 0; i < snakes.size(); i++){
		Snake& snake = snakes[i];
		deque<Point> points = snake.getPoints();
		for(auto point: points){
			if(snake.isAlive() && !board.isOccupantOf(point, i)){
				cout << "snake is alive but doesnt occupy point" << endl;
				return false;
			}
		}
	}
	return true;
}

Board GameState::getBoard(){
	return board;
}

int GameState::getHeight(){
	return board.getHeight();
}

int GameState::getWidth(){
	return board.getWidth();
}

vector<Snake> GameState::getSnakes(){
	return snakes;
}

