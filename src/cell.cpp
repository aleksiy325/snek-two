#pragma once

#include <unordered_set>
#include <cassert>
#include "defs.cpp"

using namespace std;

class Cell {
	unordered_set<snake_index> occupants;
public:
	CellType type;
	Cell();
	void occupy(snake_index idx);
	void vacate(snake_index);
	void vacateAll();
	size_t numOccupants();
	unordered_set<snake_index> getOccupants();
	CellType getType();
	void setType(CellType type);
	bool isOccupant(snake_index idx);
	void setFood();
};


Cell::Cell(){
	type = CellType::empty;
}

void Cell::occupy(snake_index idx){
	occupants.insert(idx);
}

void Cell::vacate(snake_index idx){
	occupants.erase(idx);
}

void Cell::vacateAll(){
	occupants.clear();
}

size_t Cell::numOccupants(){
	return occupants.size();
}

unordered_set<snake_index> Cell::getOccupants(){
	return occupants;
}

CellType Cell::getType(){
	return type;
}

void Cell::setType(CellType type){
	this->type = type;
}

bool Cell::isOccupant(snake_index idx){
	return occupants.find(idx) != occupants.end();
}

void Cell::setFood(){
	assert(type == CellType::empty);
	assert(numOccupants() == 0);
	type = CellType::food;
}