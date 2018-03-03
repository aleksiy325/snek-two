#pragma once

#include <iostream>


class Score{
	int ticks;
  int rank;
public:
	Score();
	Score(int ticks, int rank);
	double evalFitness();
};

Score::Score(){
	ticks = 0;
}

Score::Score(int ticks, int rank){
	this->ticks = ticks;
  this->rank = rank;
  // std::cout << "rank = " << rank << "\n";
  // std::cout << "ticks = " << ticks<< "\n";
}

double Score::evalFitness(){
	double fitness = (300.0/(rank+1.0)) + rank / 100.0;
  // std::cout << "fitness is " << fitness << "\n";
	return fitness;
}
