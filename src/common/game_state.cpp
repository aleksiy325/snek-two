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
	int getTick();
	bool isSafe(Point p, int distance);
	bool willBeUnnocupied(Point p, int distance);
	vector<Path> bfsFood(Point start);
	int floodFill(Point start);
};

// bool operator< ( Point a, Point b ) { return std::make_pair(a.x,a.y) < std::make_pair(b.x,b.y) ; }

// int GameState::voronoi(int voronoi_snake_id){
// 	map<int, int> freespace_score;
// 	set<Point> visited;
// 	queue<tuple<Point,int, int>> q;
// 	// Get all snake's head
// 	for(int i = 0; i < snakes.size(); i++){
// 		if (snakes.at(i).isAlive()){
// 			q.push(make_tuple(snakes.at(i).getHead(), i, 0));
// 			// visited.insert(snakes.at(i).getHead());
// 			freespace_score[i] = 0;
// 		// cout << "i " << i << " y x " << snakes.at(i).getHead().y << " " << snakes.at(i).getHead().x << "\n"; 					
// 		}
// 	}


// 	// Init new board
// 	// cout << "Snake size: " << snakes.size() << "\n";
// 	vector<vector<set<int>>>  voronoi_board = vector<vector<set<int>>>(board.getHeight(),vector<set<int>> (board.getWidth()));
// 	for(int i = 0; i < board.getHeight(); i ++){
// 		for(int j = 0; j < board.getWidth(); j ++){
// 			CellType  cellType = board.getCellType(Point(j,i));
// 			unordered_set<snake_index> aSet =  board.getCellOccupants(Point(j,i));
// 			if (cellType == CellType::wall){
// 				voronoi_board[i][j].insert(-3);
// 			}else if (cellType == CellType::food){
// 				// voronoi_board[i][j].insert(-2);	
// 			}else { // Empty
// 				if (aSet.size() != 0){ // Snake occupy
// 					for (const auto& elem: aSet) {
// 						// cout << elem << "*";
// 						voronoi_board[i][j].insert(elem);
// 					}
// 					// voronoi_board[i][j].insert(-3);
// 				}else{ // Really empty
// 					// voronoi_board[i][j].insert(-1);
// 				}
// 			}
// 			// if (voronoi_board[i][j].size() == 0){
// 			// 	cout << " X" << " ";
// 			// }else{
// 			// 	for (const auto& elem: voronoi_board[i][j]) {
// 			// 		cout << elem << " ";
// 			// 	}					
// 			// }
				
// 		}
// 		// cout << "\n";
// 	}

// 	// Main queue
// 	while(!q.empty()){
// 		queue<tuple<Point,int,int>> sub_q;
// 		while(!q.empty()){
// 			tuple<Point,int,int> cur = q.front();
// 			q.pop();
// 			Point point = get<0>(cur);
// 			int snake_id = get<1>(cur);
// 			if (visited.find(point) == visited.end()){
// 				visited.insert(get<0>(cur));
// 				// freespace_score[snake_id] += 1;
// 				sub_q.push(cur);
// 			}
// 			// set<int> occupantsAtPoint = voronoi_board.at(point.y).at(point.x);
// 			voronoi_board.at(point.y).at(point.x).insert(snake_id);					
// 			freespace_score[snake_id] += 1;
// 		}		
// 		while(!sub_q.empty()){
// 			tuple<Point,int,int> cur = sub_q.front();
// 			sub_q.pop();
// 			Point point = get<0>(cur);
// 			set<int> occupantsAtPoint = voronoi_board.at(point.y).at(point.x);
// 			bool isFree = true;
// 			if (occupantsAtPoint.size() > 1){ // at least 2 snakes
// 				isFree = false;
// 			}
// 			if(isFree){
// 				vector<Point> neighbors = board.expand(point);
// 				for(auto neighbor : neighbors){
// 					if(board.in(neighbor) && isSafe(neighbor) && visited.find(neighbor) == visited.end()){
// 						q.push(make_tuple(neighbor, get<1>(cur), get<2>(cur) + 1));
// 					}
// 				}				
// 			}			
// 		}

// 	}

// 	// cout << "freeSpace for index: " << voronoi_snake_id << "\n";
// 	// for (auto& t : freespace_score)
// 	// 	cout << t.first << "-" 
// 	// 			<< t.second<< "\n";	
// 	assert(freespace_score[voronoi_snake_id] != 0);
// 	return freespace_score[voronoi_snake_id];
// }



GameState::GameState(){}

GameState::GameState(int width, int height){
	board = Board(width, height);
	this->tick = 0;
}

GameState::GameState(int width, int height, int max_food){
	board = Board(width, height);
	this->max_food = max_food;
	cur_food = 0;
	for(int i = 0; i < max_food; i++){
    	addFood();
    }
    this->tick = 0;
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
			if(snake.getFreeMoves() == 0){
				old_tail = snake.popTail();
				board.vacateCell(old_tail, idx);
			}else{
				snake.useFreeMove();
			}
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
	tick++;
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

int GameState::getTick(){
	return this->tick;
}

bool GameState::willBeUnnocupied(Point p, int distance){
	// TODO: fix conditions
	bool will_be_unoccupied = true;
	if(board.cellNumOccupants(p) > 0){
		//assert(board.cellNumOccupants(p) == 1);
		snake_index occupant = *board.getCellOccupants(p).begin();
		will_be_unoccupied = snakes[occupant].getTurnsOccupied(p) <= distance;
	}
	return will_be_unoccupied;
}

bool GameState::isSafe(Point p, int distance){
	return board.getCellType(p) != CellType::wall && willBeUnnocupied(p, distance);
}

vector<Path> GameState::bfsFood(Point start){
	int depth  = 0;
	vector<Path> paths;
	queue<Point> q = queue<Point>();
	unordered_set<Point> visited = unordered_set<Point>();
	unordered_map<Point, Point> parent = unordered_map<Point, Point>();

	visited.insert(start);
	q.push(start);
	q.push(DEPTH_MARK);

	while(!q.empty()){
		Point cur = q.front();
		q.pop();

		if(cur == DEPTH_MARK){
			depth++;
			q.push(DEPTH_MARK);
			if(q.front() == DEPTH_MARK){
				break;// two marks end cond
			} 
		}else{
			for(auto point: board.expand(cur)){
				if(board.in(point) && isSafe(point, depth) && visited.find(point) == visited.end()){
					parent[point] = cur;
					visited.insert(point);
					if(board.getCellType(point) == CellType::food){
						Path path = Path();
						while(start != point){
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

int GameState::floodFill(Point start){
	int depth  = 0;
	queue<Point> q = queue<Point>();
	unordered_set<Point> visited = unordered_set<Point>();
	visited.insert(start);
	q.push(start);
	q.push(DEPTH_MARK);

	while(!q.empty()){
		Point cur = q.front();
		q.pop();

		if(cur == DEPTH_MARK){
			depth++;
			q.push(DEPTH_MARK);
			if(q.front() == DEPTH_MARK){
				break;// two marks end cond
			} 
		}else{
			for(auto point: board.expand(cur)){
				if(board.in(point) && isSafe(point, depth) && visited.find(point) == visited.end()){
					visited.insert(point);
					q.push(point);
				}
			}
		}
	}
	return visited.size();
}