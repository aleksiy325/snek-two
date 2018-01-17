#include "gtest/gtest.h"
#include "game_state.cpp"

 
TEST (CollisionTest, Wall) { 
	GameState gs = GameState(10, 10);
	Point start = Point(1, 1);
	snake_index snake_idx = gs.addSnake(start);
	Snake& snake = gs.getSnake(snake_idx);
	ASSERT_EQ(true, snake.isAlive());
	gs.makeMove(Direction::North, snake_idx);
	gs.cleanup();

	ASSERT_EQ(false, snake.isAlive());
	ASSERT_EQ(true, gs.isValid());

}

TEST (CollisionTest, EqualHead) { 
	GameState gs = GameState(10, 10);
	
	Point a = Point(1, 1);
	snake_index ia = gs.addSnake(a);

	Point b = Point(3, 1);
	snake_index ib = gs.addSnake(b);

	Snake& sa = gs.getSnake(ia);
	Snake& sb = gs.getSnake(ib);

	ASSERT_EQ(true, sa.isAlive());
	ASSERT_EQ(true, sb.isAlive());

	gs.makeMove(Direction::East, ia);
	gs.makeMove(Direction::West, ib);
	gs.cleanup();

	ASSERT_EQ(false, sa.isAlive());
	ASSERT_EQ(false, sb.isAlive());
	ASSERT_EQ(true, gs.isValid());
}


TEST (FoodTest, EatFood) { 
	GameState gs = GameState(10, 10);
	
	Point a = Point(1, 1);
	snake_index ia = gs.addSnake(a);

	Point b = Point(2, 1);
	gs.board.placeFood(b);

	Snake& sa = gs.getSnake(ia);

	ASSERT_EQ(true, sa.isAlive());	

	gs.makeMove(Direction::East, ia);
	gs.cleanup();

	ASSERT_EQ(2, sa.getSize());
	ASSERT_EQ(true, sa.isAlive());
	ASSERT_EQ(true, gs.isValid());
}

TEST (CollisionTest, EqualHeadFood) { 
	GameState gs = GameState(10, 10);
	
	Point a = Point(1, 1);
	snake_index ia = gs.addSnake(a);

	Point b = Point(3, 1);
	snake_index ib = gs.addSnake(b);

	Point c = Point(2, 1);
	gs.board.placeFood(c);

	Snake& sa = gs.getSnake(ia);
	Snake& sb = gs.getSnake(ib);

	ASSERT_EQ(true, sa.isAlive());
	ASSERT_EQ(true, sb.isAlive());

	gs.makeMove(Direction::East, ia);
	gs.makeMove(Direction::West, ib);
	gs.cleanup();

	ASSERT_EQ(false, sa.isAlive());
	ASSERT_EQ(false, sb.isAlive());
	ASSERT_EQ(true, gs.isValid());
}


TEST (CollisionTest, GreaterHead) { 
	GameState gs = GameState(10, 10);
	
	Point a = Point(1, 1);
	snake_index ia = gs.addSnake(a);

	Point b = Point(3, 1);
	snake_index ib = gs.addSnake(b);

	Point c = Point(2, 1);
	gs.board.placeFood(c);

	Snake& sa = gs.getSnake(ia);
	Snake& sb = gs.getSnake(ib);

	ASSERT_EQ(true, sa.isAlive());
	ASSERT_EQ(true, sb.isAlive());

	gs.makeMove(Direction::East, ia);
	gs.cleanup();
	
	ASSERT_EQ(2, sa.getSize());

	gs.makeMove(Direction::East, ia);
	gs.cleanup();

	ASSERT_EQ(true, sa.isAlive());
	ASSERT_EQ(false, sb.isAlive());
	ASSERT_EQ(true, gs.isValid());
}


 
int main(int argc, char **argv) {
    testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}