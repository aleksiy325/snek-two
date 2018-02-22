#include <deque>

class Path{
public:
	deque<Point> path;
	void add(Point p);
	bool exists();
	Direction getStepDir(int step);
};

void Path::add(Point p){
	path.push_front(p);
}

bool Path::exists(){
	return path.size() > 1;
}

Direction Path::getStepDir(int step) {
	assert(step >= 0 && step < path.size() - 1);
	Point one = path[step];
	Point two = path[step + 1];

	int x = two.x - one.x;
	int y = two.y - one.y;

	assert(x != 0 || y != 0);

	if (y == -1) {
		return Direction::North;
	} else if (y == 1) {
		return Direction::South;
	} else if (x == 1) {
		return Direction::East;
	}
	return Direction::West;
}