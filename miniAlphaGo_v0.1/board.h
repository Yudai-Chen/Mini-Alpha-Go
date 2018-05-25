#pragma once

#include<iostream>
#include<cstring>
#include<cassert>
#include<bitset>
#include<vector>

#include"elements.h"
#include"position.h"

/*
* model of the board
*/
class Board
{
private:
	/* the characters of a board on a time slide */
	bool turn;
	Position position[10][10]; //the status of each position

	std::bitset<100> boundary; //the boundary of both sides
	std::bitset<100> sideBoundary[2]; // the boundary of each side

	short stateCount[3];  //the number of each state

	std::vector<Coord> validPositionVector;

	bool hasPast[2];

private:
	/* Game rules */
	inline short bitoffset(short x, short y) const { return (x * 10 + y); }	//transform (x,y) to a number
	inline Coord bitdeoffset(short x) const { return std::pair<short, short>(x / 10, x % 10); } //transform a number to a coordination

	void setValidPosition();

	//is (x, y) on the abstract board
	inline bool onAbstractBoard(short x, short y) const { return (x >= 0 && x <= 10 && y >= 0 && y <= 10); }

	//is (x, y) on the solid board
	inline bool onSolidBoard(short x, short y) const { return (x >= 1 && x <= 8 && y >= 1 && y <= 8); }
	inline bool onSolidBoard(Coord &coord) const { return onSolidBoard(coord.first, coord.second); }


	//set the boundary of a side
	void setSideBoundary(bool side);

	//set the boundary of both sides
	void setBoundary();

	//is (x, y) is a position in a corner
	inline bool isCorner(short x, short y) const {
		return ((x == 1 && y == 1) ||
			(x == 1 && y == 8) ||
			(x == 8 && y == 1) ||
			(x == 8 && y == 8));
	}

	//is (x, y) is a position on an edge
	inline bool isEdge(short x, short y)  const {
		return (((x == 1 || x == 8) && y >1 && y < 8) ||
			((y == 1 || y == 8) && x >1 && x < 8));
	}

	//is (x, y) a valid position for a side. Before call it, you should confirm (x, y) is a empty position
	//not a const function, because it changes the valid_dir of a position
	bool isValid(short x, short y, bool side);

public:
	Board();

	//print current boundary, for debugging
	void printBoundary() const;

	void putPiece(const Coord &coord);

	inline bool whosTurn() const { return turn; }

	inline short getPieceCount(bool side) const { return stateCount[side]; }

	//print valid move, for debugging
	void printValid() const ;

	//print board, for debugging
	void printBoard() const;

	inline const bool isCertainStepValid(Coord &coord)
	{
		std::vector<Coord>::iterator it;
		it = std::find(validPositionVector.begin(), validPositionVector.end(), coord);
		return it != validPositionVector.end();
	}

	//after one side put a piece, call this to check. 
	//This time the turn has already been flipped.
	inline const Result isTerminal() const
	{
		//when the other side has just put a piece, it's only possible that my pieces have run out	
		if ((stateCount[WHITE] + stateCount[BLACK] == 64) || (hasPast[0] && hasPast[1]))
		{
			if (stateCount[WHITE] == stateCount[BLACK])
				return DRAW;
			return stateCount[WHITE] > stateCount[BLACK] ? WHITE_WIN : BLACK_WIN;
		}
		if (!stateCount[turn])
		{
			return Result(!turn);
		}
		return UNFINISHED;
	}

	// after isTerminal() has been called, call this to check if current side should pass
	inline const bool shouldPass() const
	{
		return !stateCount[VALID];
	}

	// do the pass
	void doPass();

	// choose a valid step randomly. before call it, you should confirm that there is at least one valid step
	const Coord& randomlyChooseNextStep() const;

	// randomly play with itself to the terminal, return the result
	Result randomlyPlay();
	Result randomlyPlay_DEBUG();

	// randomly play with itself to the terminal, return ( black piece count - white piece count)
	int randomlyPlayAndGetDetail();

	inline int getPositionValue(short x, short y) { return position_value[x][y]; }
	inline int getPositionValue(Coord coord) { return position_value[coord.first][coord.second]; }

	inline int getMobility() { return stateCount[VALID]; }

	inline int getPotential(bool side) { return sideBoundary[side].count(); }

	inline int getCornerCount(bool side) 
	{ 
		return (position[1][1].state == State(side)) +
			(position[1][8].state == State(side)) +
			(position[8][1].state == State(side)) +
			(position[8][8].state == State(side));
	}

	inline int getNearbyCount(bool side);

	int getEdgeCornerStable(bool side);

	inline std::vector<Coord> getValidPosition() const { return validPositionVector; }
};
