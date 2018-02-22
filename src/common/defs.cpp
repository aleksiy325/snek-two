#pragma once

#include <map>

using namespace std;

typedef int snake_index;

enum class CellType {
	empty, 
	wall, 
	food
};

enum class Direction {
	North,
	South,
	West,
	East	
};

const vector<Direction> DIRECTIONS = {Direction::North, Direction::South, Direction::West, Direction::East};
const int MAX_HEALTH = 100;
const int HEALTH_LOSS = 1;

map<CellType, string> CELL_STR_MAP {
	{CellType::empty, "  "},
	{CellType::wall, "X "},
	{CellType::food, "f "}
};