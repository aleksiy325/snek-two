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

const int MAX_HEALTH = 100;

map<CellType, string> CELL_STR_MAP {
	{CellType::empty, "  "},
	{CellType::wall, "X "},
	{CellType::food, "f "}
};