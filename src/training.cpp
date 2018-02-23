#include "../GALGO-2.0/src/Galgo.hpp"
#include "../GALGO-2.0/src/Evolution.hpp"
#include "strategies/random_snake.cpp"
#include "strategies/eat_snake.cpp"
#include "strategies/heuristic_snake.cpp"
#include "arena/game.cpp"

const int width = 15;
const int height = 15;
const int max_food = 5;
const int num_train_snakes = 6;
const int iterations = 3000;


template <typename T>
class MaxScore
{
public:
   static std::vector<std::vector<T>> Objective(const std::vector<std::vector<T>>& snake_params)
   {  
      Game game = Game(width, height, max_food);
      RandomSnake rsnake = RandomSnake{};
      EatSnake esnake = EatSnake{};
      game.addStrategy(&rsnake);
      game.addStrategy(&rsnake);
      game.addStrategy(&esnake);
      game.addStrategy(&esnake);

      std::vector<HeuristicSnake> strategies;
      for(auto params: snake_params){
         HeuristicSnake hsnake = HeuristicSnake(params[0], params[1], params[2], params[3]);
         strategies.push_back(hsnake);
      } 
      for(auto strat: strategies){
         game.addStrategy(&strat);
      } 

      game.execute();      

      std::vector<Score> scores = game.getScores();
      std::vector<std::vector<T>> result;

      for(int i = scores.size() - num_train_snakes; i < scores.size(); i++){
         std::vector<double> fitness;
         fitness.push_back(scores[i].evalFitness());
         result.push_back(fitness);
      }

      return result;
   }
};

int main()
{
   galgo::Parameter<double> health_weight({-10.0,10.0, 6.53956});
   galgo::Parameter<double> food_weight({-10.0,10.0, 9.54376});
   galgo::Parameter<double> length_weight({-10.0,10.0, -7.61898});
   galgo::Parameter<double> free_weight({-10.0,10.0, 10.0});
   galgo::GeneticAlgorithm<double> ga(MaxScore<double>::Objective, num_train_snakes, iterations, true, health_weight, food_weight, length_weight, free_weight);
   ga.Selection = TRS;
   ga.CrossOver = P1XO;
   ga.Mutation = SPM;
   ga.run();
}
