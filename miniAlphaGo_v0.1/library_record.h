#pragma once

#include "position.h"

class Library_record
{
public:
	Coord moveList[40];		//��¼ǰ40���߷�
	unsigned int realResultBlackCount;	//��¼���ձ���ʵ�ʺ�����
	unsigned int solvedResultBlackCount;	//��¼�����վֽ����ĺ����������вο���ֵ��׼ȷ��˵�����ڵ�60-i���ľ����������вο���ֵ(i�ǽ������)	
public:	
	Library_record() {}
	void parseRecordBuf(const unsigned char* recordBuf);
};