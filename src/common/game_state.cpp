#pragma once

#include <vector>
#include <random>
#include <algorithm>
#include "board.cpp"
#include "snake.cpp"


#include <map>
#include <set>
#include <cassert>

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
	void printScoreBoard();
	int voronoi(int voronoi_snake_id);
};

// bool operator< ( Point a, Point b ) { return std::make_pair(a.x,a.y) < std::make_pair(b.x,b.y) ; }

int GameState::voronoi(int voronoi_snake_id){
	map<int, int> freespace_score;
	set<Point> visited;
	queue<Point> q;
	// Get all snake's head
	vector<Point> snakes_head;
	for(int i = 0; i < snakes.size(); i++){
		snakes_head.push_back(snakes.at(i).getHead());
		q.push(snakes.at(i).getHead());
		freespace_score[i] = 0;
		cout << "i " << i << " y x " << snakes.at(i).getHead().y << " " << snakes.at(i).getHead().x << "\n"; 
	}

	cout << "freeSpace: \n";
	for (auto& t : freespace_score)
		cout << t.first << "-" 
				<< t.second<< "\n";		
	// Init new board

	cout << "Snake size: " << snakes.size() << "\n";
	vector<vector<set<int>>>  voronoi_board = vector<vector<set<int>>>(board.getHeight(),vector<set<int>> (board.getWidth()));
	for(int i = 0; i < board.getHeight(); i ++){
		for(int j = 0; j < board.getWidth(); j ++){
			CellType  cellType = board.getCellType(Point(j,i));
			unordered_set<snake_index> aSet =  board.getCellOccupants(Point(j,i));
			if (cellType == CellType::wall){
				voronoi_board[i][j].insert(-3);
			}else if (cellType == CellType::food){
				voronoi_board[i][j].insert(-2);	
			}else {
				if (aSet.size() != 0){
					cout << ">";
					for (const auto& elem: aSet) {
						cout << elem << "*";
					}
					cout << "<";
				}else{
					voronoi_board[i][j].insert(-1);
				}
			}
			for (const auto& elem: voronoi_board[i][j]) {
				cout << elem << " ";
			}					
		}
		cout << "\n";
	}

	set<Point>::iterator it = visited.find(Point(0,0));
	while(!q.empty()){
		Point cur = q.front();
		q.pop();
		for (auto point : board.expand(cur)){
			if(board.in(point)
			&& board.isSafe(point) 

			&& visited.find(point) == visited.end()){
				
				visited.insert(point);
				int snake_idx; 
				// for(unordered_set<snake_index>::iterator it =  board.getCellOccupants(point).begin(); 
				// 	it != board.getCellOccupants(point).end(); ++it){
				// 	snake_idx = *it;
				// 	break;
				// }
				for (const auto& elem: board.getCellOccupants(point)) {
					snake_idx = elem;
				}				

				freespace_score[snake_idx] += 1;
				set<int> occupantsAtPoint = voronoi_board.at(point.y).at(point.x);

				if (occupantsAtPoint.size() > 2){ // already 2 things
					for (const auto& elem: occupantsAtPoint) {
						if (elem >= 0){
							// freespace_score[elem] -= 1;
							cout << "DELETED\n";							
						}
					}					
					// for(set<int>::iterator it =  occupantsAtPoint.begin(); it != occupantsAtPoint.end(); ++it){
					// 	if (*it >= 0){
					// 		freespace_score[snake_idx] -= 1;
					// 		cout << "DELETED\n";
					// 	}
					// }
				}else{
					
					vector<Point> neighbors = board.expand(point); 
					for(auto point : neighbors){
						q.push(point);
					}
					// if (board.getCellType(point) == CellType::food){
					// 	closestFood[point] = snake_idx;
					// }
					// if(board.getCellType(point) == CellType::wall){
					// 	int snake_id_at_wall;
					// 	for(unordered_set<snake_index>::iterator it =  board.getCellOccupants(point).begin(); 
					// 		it != board.getCellOccupants(point).end(); ++it){
					// 		snake_id_at_wall = *it;
					// 		break;
					// 	}
					// 	// TODO: change this if add the logic for remove tail
					// 	int ttl = snakes.at(snake_id_at_wall).getIndexOfPoint(point);
					// 	if (ttl > )
					// }
				}
				occupantsAtPoint.insert(snake_idx);
			}
		}
	}
	cout << "freeSpace: \n";
	for (auto& t : freespace_score)
		cout << t.first << "-" 
				<< t.second<< "\n";	
	assert(freespace_score[voronoi_snake_id] != 0);
	return freespace_score[voronoi_snake_id];
}



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
	assert(idx < snakes.size() && idx >= 0);
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

		for(auto s_index: occupants){
			Snake cur_snake = snakes[s_index];
			Point head = cur_snake.getHead();
			if(cur_point == head){
				heads.push_back(s_index);
				head_lengths.push_back(cur_snake.getSize());
			}
		}

		assert(head_lengths.size() > 0);
		assert(head_lengths.size() == heads.size());

		// if all occupants are heads and there is more than on head
		if(heads.size() == board.cellNumOccupants(cur_point) && heads.size() > 1){
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
			for(auto s_index: heads){
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
	
	assert(isValid());
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

void GameState::printScoreBoard(){
	int i = 0;
	for(auto snake: snakes){
		Point head = snake.getHead();
		cout << i << " " << snake.getHealth() << " " << snake.isAlive() << " " << head.x << "," << head.y << endl;
		i++;
	}
}

