#include "library_record.h"

void Library_record::parseRecordBuf(const unsigned char* recordBuf)
{
	realResultBlackCount = recordBuf[0];
	solvedResultBlackCount = recordBuf[1];
	for (int i = 2; i < 42; i++)
	{
		unsigned int temp = recordBuf[i];
		moveList[i - 2] = Coord(temp / 10, temp % 10);
	}
}