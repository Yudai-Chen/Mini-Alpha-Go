#pragma once

#include"board.h"

class MCTS_node
{
private:
	Board board;

	/* MCTS structure */
	MCTS_node* parent;         //parent node
	MCTS_node* child[30];      //child node
	
	/* MCTS value */
	Coord move;
	int visitTime;
	double winTime[2];
	double quality;
	unsigned short childCount; //number of child nodes
	std::vector<Coord> unvisited;

	

public:
	MCTS_node(){}

	//initial the root
	MCTS_node(const Board & board);

	//initial a child
	MCTS_node(MCTS_node* parent, const Coord & move);

	MCTS_node* getBestChild(double C);

	double getChildScore(MCTS_node * child, double C);

	double getScoreAfterMCTS();

	inline bool hasUnexpandedChild() { return unvisited.size(); }

	Coord popUnvisitied();

	double rollout() const;

	void changeVisitTimeAndWinTime(double score);

	inline MCTS_node * getParent() const { return parent; }

	Coord choiceAfterMCTS() const;

	inline Coord getMove() const { return move; }

	void release();

	inline bool isTerminal() const { return board.isTerminal() < UNFINISHED; }

	inline bool shouldPass() const { return board.shouldPass(); }
};

