#include "../GALGO-2.0/src/Galgo.hpp"
#include "../GALGO-2.0/src/Evolution.hpp"
#include "strategies/random_snake.cpp"
#include "strategies/eat_snake.cpp"
#include "strategies/heuristic_snake.cpp"
#include "arena/game.cpp"

const int width = 15;
const int height = 15;
const int max_food = 10;
const int num_train_snakes = 10;
const int iterations = 100;
const int game_iters = 10;

template <typename T>
class MaxScore
{
public:
    static std::vector<std::vector<T>> Objective(const std::vector<std::vector<T>>& snake_params)
    {
        std::vector<std::vector<T>> result(num_train_snakes, std::vector<T>(1, 0.0));
        RandomSnake rsnake = RandomSnake{};
        EatSnake esnake = EatSnake{};

        for (int j = 0; j < game_iters; j++) {
            Game game = Game(width, height, max_food);
            game.addStrategy(&rsnake);
            game.addStrategy(&rsnake);
            game.addStrategy(&esnake);
            game.addStrategy(&esnake);


            //leaking memory fix later
            for (auto params : snake_params) {
                game.addStrategy(new HeuristicSnake(params[0], params[1], params[2], params[3]));
            }

            game.execute();

            std::vector<Score> scores = game.getScores();
            for (int i = scores.size() - num_train_snakes; i < scores.size(); i++) {
                result[i - (scores.size() - num_train_snakes)][0] += scores[i].evalFitness() / game_iters;
            }
        }
        return result;
    }
};

int main()
{
    galgo::Parameter<double> health_weight({ -10.0, 10.0});
    galgo::Parameter<double> food_weight({ -10.0, 10.0});
    galgo::Parameter<double> length_weight({ -10.0, 10.0});
    galgo::Parameter<double> free_weight({ -10.0, 10.0});
    galgo::GeneticAlgorithm<double> ga(MaxScore<double>::Objective, num_train_snakes, iterations, true, health_weight, food_weight, length_weight, free_weight);
    ga.Selection = TRS;
    ga.CrossOver = P1XO;
    ga.Mutation = SPM;
    ga.covrate = 0.1;
    ga.mutrate = 0.05;
    ga.elitpop = 2;
    ga.run();
}
