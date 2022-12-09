#pragma once
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stringFunctions.h>
#include <iostream>



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
	long inputFileOffset;
	long outputFileOffset;
	preprocessorWord* getFlag(char* buffer);
	void includeFile(void);
	void getLine(char* buffer);
public: 
	Preprocessor(const char* inputFilePath, const char* outputFilePath);
	void processFile(void);
	
	~Preprocessor(void){
		fclose(inputFile);
		fclose(outputFile);
	}
};

