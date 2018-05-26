#include "library_reader.h"

void Library_reader::readHeader(const std::string & path)
{
	library.open(path, std::ios::binary);
	char temp[16];
	library.read(temp, 16);
	memcpy(header, temp, 16);

	recordNumber = 0;
	bool flag = false;
	for (int i = 7, j = 0; i > 3; i--)
	{
		if (flag == false && header[i] == 0)
		{			
			continue;
		}
		flag = true;
		recordNumber = recordNumber * std::pow(256, j) + header[i];
		j++;
	}
	flag = false;
	year = 0;
	for (int i = 11, j = 0; i > 9; i--)
	{
		if (flag == false && header[i] == 0)
		{
			continue;
		}
		flag = true;
		year = year * std::pow(256, j) + header[i];
		j++;
	}
	searchDepth = header[14];
}

void Library_reader::readSingleRecord(unsigned int index)
{
	char temp[68];
	library.read(temp, 68);
	unsigned char recordBuf[42];
	memcpy(recordBuf, temp + 6, 42);
	records[index].parseRecordBuf(recordBuf);
}

void Library_reader::readRecords()
{
	records = new Library_record[recordNumber];
	for (unsigned int i = 0; i < recordNumber; i++)
	{
		readSingleRecord(i);
	}
	library.close();
}

void Library_reader::initLibrary(const std::string & path)
{
	readHeader(path);
	readRecords();
}

void Library_reader::release()
{
	delete[] records;
}