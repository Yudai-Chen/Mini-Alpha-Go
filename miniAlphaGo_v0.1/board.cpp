#include<random>
#include<ctime>

#include "board.h"

Board::Board()
{
	turn = true;
	hasPast[0] = 0;
	hasPast[1] = 0;
	position[4][4].state = WHITE;
	position[4][5].state = BLACK;
	position[5][4].state = BLACK;
	position[5][5].state = WHITE;
	stateCount[turn] = 2;
	stateCount[!turn] = 2;
	setBoundary();
	setValidPosition();
}

void Board::setSideBoundary(bool side)
{
	sideBoundary[side].reset();

	for (short i = 1; i < 9; i++)
		for (short j = 1; j < 9; j++)
			if (position[i][j].state == side)
				for (short d = 0; d < 8; d++)
				{
					short x = i + delta[d][0];
					short y = j + delta[d][1];
					if (onAbstractBoard(x, y) && (position[x][y].state == INVALID || position[x][y].state == VALID))
					{
						sideBoundary[side].set(bitoffset(x, y));
					}
				}
}

void Board::setBoundary()
{
	setSideBoundary(0);
	setSideBoundary(1);
	boundary = sideBoundary[0] | sideBoundary[1];
}

void Board::printBoundary() const
{
	for (short s = 0; s < 2; s++)
		for (short i = 0; i < 100; i++)
			if (sideBoundary[s][i] == 1)
			{
				s ? std::cout << "Black Boundary: " : std::cout << "White Boundary: ";
				std::cout << bitdeoffset(i).first << "," << bitdeoffset(i).second << std::endl;
			}
}

bool Board::isValid(short x, short y, bool side)
{
	bool res = false;
	assert(onSolidBoard(x, y));
	if (position[x][y].state < VALID)
	{
		std::cerr << "Try to put a piece on another piece" << std::endl;
		return false;
	}
	for (short d = 0; d < 8; d++)
	{
		position[x][y].valid_dir[d] = false;
		short x_next = x + delta[d][0];
		short y_next = y + delta[d][1];
		if (onSolidBoard(x_next, y_next) && position[x_next][y_next].state == !side)
		{
			for(x_next = x_next + delta[d][0], y_next = y_next + delta[d][1];onSolidBoard(x_next, y_next); x_next = x_next + delta[d][0], y_next = y_next + delta[d][1])
			{
				if (position[x_next][y_next].state == !side)
					continue;
				if (position[x_next][y_next].state == side)
				{
					position[x][y].valid_dir[d] = true;
					res = true;
					break;
				}
				break;
			}
		}
	}
	return res;
}

void Board::putPiece(const Coord &coord)
{
	short x = coord.first, y = coord.second;
	short flipCount = 0;	//number of the pieces be flipped when the piece is put
	position[x][y].state = State(turn);
	for (short d = 0; d < 8; d++)
	{
		if (position[x][y].valid_dir[d] == true)
		{
			short x_next = x + delta[d][0];
			short y_next = y + delta[d][1];
			while (position[x_next][y_next].state == !turn)
			{
				position[x_next][y_next].state = State(turn);
				x_next = x_next + delta[d][0];
				y_next = y_next + delta[d][1];
				flipCount++;
			}
		}
	}
	stateCount[turn] += flipCount + 1;
	stateCount[!turn] -= flipCount;
	hasPast[turn] = false;
	turn = !turn;
	setBoundary();
	setValidPosition();
}

void Board::setValidPosition()
{
	validPositionVector.clear();
	stateCount[VALID] = 0;
	for (unsigned short i = 0; i < 100; i++)
	{
		Coord coord = bitdeoffset(i);
		if (boundary[i] && onSolidBoard(coord))
		{
			position[coord.first][coord.second].state = INVALID;
			if(sideBoundary[!turn][i] && isValid(coord.first, coord.second, turn))
			{
				validPositionVector.push_back(coord);
				stateCount[VALID] ++;
				position[coord.first][coord.second].state = VALID;
			}
		}
	}
}

void Board::doPass()
{
	hasPast[turn] = true;
	turn = !turn;
	setValidPosition();
}

void Board::printValid() const
{
	for (auto valid : validPositionVector)
	{
		std::cout << "(" << valid.first << ", " << valid.second  << ")" << std::endl;
	}
}

void Board::printBoard() const
{
	for (int i = 0; i < 10; i++)
	{
		for (int j = 0; j < 10; j++)
		{
			if (position[i][j].state == BLACK)
				std::cout << "b ";
			else if (position[i][j].state == WHITE)
				std::cout << "w ";
			else if (position[i][j].state == VALID)
				std::cout << "v ";
			else if (position[i][j].state == INVALID)
				std::cout << "i ";
		}
		std::cout << std::endl;
	}
	std::cout << "==================================================" << std::endl;
}

const Coord& Board::randomlyChooseNextStep() const
{
	std::srand((unsigned)time(NULL));
	int choice = int(std::rand() % stateCount[VALID]);
	return validPositionVector.at(choice);
}
