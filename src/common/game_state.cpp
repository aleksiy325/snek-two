#pragma once

#include <vector>
#include <random>
#include <algorithm>
#include "board.cpp"
#include "snake.cpp"

using namespace std;


class GameState {
public:
	Board board;
	vector<Snake> snakes;
	GameState(int height, int width);
	GameState(int height, int width, int num_snakes);
	void makeMove(Direction dir, snake_index idx);
	snake_index addSnake();
	snake_index addSnake(Point start);
	Snake& getSnake(snake_index idx);
	void checkCollision(Point cur_point);
	void cleanup();
	bool isValid();
};



GameState::GameState(int width, int height){
	board = Board(width, height);
}

GameState::GameState(int width, int height, int num_snakes){
	board = Board(width, height);
	for(int i = 0; i < num_snakes; i++){
		addSnake();
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
	board.occupyCell(head, idx);
	CellType type = board.getCellType(head);
	Point old_tail;

	switch (type) {
		case CellType::food:
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
			if(cur_point.compare(head)){
				heads.push_back(s_index);
				head_lengths.push_back(cur_snake.getSize());
			}
		}

		if(heads.size() > 1){
			size_t max_len = *max_element(head_lengths.begin(), head_lengths.end());
			int count_max = count(head_lengths.begin(), head_lengths.end(), max_len);
			cout << max_len << endl;

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

void GameState::cleanup(){
	for(auto& snake: snakes){
		if(snake.isAlive()){
			Point cur_point = snake.getHead();
			checkCollision(cur_point);
			if(board.getCellType(cur_point) == CellType::food){
				board.setCellType(cur_point, CellType::empty);
			}
		}
	}

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
}

bool GameState::isValid(){
	if(!board.isValid()){
		return false;
	}

	for(snake_index i = 0; i < snakes.size(); i++){
		Snake& snake = snakes[i];
		deque<Point> points = snake.getPoints();
		for(auto point: points){
			if(snake.isAlive() && !board.isOccupantOf(point, i)){
				return false;
			}
		}
	}
	return true;
}
