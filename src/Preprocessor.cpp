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
	getLine(this->inputFile, buffer);
	preprocessorWord* ret = new preprocessorWord;

	if (buffer == NULL) {
		printf("buffer is null!");
		throw -1;
	}
	
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

void Preprocessor::processFile(void) {
	char* buffer = (char*)calloc(1, MAX_LINE_SIZE);
	preprocessorWord* currentFlag = getFlag(buffer);
	int parameterOffsets[4];
	
	switch (currentFlag->flag) {
		 case 'c': {
			parameterOffsets[0] = 0;																//Type
			parameterOffsets[1] = strlenTillSpaces(currentFlag->data, 1);							//Value
			parameterOffsets[2] = strlenTillSpaces(currentFlag->data, 2);							//Identifier
			Symbol* symbol = translateSymbolValue(cutStringTillCharacter(currentFlag->data + parameterOffsets[0], ' '), cutStringTillCharacter(currentFlag->data + parameterOffsets[1], ' '));
			printf("symbol \"%s\" has a value of: %d, and a type of: %d\n",currentFlag->data + parameterOffsets[2] ,symbol->value.integer, symbol->type);
			addSymbolIntoGlobalTable(symbol, currentFlag->data + parameterOffsets[2]);
			printf("symbol is defined? %d\n", checkIfSymbolDefined(currentFlag->data + parameterOffsets[2]));
			break;
		} case 'i': {
			includeFile(currentFlag->data);
			break;
		} default: {
			break;
		}
	}
	
}