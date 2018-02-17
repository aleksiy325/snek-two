#pragma once

class Point {
public:
	int x;
	int y;
	Point();
	Point(int x, int y);
	Point addMove(Direction dir);
	bool compare(Point p);
};


Point::Point(){
	x = 0;
	y = 0;
}

Point::Point(int x, int y): x(x), y(y){
}

Point Point::addMove(Direction dir){
	Point p = Point(x, y);
	switch (dir) {
		case Direction::North:
			p.y--;
			break;
		case Direction::East:
			p.x++;
			break;
		case Direction::South:
			p.y++;
			break;
		case Direction::West:
			p.x--;
			break;
	}
	return p;
}

bool Point::compare(Point p) {
	return (x == p.x) && (y == p.y);
}
