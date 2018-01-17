#include "game_state.cpp"

int main(int argc, char **argv){
	GameState gs = GameState(10, 10, 0);
	gs.addSnake(Point(1, 1));
	gs.addSnake(Point(6, 5));
	gs.board.print();
	gs.makeMove(Direction::North, 0);
	gs.cleanup();
	gs.board.print();
	gs.makeMove(Direction::East, 0);
	gs.cleanup();
	gs.board.print();
}
