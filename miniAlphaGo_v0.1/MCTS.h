#pragma once

#include<iostream>
#include"Board.h"

class MCTS
{
private:
	int player;
	double C;
	int num;  //num of objects allocated in current memory pool

public:
	char* pool;   //memory pool
	Board* root;  //root node of MCTS tree
	MCTS() {};
	MCTS(double p[4], char state[8][8], int player, double _C);
	Board* search();   //search for the best move
	Board* treePolicy();
	void release(); //release the memory of this tree
	int defaultPolicy(Board* curState);
};
