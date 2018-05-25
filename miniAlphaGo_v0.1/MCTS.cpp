#include <time.h>
#include "omp.h"  
#include "MCTS.h"

#ifdef SEARCH_PARALLELLY
omp_lock_t search_lock;
#endif

MCTS::MCTS(const Board & board, bool _turn, double _C)
{
	root = new MCTS_node(board);

	root_turn = _turn;

	memoryPool = new Memory_pool();

	nodeCount = 0;

	C = _C;
}

MCTS_node* MCTS::select()
{
	MCTS_node * curNode = root;
	MCTS_node * nextNode = nullptr;
	while (1)
	{	
		if (curNode->isTerminal())
		{
			return curNode;
		}
#ifdef SEARCH_PARALLELLY
#pragma omp critical
		{
#endif
			if (curNode->hasUnexpandedChild())
			{
				return expand(curNode);
			}
#ifdef SEARCH_PARALLELLY
		}
#endif
		nextNode = curNode->getBestChild(C);
		if (nextNode == nullptr)
		{
			return curNode;
		}
		curNode = nextNode;		
	}
	return 0;
}

MCTS_node * MCTS::expand(MCTS_node * needExpand)
{
	Coord move = needExpand->popUnvisitied();
	MCTS_node * res;
	if(nodeCount >= pool_size)
	{
		memoryPool->expandMemoryPool();
		nodeCount = 0;
	}
	nodeCount++;

#ifdef DEBUG_PRINT_WHEN_NODE_100
	if (nodeCount % 100 == 0)
		std::cout << nodeCount << std::endl;
#endif
	res = new(memoryPool->getMemoryPool() + (nodeCount - 1) * sizeof(MCTS_node)) MCTS_node(needExpand, move);
	if (res->shouldPass())
	{
		if (nodeCount >= pool_size)
		{
			memoryPool->expandMemoryPool();
			nodeCount = 0;
		}
		nodeCount++;
		res = new(memoryPool->getMemoryPool() + (nodeCount - 1) * sizeof(MCTS_node)) MCTS_node(res, Coord(-1, -1));
	}
	return res;
}

double MCTS::simulate(MCTS_node* start)
{
	return start->rollout();
}

int MCTS::propagate(MCTS_node* start, double simulateResult)
{
	MCTS_node* curNode = start;
	while (curNode != nullptr)
	{
		curNode->changeVisitTimeAndWinTime(simulateResult);
		curNode = curNode->getParent();
	}
	return 0;
}

void MCTS::release()
{
	root->release();
	memoryPool->release();
}

Coord MCTS::search()
{
	time_t timeStart = time(NULL);
	time_t timeEnd;
	int searchTimeCount = 0;
#ifdef SOLID_THINK_TIME
	while (1)
	{
		searchTimeCount++;
		MCTS_node * currNode = select();
		double simulateRes = simulate(currNode);
		propagate(currNode, simulateRes);
		if (searchTimeCount % 2000 == 0)
		{
			timeEnd = time(NULL);
			if (timeEnd - timeStart >= think_time)
			{  
				break;
			}
		}
	}
#ifdef OUTPUT_SEARCH_TIME
	std::cout << "Search time for this move: " << searchTimeCount << std::endl;
#endif
#endif
#ifdef SOLID_SEARCH_TIME
#ifdef SEARCH_PARALLELLY
	//omp_init_lock(&search_lock);
#pragma omp parallel for num_threads(2)
#endif
	for (searchTimeCount = 0; searchTimeCount < search_time; searchTimeCount++)
	{
		MCTS_node * currNode = select();
		double simulateRes = simulate(currNode);
		propagate(currNode, simulateRes);
	}
#ifdef OUTPUT_THINK_TIME
	timeEnd = time(NULL);
	std::cout << "Think time for this move: " << timeEnd - timeStart << std::endl;
#endif
#endif
	return root->choiceAfterMCTS();
}