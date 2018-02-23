#pragma once


class Score{
	int ticks;
	bool isWinner;
public:
	Score();
	Score(int ticks, bool isWinner);
	double evalFitness();
};

Score::Score(){
	ticks = 0;
	isWinner = false;
}

Score::Score(int ticks, bool isWinner){
	this->ticks = ticks;
	this->isWinner = isWinner;
}

double Score::evalFitness(){
	double fitness = ticks;
	if(isWinner){
		fitness += 0;
	}
	return fitness;
}
