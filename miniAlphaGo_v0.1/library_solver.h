#pragma once
#include "library_reader_factory.h"

class Library_tree_node
{
public:
	Coord move;
	int solvedBlackCount;
	int visitTime;
	int winTime[2];
	Library_tree_node * nextMove[30];
	short nextMoveCount;
public:
	Library_tree_node() 
	{ 
		nextMoveCount = 0; 
		move = Coord(-1, -1);
		for (int i = 0; i < 30; i++)
			nextMove[i] = nullptr;
	};
	Library_tree_node(Coord & move)
	{
		this->move = move;
		solvedBlackCount = 0;
		visitTime = 0;
		winTime[0] = 0;
		winTime[1] = 0;
		nextMoveCount = 0;
		for (int i = 0; i < 30; i++)
			nextMove[i] = nullptr;
	}
	void release();
};

class Library_solver
{
private:
	Library_tree_node * mainState;
	Library_tree_node * root;
	short turnID;
	int similar;
private:
	void transformSimilar(Coord & input);
public:
	Library_solver()
	{
		root = new Library_tree_node();
		mainState = root;
		turnID = 1;
		similar = 0;
	}
	void buildLibraryTree(const Library_reader_factory &);
	Coord getBestMoveAndChangeState();
	int changeState(Coord & coord, short nextMoveIndex);	//if comes from outside, nextMoveIndex = -1
	void releaseLibraryTree();
};
