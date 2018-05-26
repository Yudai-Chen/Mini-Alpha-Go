#pragma once

#include "position.h"

class Library_record
{
public:
	Coord moveList[40];		//记录前40个走法
	unsigned int realResultBlackCount;	//记录最终比赛实际黑棋数
	unsigned int solvedResultBlackCount;	//记录完美终局解算后的黑棋数，更有参考价值，准确地说，对于第60-i步的局面评估更有参考价值(i是解算深度)	
public:	
	Library_record() {}
	void parseRecordBuf(const unsigned char* recordBuf);
};