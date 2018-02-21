#include <deque>

class Path{
public:
	deque<Point> path;
	void add(Point p);
};

void Path::add(Point p){
	path.push_front(p);
}