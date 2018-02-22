#pragma once


class Score{
public:
	int ticks;
	bool isWinner;
	Score();
};

Score::Score(){
	ticks = 0;
	isWinner = false;
}