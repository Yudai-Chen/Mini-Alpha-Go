#pragma once

#include "MCTS_node.h"

class MCTS
{
private:
	//the root of the MC tree
	MCTS_node* root;

	//the turn of the root Board
	bool root_turn;

	//a memory pool, used for the rollout phase
	char * pool;

	//the number of codes on the tree
	unsigned int nodeCount;

	//the hyperparameter of MCTS algorithm
	double C;
public:
	MCTS(){}
	MCTS(const Board & board, bool _turn, double _C);
	int select();
	int expand();
	int simulate(const Board* start);
	int propagate();
	void release();
};