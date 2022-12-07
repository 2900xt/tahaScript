#pragma once
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

class preprocessorWord {
public:
	char flag;
	char* data;

	~preprocessorWord() {
		free(data);
	}
};

class Preprocessor
{
private:
	FILE* inputFile;
	FILE* outputFile;
	preprocessorWord* getFlag(char* buffer);
	void replaceWords(const char* phrase, const char* replacement);
	void includeFile(const char* file, long line);
public: 
	Preprocessor(const char* inputFilePath, const char* outputFilePath);
	void processFile(void);
};

