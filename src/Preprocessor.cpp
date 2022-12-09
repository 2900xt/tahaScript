#include <Preprocessor.h>
#include <symbols.h>
#define MAX_LINE_SIZE 512

Preprocessor::Preprocessor(const char* inputFilePath, const char* outputFilePath) {
	if (inputFilePath == NULL || outputFilePath == NULL) {
		std::cerr << "Invalid File Path!\n";
		throw -1;
	}

	inputFile =  fopen(inputFilePath, "r");
	outputFile = fopen(outputFilePath, "w");

	if(inputFile == NULL || outputFile == NULL){
		std::cerr << "Unable to open file!\n";
		throw -2;
	}

	inputFileOffset = 0;
	outputFileOffset = 0;
}

void Preprocessor::getLine(FILE* file, char* buffer) {
	int bufferPtr = 0;
	char current = fgetc(file);
	while (current != '\n' && current != EOF) {
		buffer[bufferPtr++] = current;
		current = fgetc(file);
		inputFileOffset++;
	}
}

preprocessorWord* Preprocessor::getFlag(char* buffer) {
	if (buffer == NULL) {
		printf("buffer is null!");
		throw -1;
	}

	getLine(this->inputFile, buffer);
	preprocessorWord* ret = new preprocessorWord;

	
	if (buffer[0] != '#') {
		printf("no flag found!\n%s", buffer);
		return NULL;
	}

	ret->flag = buffer[1];
	ret->data = buffer + 3;

	return ret;
}

void Preprocessor::includeFile(const char* file) {
	char* buffer = (char*)malloc(MAX_LINE_SIZE);
	FILE* includedFile = fopen(file, "r");

	if (includedFile == NULL) {
		printf("Unable to open file: %s\n", file);
		return;
	}

	fseek(this->outputFile, outputFileOffset, SEEK_SET);

	char currentChar = fgetc(includedFile);

	while (currentChar != EOF) {
		fputc(currentChar, this->outputFile);
		outputFileOffset++;
		currentChar = fgetc(includedFile);
	}
}

#define TYPE 		parameterOffsets[0]
#define VALUE 		parameterOffsets[1]
#define IDENTIFIER 	parameterOffsets[2]
#define MISC		parameterOffsets[3]

void Preprocessor::processFile(void) {
	char* buffer = (char*)calloc(1, MAX_LINE_SIZE);
	preprocessorWord* currentFlag = getFlag(buffer);
	char* parameterOffsets[4];
	
	switch (currentFlag->flag) {
		 case 'c': {
			TYPE = cutStringTillCharacter(currentFlag->data, ' ');
			VALUE = cutStringTillCharacter(currentFlag->data + strlenTillSpaces(currentFlag->data, 1), ' ');
			IDENTIFIER = cutStringTillCharacter(currentFlag->data + strlenTillSpaces(currentFlag->data, 2), ' ');
			Symbol* symbol = translateSymbolValue(TYPE, VALUE);

			printf("symbol \"%s\" has a value of: %d, and a type of: %d\n", IDENTIFIER, symbol->value.integer, symbol->type);
			addSymbolIntoGlobalTable(symbol, IDENTIFIER);
			printf("symbol is defined? %d\n", checkIfSymbolDefined(IDENTIFIER));

			free(currentFlag->data);
			free(currentFlag);

			break;
		} case 'i': {
			includeFile(currentFlag->data);
			break;
		} default: {
			break;
		}
	}
	
}