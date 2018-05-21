#include "MCTS.h"

MCTS::MCTS(const Board & board, bool _turn, double _C)
{
	root = new MCTS_node(board);

	root_turn = _turn;

	pool = (char*)malloc(sizeof(MCTS_node) * 1000);

	nodeCount = 0;

	C = _C;
}

int MCTS::select()
{
	return 0;
}

int MCTS::expand()
{
	return 0;
}

int MCTS::simulate(const Board* start)
{
	return 0;
}

int MCTS::propagate()
{
	return 0;
}

void MCTS::release()
{
	root->release();
	delete[] pool;
}