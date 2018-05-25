#pragma once

#include "MCTS_node.h"
#include "memory_pool.h"

class MCTS
{
private:
	//the root of the MC tree
	MCTS_node* root;

	//the turn of the root Board
	bool root_turn;

	//a memory pool, used for the rollout phase
	Memory_pool * memoryPool;

	//the number of codes on the tree
	unsigned int nodeCount;

	//the hyperparameter of MCTS algorithm
	double C;
public:
	MCTS(){}
	MCTS(const Board & board, bool _turn, double _C);
	MCTS_node *  select();
	MCTS_node *  expand(MCTS_node * needExpand);
	double simulate(MCTS_node* start);
	int propagate(MCTS_node* start, double simulateResult);
	void release();
	Coord search();
};