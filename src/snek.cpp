#include "strategies/random_snake.cpp"
#include "strategies/eat_snake.cpp"
#include "arena/arena.cpp"

int main(int argc, char **argv){
	Arena arena = Arena(10, 10, 5, 1000);

    RandomSnake rsnake = RandomSnake{};
    Strategy* strat = &rsnake;
    EatSnake esnake = EatSnake{};
    Strategy* estrat = &esnake;

    arena.addStrategy(strat);
    arena.addStrategy(strat);
    arena.addStrategy(estrat);
    arena.addStrategy(estrat);

    arena.execute();
}
