#include <algorithm>

#include "MCTS_node.h"

#ifdef DEBUG_MODE_POSITION_VALUE_EVALUATE
double maxQuality = 0, minQuality = 0;
#endif // DEBUG_MODE_POSITION_VALUE_EVALUATE


//position evaluate
double W1 = 0.13;
//stable pieces count evaluate
double W2 = 0.25;

double W3 = 0.2;

double W4 = 1;

double W5 = 0.1;

double W6 = 0;

MCTS_node::MCTS_node(const Board & board)
{
	this->board = board;

	parent = nullptr;
	child[0] = nullptr;
	childCount = 0;

	visitTime = 0;
	winTime[0] = 0;
	winTime[1] = 0;
#ifdef PRINT_DETAIL_SCORES
	positionValueScore = 0;
	cornerCountScore = 0;
	mobilityScore = 0;
	potentialScore = 0;
	nearbyCountScore = 0;
	edgeCornerStableScore = 0;
#endif
	quality = 0;

	move = Coord(0, 0);

	unvisited = board.getValidPosition();
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

	if (move != Coord(-1, -1))
		board.putPiece(move);
	else
		board.doPass();

	this->move = move;
#ifdef PRINT_DETAIL_SCORES
	positionValueScore = 0;
	cornerCountScore = 0;
	mobilityScore = 0;
	potentialScore = 0;
	nearbyCountScore = 0;
	edgeCornerStableScore = 0;
#endif
	quality = calculateNodeQuality();

	unvisited = board.getValidPosition();
	parent->child[parent->childCount++] = this;
}

double MCTS_node::calculateNodeQuality()
{
	if (board.getTurnId() > 28)
	{

	}
	else if (board.getTurnId() > 20)
	{
		W1 = 0;	//²»¿¼ÂÇÎ»ÖÃ¹ÀÖµ
	}

	double res = 0;

#ifdef PRINT_DETAIL_SCORES
	if (W1)
	{
		int temp = board.getPositionValueEvaluate(!board.whosTurn());
		positionValueScore = W1 * temp;
		res += positionValueScore;
	}
	if (W2)
	{
		int temp = (board.getCornerCount(!board.whosTurn()) - board.getCornerCount(board.whosTurn()));
		cornerCountScore = W2 * temp;
		res += cornerCountScore;
	}
	if (W3)
	{
		int temp = board.getMobility();
		mobilityScore = -W3 * temp;
		if (temp > 0)
			res += mobilityScore;
		else
			res += 5;
	}
	if (W4)
	{
		double fathersPotential = board.getPotential(!board.whosTurn());
		double fathersEnemyPotential = board.getPotential(board.whosTurn());
		double ratio;
		if (fathersPotential > fathersEnemyPotential)
			ratio = fathersPotential / (fathersPotential + fathersEnemyPotential);
		else if (fathersPotential < fathersEnemyPotential)
			ratio = -fathersEnemyPotential / (fathersPotential + fathersEnemyPotential);
		else
			ratio = 0;
		potentialScore = W4 * ratio;
		res += potentialScore;
	}
	if (W5)
	{
		int temp = board.getNearbyCount(!board.whosTurn()) - board.getNearbyCount(board.whosTurn());
		nearbyCountScore = -W5 * temp;
		res += nearbyCountScore;
	}
	if (W6)
	{
		int temp = board.getEdgeCornerStable(!board.whosTurn()) - board.getEdgeCornerStable(board.whosTurn());
		edgeCornerStableScore = W6 * temp;
		res += edgeCornerStableScore;
	}
#else
	if (W1)
	{
		int temp = board.getPositionValueEvaluate(!board.whosTurn());
		double positionValueScore = W1 * temp;
		res += positionValueScore;
	}
	if (W2)
	{
		int temp = (board.getCornerCount(!board.whosTurn()) - board.getCornerCount(board.whosTurn()));
		double cornerCountScore = W2 * temp;
		res += cornerCountScore;
	}
	if (W3)
	{
		int temp = board.getMobility();
		double mobilityScore = -W3 * temp;
		if (temp > 0)
			res += mobilityScore;
		else
			res += 5;
	}
	if (W4)
	{
		double fathersPotential = board.getPotential(!board.whosTurn());
		double fathersEnemyPotential = board.getPotential(board.whosTurn());
		double ratio;
		if (fathersPotential > fathersEnemyPotential)
			ratio = fathersPotential / (fathersPotential + fathersEnemyPotential);
		else if (fathersPotential < fathersEnemyPotential)
			ratio = -fathersEnemyPotential / (fathersPotential + fathersEnemyPotential);
		else
			ratio = 0;
		double potentialScore = W4 * ratio;
		res += potentialScore;
	}
	if (W5)
	{
		int temp = board.getNearbyCount(!board.whosTurn()) - board.getNearbyCount(board.whosTurn());
		double nearbyCountScore = -W5 * temp;
		res += nearbyCountScore;
	}
	if (W6)
	{
		int temp = board.getEdgeCornerStable(!board.whosTurn()) - board.getEdgeCornerStable(board.whosTurn());
		double edgeCornerStableScore = W6 * temp;
		res += edgeCornerStableScore;
}
#endif
	return res;
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
	for (int i = 0; i < childCount; i++)
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

	res = child->quality + 4 * child->winTime[board.whosTurn()] / child->visitTime + 4 * C * sqrt(2 * log(visitTime) / child->visitTime);

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
		winTime[0] -= score;
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
#ifdef PRINT_DETAIL_SCORES
	std::cout << "My choice is: (" << res.first << ", " << res.second << ")" << std::endl;
#endif
	return res;
}


double MCTS_node::getScoreAfterMCTS()
{
	double res;

	res = quality + 4 * winTime[!board.whosTurn()] / visitTime;

#ifdef PRINT_DETAIL_SCORES
	printChildDetailScores();
#endif

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

#ifdef PRINT_DETAIL_SCORES
void MCTS_node::printChildDetailScores()
{
	std::cout << "For child move: (" << move.first << ", " << move.second << ")" << std::endl;
	std::cout << "positionValueScore: " << positionValueScore << std::endl;
	std::cout << "cornerCountScore: " << cornerCountScore << std::endl;
	std::cout << "mobilityScore: " << mobilityScore << std::endl;
	std::cout << "potentialScore£º " << potentialScore << std::endl;
	std::cout << "nearbyCountScore£º " << nearbyCountScore << std::endl;
	std::cout << "edgeCornerStableScore£º " << edgeCornerStableScore << std::endl;
	std::cout << "visit time£º " << visitTime << std::endl;
	std::cout << "win rate£º " << winTime[!board.whosTurn()] / visitTime << std::endl;
	std::cout << "quality score£º " << quality << std::endl;
	std::cout << "win time score£º " << 4 * winTime[!board.whosTurn()] / visitTime << std::endl << std::endl;
}
#endif