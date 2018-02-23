#pragma once

#include <vector>
#include <iostream>
#include <string>
#include <queue>
#include <unordered_set>
#include <unordered_map>
#include "cell.cpp"
#include "point.cpp"
#include "path.cpp"

using namespace std;

class Board {
public:
	vector<vector<Cell>> board;
	int width;
	int height;
	Board();
	Board(int width, int height);
	void clear();
	void print();
	Point getRandomEmptyPoint();
	void occupyCell(Point p, snake_index idx);
	void vacateCell(Point p, snake_index idx);
	size_t cellNumOccupants(Point p);
	CellType getCellType(Point p);
	void setCellType(Point p, CellType type);
	unordered_set<snake_index> getCellOccupants(Point p);
	bool isOccupantOf(Point p, snake_index idx);
	vector<Point> getPoints();
	bool isValid();
	void placeFood(Point p);
	size_t getWidth();
	size_t getHeight();
	bool isSafe(Point p);
	bool in(Point p);
	vector<Point> expand(Point p);
	vector<Path> bfsFood(Point start);
	int floodFill(Point start);
};


Board::Board(){
}

Board::Board(int width, int height) {
	board.resize(height + 2);
	for(int i = 0; i < height + 2; i++){
		board[i].resize(width + 2);
	}
	clear();
}

void Board::clear(){
	for(auto y = board.begin(); y != board.end(); y++){
		for(auto x = y->begin(); x != y->end(); x++){
			if(x == y->begin() || x == y->end() - 1 || y == board.begin() || y == board.end() - 1){
				x->setType(CellType::wall);
				x->vacateAll();
			}else{
				x->setType(CellType::empty);
				x->vacateAll();
			}
		}
	}
}

void Board::print(){
	for (auto row : board) {
		for (auto cell : row) {
			if(cell.numOccupants() > 0){
				cout << *cell.getOccupants().begin() << ' ';
			}else{
				cout << CELL_STR_MAP[cell.getType()];
			}
		}
		cout << endl;
	}
}

Point Board::getRandomEmptyPoint(){
	int x;
	int y;
	do{
		x = rand() % board.size();
		y = rand() % board[x].size();
	}while(board[y][x].getType() != CellType::empty || board[y][x].numOccupants() != 0);
	return Point(x, y);
}

void Board::occupyCell(Point p, snake_index idx){
	board[p.y][p.x].occupy(idx);
}

void Board::vacateCell(Point p, snake_index idx){
	board[p.y][p.x].vacate(idx);
}

size_t Board::cellNumOccupants(Point p){
	return board[p.y][p.x].numOccupants();
}

CellType Board::getCellType(Point p){
	return board[p.y][p.x].getType();
}

void Board::setCellType(Point p, CellType type){
	board[p.y][p.x].setType(type);
}

unordered_set<snake_index> Board::getCellOccupants(Point p){
	return board[p.y][p.x].getOccupants();
}

bool Board::isOccupantOf(Point p, snake_index idx){
	return board[p.y][p.x].isOccupant(idx);
}

vector<Point> Board::getPoints(){
	vector<Point> points;
	for(auto y = board.begin(); y != board.end(); y++){
		for(auto x = y->begin(); x != y->end(); x++){
			Point p = Point(x - y->begin(), y - board.begin());
			points.push_back(p);
		}
	}
	return points;
}

bool Board::isValid(){
	for (auto row : board) {
		for (auto cell : row) {
			if(cell.numOccupants() > 1){
				cout << "Num occupants is greater than 1" << endl;
				return false;
			}
			if(cell.numOccupants() == 1 && cell.getType() != CellType::empty){
				cout << "Num occupants is 1 but celltype is not empty" << endl;
				return false;
			}
		}
	}
	return true;
}

void Board::placeFood(Point p){
	board[p.y][p.x].setFood();
}

size_t Board::getWidth(){
	return board.size();
}

size_t Board::getHeight(){
	return board[0].size();
}

bool Board::isSafe(Point p){
	return board[p.y][p.x].getType() != CellType::wall && cellNumOccupants(p) == 0;
}

bool Board::in(Point p){
	return p.y >= 0 && p.y < board.size() && p.x >= 0 && p.x < board[p.y].size();
}

vector<Point> Board::expand(Point p) {
	vector<Point> neighbours = vector<Point>();
	for (auto d : DIRECTIONS) {
		Point n = p.addMove(d);
		if(in(n)){
			neighbours.push_back(n);
		}
	}
	return neighbours;
}

vector<Path> Board::bfsFood(Point start){
	vector<Path> paths;
	queue<Point> q = queue<Point>();
	unordered_set<Point> visited = unordered_set<Point>();
	unordered_map<Point, Point> parent = unordered_map<Point, Point>();
	visited.insert(start);
	q.push(start);
	while(!q.empty()){
		Point cur = q.front();
		q.pop();
		for(auto point: expand(cur)){
			if(in(point) && isSafe(point) && visited.find(point) == visited.end()){
				parent[point] = cur;
				visited.insert(point);
				if(getCellType(point) == CellType::food){
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
	return paths;
}

int Board::floodFill(Point start){
	queue<Point> q = queue<Point>();
	unordered_set<Point> visited = unordered_set<Point>();
	visited.insert(start);
	q.push(start);
	while(!q.empty()){
		Point cur = q.front();
		q.pop();
		for(auto point: expand(cur)){
			if(in(point) && isSafe(point) && visited.find(point) == visited.end()){
				visited.insert(point);
				q.push(point);
			}
		}
	}
	return visited.size();
}