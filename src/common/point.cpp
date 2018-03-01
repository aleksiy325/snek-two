#pragma once

class Point {
public:
	int x;
	int y;
	Point();
	Point(int x, int y);
	Point addMove(Direction dir);
	bool operator ==(const Point& p) const;
	bool operator !=(const Point& p) const;
	bool operator < (const Point& p)  const;

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

bool Point::operator ==(const Point& p) const{
	return (x == p.x) && (y == p.y);
}

bool Point::operator !=(const Point& p) const{
	return (x != p.x) || (y != p.y);
}

bool Point::operator < (const Point& p) const{
	return (x*1000 + y) < (p.x*1000 + p.y);
}

namespace std {
	template <> 
		class hash<Point> {
		public:
			size_t operator()(const Point& p) const {
				return hash<int>()(p.x) ^ hash<int>()(p.y);
			}
	};
};