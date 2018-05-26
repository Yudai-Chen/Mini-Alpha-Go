#pragma once
#include "elements.h"
#include "library_reader.h"

class Library_reader_factory
{
private:
	Library_reader * libraryReaders;
	int libraryReaderCount;
public:
	Library_reader_factory() { libraryReaderCount = 0; libraryReaders = new Library_reader[max_library_number]; }
	void initLibrary(const std::string & path) { libraryReaders[libraryReaderCount++].initLibrary(path); }
	inline int getLibraryCount() const { return libraryReaderCount; }
	inline int getRecordCount(short libID) const { return libraryReaders[libID].getRecordCount(); }
	inline Coord getMove(short libID, int recordID, short turnID) const { return libraryReaders[libID].getMove(recordID, turnID); }
	inline int getSolvedBlackCount(short libID, int recordID) const { return libraryReaders[libID].getSolvedBlackCount(recordID); }
	void release();
};