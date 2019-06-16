#pragma once
#include <cstdio>

class CLogReader
{
public:
	CLogReader();
	~CLogReader();


	bool Open(const char* path);

	void Close(void);


	bool SetFilter(const char* filter);


	bool GetNextLine(char* buf, const int bufsize);
private:
	FILE* file_;
	char* filterStr;
	long posInFile;
	bool lastLine;
	int compareStrings(const char* wild, const char* string);
};