#pragma once

#include"board.h"

class MCTS_node
{
private:
	Board* board;

	/* MCTS structure */
	MCTS_node* parent;         //parent node
	MCTS_node* child[30];      //child node
	unsigned short childCount; //number of child nodes

	/* MCTS value */
	int visitTime;
	int winTime[2];
	double quality;

public:
	MCTS_node(){}
	MCTS_node(const Board & board);
	void release();
};

