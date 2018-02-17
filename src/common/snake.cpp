#pragma once

#include <deque>
#include "point.cpp"

using namespace std;

class Snake {
	bool alive;
	int health;
public: 
	deque<Point> points;
	Snake(Point start);
	Point getHead();
	bool isAlive();
	void setAlive(bool alive);
	Point makeMove(Direction dir);
	Point popTail();
	size_t getSize();
	deque<Point> getPoints();
	void clearPoints();
};


Snake::Snake(Point start){
	health = MAX_HEALTH;
	setAlive(true);
	points.push_back(start);
}

Point Snake::getHead(){
	return *points.begin();
}

bool Snake::isAlive(){
	return alive;
}

void Snake::setAlive(bool alive){
	this->alive = alive;
}

Point Snake::makeMove(Direction dir){
	Point head = *points.begin();
	Point new_head = head.addMove(dir);
	points.push_front(new_head);
	return new_head;
}

Point Snake::popTail(){
	Point back = points.back();
	points.pop_back();
	return back;
}


size_t Snake::getSize(){
	return points.size();
}

deque<Point> Snake::getPoints(){
	return points;
}

void Snake::clearPoints(){
	points.clear();
}
