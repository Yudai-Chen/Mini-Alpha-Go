#include "library_solver.h"

void Library_tree_node::release()
{
	if (nextMoveCount != 0)
	{
		for (int i = 0; i < nextMoveCount; i++)
		{
			if (nextMove[i]->nextMoveCount != 0)
				nextMove[i]->release();
			delete nextMove[i];
		}
	}
}

void Library_solver::buildLibraryTree(const Library_reader_factory & lrf)
{
	int libraryCount = lrf.getLibraryCount();
	for (short i_lib = 0; i_lib < libraryCount; i_lib++)
	{
		int recordCount = lrf.getRecordCount(i_lib);
		for (int i_rec = 0; i_rec < recordCount; i_rec++)
		{
			int solvedBlackCount_rec = lrf.getSolvedBlackCount(i_lib, i_rec);
			Result winSide = DRAW;
			if (solvedBlackCount_rec > 32)
			{
				winSide = BLACK_WIN;
			}
			else if (solvedBlackCount_rec < 32)
			{
				winSide = WHITE_WIN;
			}
			Library_tree_node * curNode = root;
			for (short i_turn = 0; i_turn < 40; i_turn++)
			{
				Coord move = lrf.getMove(i_lib, i_rec, i_turn);
				if (curNode->nextMoveCount == 0)
				{
					curNode->nextMove[0] = new Library_tree_node(move);	//创建新节点
					curNode->nextMoveCount = 1;
					curNode = curNode->nextMove[0];					//tree walk	
				}
				else
				{
					for (short i_child = 0; i_child < curNode->nextMoveCount; i_child++)
					{
						if (curNode->nextMove[i_child]->move == move)			//this move has been tried
						{
							curNode = curNode->nextMove[i_child];
							goto update_node_info;
						}
					}
					curNode->nextMove[curNode->nextMoveCount++] = new Library_tree_node(move);	//创建新节点
					curNode = curNode->nextMove[curNode->nextMoveCount - 1];					//tree walk	
				}
				update_node_info:
				curNode->solvedBlackCount += solvedBlackCount_rec;							//update
				curNode->visitTime++;
				if (winSide != DRAW)
				{
					curNode->winTime[winSide]++;
				}
			}
		}
	}
}

Coord Library_solver::getBestMoveAndChangeState()
{
	short nextMoveIndex = -1;
	Coord res = Coord(-1, -1);
	if (mainState->move != Coord(-1, -1) && mainState->visitTime < 11)
		return res;

	double maxScore = 0;

	if (turnID % 2 == 1)
	{
		for (int i = 0; i < mainState->nextMoveCount; i++)
		{
			double visitRatio = (double)mainState->nextMove[i]->visitTime / (double)mainState->visitTime;
			//double averageBlackCount = (double)mainState->nextMove[i]->solvedBlackCount / (double)mainState->nextMove[i]->visitTime;
			double winRate = (double)mainState->nextMove[i]->winTime[1] / (double)mainState->nextMove[i]->visitTime;
			double score = visitRatio + winRate;
			if (score > maxScore)
			{
				maxScore = score;
				res = mainState->nextMove[i]->move;
				nextMoveIndex = i;
			}
		}
	}
	else
	{
		for (int i = 0; i < mainState->nextMoveCount; i++)
		{
			double visitRatio = (double)mainState->nextMove[i]->visitTime / (double)mainState->visitTime;
			//double averageBlackCount = (double)mainState->nextMove[i]->solvedBlackCount / (double)mainState->nextMove[i]->visitTime;
			double winRate = (double)mainState->nextMove[i]->winTime[0] / (double)mainState->nextMove[i]->visitTime;
			double score = visitRatio + winRate;
			if (score > maxScore)
			{
				maxScore = score;
				res = mainState->nextMove[i]->move;
				nextMoveIndex = i;
			}
		}
	}
	if (nextMoveIndex != -1)
	{
		changeState(res, nextMoveIndex);
		transformSimilar(res);
	}
	return res;
}

int Library_solver::changeState(Coord & coord, short nextMoveIndex)
{
	if (nextMoveIndex != -1)
	{
		mainState = mainState->nextMove[nextMoveIndex];
		turnID++;
		return 0;
	}
	if (mainState == root)
	{
		if (coord == Coord(6, 5))
			similar = 1;
		else if (coord == Coord(3, 4))
			similar = 2;
		else if (coord == Coord(4, 3))
			similar = 3;
	}
	transformSimilar(coord);
	for (int i = 0; i < mainState->nextMoveCount; i++)
	{
		if (mainState->nextMove[i]->move == coord)
		{
			mainState = mainState->nextMove[i];
			turnID++;
			return 0;
		}
	}
	return 1;			//failed to match
}

void Library_solver::transformSimilar(Coord & input)
{
	Coord temp;
	if (!similar)
		return;
	if (similar == 1)
	{
		temp.first = input.second;
		temp.second = input.first;
		input = temp;
		return;
	}
	if (similar == 2)
	{
		temp.first = 9 - input.second;
		temp.second = 9 - input.first;
		input = temp;
		return;
	}
	if (similar == 3)
	{
		temp.first = 9 - input.first;
		temp.second = 9 - input.second;
		input = temp;
		return;
	}
}


void Library_solver::releaseLibraryTree()
{
	root->release();
}