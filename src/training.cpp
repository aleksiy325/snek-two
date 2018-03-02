#include "../GALGO-2.0/src/Galgo.hpp"
#include "../GALGO-2.0/src/Evolution.hpp"
#include "strategies/random_snake.cpp"
#include "strategies/eat_snake.cpp"
#include "strategies/heuristic_snake.cpp"
#include "arena/game.cpp"
#include <fstream>
#include <string>
#include <random>

const int width = 20;
const int height = 20;
const int max_food = 10;
const int num_train_snakes = 8;
const int iterations = 100;
const int game_iters = 10;

static int g_iter_count = 0;
static int g_train_id;

void saveSnakeParams(vector<float> params){
  std::string fname = "saves/"
    + std::to_string(g_train_id) + "T"
    + std::to_string(g_iter_count) + "I.snk";
  ofstream result_file(fname);

  for(float param : params){
    result_file << param << "\n";
  }
}

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
              int indexAdj = i - (scores.size() - num_train_snakes);
              result[indexAdj][0] += scores[i].evalFitness() / game_iters;

              // save the winning snake's params
              // if(game.isWinner(i)){
              //   std::vector<float> params;
              //   params.push_back(snake_params[indexAdj][0]);
              //   params.push_back(snake_params[indexAdj][1]);
              //   params.push_back(snake_params[indexAdj][2]);
              //   params.push_back(snake_params[indexAdj][3]);
              //   saveSnakeParams(params);
              // }
              delete game.strategies[i];
            }
        }
        g_iter_count++;
        return result;
    }
};

int main()
{
    // used in file name
    srand(time(NULL));
    g_train_id = rand();
    // galgo::Parameter<double> health_weight({ -10.0, 10.0});
    galgo::Parameter<double> food_weight({ -10.0, 10.0});
    galgo::Parameter<double> length_weight({ -10.0, 10.0});
    galgo::Parameter<double> free_weight({ -10.0, 10.0});
    galgo::Parameter<double> food_exp({ 0.05, 1.0});
    galgo::GeneticAlgorithm<double> ga(MaxScore<double>::Objective, num_train_snakes, iterations, true, food_weight, food_exp, length_weight, free_weight);
    ga.Selection = TRS;
    ga.CrossOver = P1XO;
    ga.Mutation = SPM;
    ga.covrate = 0.1;
    ga.mutrate = 0.05;
    ga.elitpop = 2;
    ga.run();
}
