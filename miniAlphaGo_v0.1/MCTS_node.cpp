#include <algorithm>

#include "MCTS_node.h"

bool positionValueDescendingSort(Coord coord1, Coord coord2) {
	return position_value[coord1.first][coord1.second] < position_value[coord2.first][coord2.second];
}

MCTS_node::MCTS_node(const Board & board)
{
	this->board = board;

	parent = nullptr;
	child[0] = nullptr;
	childCount = 0;

	visitTime = 0;
	winTime[0] = 0;
	winTime[1] = 0;
	quality = 0;

	move = Coord(0, 0);

	unvisited = board.getValidPosition();
	std::sort(unvisited.begin(), unvisited.end(), positionValueDescendingSort);
}

MCTS_node::MCTS_node(MCTS_node* parent, const Coord & move)
{
	board = parent->board;

	this->parent = parent;
	child[0] = nullptr;
	childCount = 0;

	visitTime = 0;
	winTime[0] = 0;
	winTime[1] = 0;
	quality = 0;

	if (move != Coord(-1, -1))
		board.putPiece(move);
	else
		board.doPass();

#ifdef DEBUG_MODE
	board.printBoard();
#endif

	this->move = move;

	unvisited = board.getValidPosition();
	parent->child[parent->childCount++] = this;
	std::sort(unvisited.begin(), unvisited.end(), positionValueDescendingSort);
}

MCTS_node* MCTS_node::getBestChild(double C)
{	
	MCTS_node* res = child[0];
	if (res == nullptr)
	{
#ifdef DEBUG_MODE_NULLCHILID
		parent->board.printBoard();
		parent->board.printValid();
		std::cout << "my choice: " << move.first << ", " << move.second << std::endl;
		board.printBoard();
		board.printValid();
#endif
		return res;
	}
	if (childCount == 1)
	{
		return res;
	}
	double score;
	double maxScore = -300;
	for (int i = 1; i < childCount; i++)
	{
		score = getChildScore(child[i], C);
		if (score > maxScore)
		{
			res = child[i];
			maxScore = score;
		}
	}
#ifdef DEBUG_MODE_BESTCHILD
	res->board.printBoard();
#endif // DEBUG_MODE_BESTCHILD

	return res;
}

double MCTS_node::getChildScore(MCTS_node * child, double C)
{
	double res;

	res = child->winTime[board.whosTurn()] / child->visitTime + C * sqrt(2 * log(visitTime) / child->visitTime);

	return res;
}

Coord MCTS_node::popUnvisitied()
{
	Coord res = unvisited.back();
	unvisited.pop_back();
	return res;
}

double MCTS_node::rollout() const
{
	Board temp = board;
	int res = temp.randomlyPlayAndGetDetail();
	if (res > 10)
		return win_above_10;
	if (res > 5)
		return win_above_5;
	if (res > 0)
		return win_below_5;
	if (res == 0)
		return draw;
	if (res > -6)
		return -win_below_5;
	if (res > -11)
		return -win_above_5;
	return -win_above_10;
}

void MCTS_node::changeVisitTimeAndWinTime(double score)
{
	visitTime++;
	if (score > 0)
	{
		winTime[1] += score;
		return;
	}
	if (score < 0)
	{
		winTime[0] += score;
		return;
	}
	winTime[0] += score;
	winTime[1] += score;
	return;
}

Coord MCTS_node::choiceAfterMCTS() const
{
	Coord res;
	double score;
	double maxScore = -300;
	for (int i = 0; i < childCount; i++)
	{
		score = child[i]->getScoreAfterMCTS();
		if (score > maxScore)
		{
			res = child[i]->getMove();
			maxScore = score;
		}
	}
	return res;
}


double MCTS_node::getScoreAfterMCTS()
{
	double res;

	res = quality + winTime[!board.whosTurn()] / visitTime;

	return res;
}

void MCTS_node::release()
{
	if (childCount != 0)
	{
		for (int i = 0; i < childCount; i++)
		{
			if(child[i]->childCount != 0)
				child[i]->release();
			child[i]->~MCTS_node();
		}
	}
}