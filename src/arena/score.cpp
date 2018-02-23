#pragma once


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
}

double Score::evalFitness(){
	double fitness = -rank + (ticks / 1000.0);
	return fitness;
}
