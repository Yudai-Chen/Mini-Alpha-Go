#include "memory_pool.h"
#include "MCTS_node.h"
#include "omp.h"

Memory_pool::Memory_pool()
{
	pool = (char*)malloc(sizeof(MCTS_node) * pool_size);
	poolList[0] = pool;
	poolCount = 1;
}

void Memory_pool::expandMemoryPool()
{
	pool = (char*)malloc(sizeof(MCTS_node) * pool_size);
	poolList[poolCount++] = pool;
}

void Memory_pool::release()
{
	for (int i = 0; i < poolCount; i++)
	{
		free(poolList[i]);
	}
}