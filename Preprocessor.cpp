#include "Preprocessor.h"

#define MAX_LINE_SIZE 512


int strlen_ts(const char* src) {
	int length = 0;
	while (*src++) {
		length++;
		if (*src == '\n' || *src == ' ')
			return length;
	}
	return length;
}

int strcmp_ts(const char* src1, const char* src2) {
	if (strlen_ts(src1) > strlen_ts(src2)) {
		return -1;
	}

	while ((*src1 != '\0') && (*src1 != ' ') && (*src1 != '\n')) {
		if (*src1++ != *src2++) {
			return 1;
		}
	}

	return 0;
}

int strlen_nl(const char* src) {
	int length = 0;
	while (*src++ != '\n' && *src++ != '\0') {
		length++;
	}
	return length;
}

void getLine(FILE* file, char* buffer) {
	int bufferPtr = 0;
	char current = fgetc(file);
	while (current != '\n' && current != EOF) {
		buffer[bufferPtr++] = current;
		current = fgetc(file);
	}
}

void strcpy_ts(char* dest, const char* src) {
	for (int i = 0; i < strlen_ts(src); i++) {
		dest[i] = src[i];
	}
}

Preprocessor::Preprocessor(const char* inputFilePath, const char* outputFilePath) {
	if (inputFilePath == NULL || outputFilePath == NULL) {
		throw "Invalid file Path";
	}
	fopen_s(&inputFile, inputFilePath, "r");
	fopen_s(&outputFile, outputFilePath, "w");
}

preprocessorWord* Preprocessor::getFlag(char* buffer) {
	preprocessorWord* ret = new preprocessorWord;

	if (buffer == NULL) {
		return NULL;
	}
	
	if (buffer[0] != '#') {
		return NULL;
	}

	char* retptr = buffer;

	while (*++buffer != '\n');

	*buffer = '\0';

	ret->flag = retptr[1];
	ret->data = retptr + 3;

	return ret;
}



void Preprocessor::replaceWords(const char* phrase, const char* replacement) {
	int currentLine = 1;
	char* buffer = (char*)calloc(1, MAX_LINE_SIZE);

	fseek(this->inputFile, 0L, SEEK_SET);		//Seek to begining of file

	//Get lines one by one and check for phrase, if found replace. Do this untill reaching EOF
	while (feof(this->inputFile) == 0){
		getLine(this->inputFile, buffer);

		int length = strlen_nl(buffer);
		if (buffer[0] == '#') {
			goto end;
		}

		for (int i = 0; i < length; i++) {
			int cmp = strcmp_ts(phrase, buffer + i);
			if (cmp == 0) {
				strcpy_ts(buffer + i , replacement);
			}
		}

		end:

		fputs(buffer, this->outputFile);
		fputc('\n', this->outputFile);

		currentLine++;

		//Reset buffer
		for (int i = 0; i < MAX_LINE_SIZE; i++) {
			buffer[i] = 0;
		}
	}
}

void Preprocessor::includeFile(const char* file, long offset) {
	char* buffer = (char*)malloc(MAX_LINE_SIZE);
	FILE* includedFile;
	int err = fopen_s(&includedFile, file, "r");

	if (includedFile == NULL) {
		printf("Unable to open file: %s\nError code: %d\n", file, err);
		return;
	}

	fseek(this->outputFile, offset, SEEK_SET);

	char currentChar = fgetc(includedFile);

	while (currentChar != EOF) {
		currentChar = fgetc(includedFile);
		fputc(currentChar, this->outputFile);
	}

	free(buffer);
}

void Preprocessor::processFile(void) {
	char* buffer = (char*)calloc(1, MAX_LINE_SIZE);
	preprocessorWord* currentFlag = getFlag(buffer);
	int currentLine = 0;
	while (currentFlag != NULL) {
		int offset = strlen_ts(currentFlag->data) + 1;
		switch (currentFlag->flag) {
		case 'd':
			replaceWords(currentFlag->data, currentFlag->data + offset);
			break;
		case 'i':
			includeFile(currentFlag->data, 0);
			break;
		default:
			break;
		}
	}
}