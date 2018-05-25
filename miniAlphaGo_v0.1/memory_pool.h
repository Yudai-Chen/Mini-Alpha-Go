#pragma once

class Memory_pool
{
private:
	char * pool;
	char * poolList[400];
	int poolCount;
	
public:
	Memory_pool();
	char * getMemoryPool() { return pool; }
	void expandMemoryPool();
	void release();
};
