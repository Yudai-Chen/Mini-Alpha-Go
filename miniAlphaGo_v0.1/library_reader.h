#pragma once
#include <iostream>
#include <fstream> 

#include "library_record.h"

class Library_reader
{
private:
	std::ifstream library;
	unsigned char header[16];
	unsigned int recordNumber;
	unsigned int year;
	unsigned int searchDepth;
	Library_record * records;
private:
	void readSingleRecord(unsigned int index);
	void readHeader(const std::string & path);
	void readRecords();
public:
	Library_reader() { };
	void initLibrary(const std::string & path);
	inline int getRecordCount() const { return recordNumber; }
	int getSolvedBlackCount(int recordID) const { return records[recordID].solvedResultBlackCount; }
	Coord getMove(int recordID, short turnID) const { return records[recordID].moveList[turnID]; }
	void release();
};