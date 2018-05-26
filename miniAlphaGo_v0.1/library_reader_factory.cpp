#include "library_reader_factory.h"

void Library_reader_factory::release()
{
	for (int i = 0; i < libraryReaderCount; i++)
		libraryReaders[i].release();
	delete[] libraryReaders;
}