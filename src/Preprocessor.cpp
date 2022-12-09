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

void Preprocessor::getLine(char* buffer) {
	int bufferPtr = 0;
	char current = fgetc(inputFile);
	while (current != '\n' && current != EOF) {
		buffer[bufferPtr++] = current;
		current = fgetc(inputFile);
	}
}

preprocessorWord* Preprocessor::getFlag(char* buffer) {
	if (buffer == NULL) {
		printf("buffer is null!");
		throw -3;
	}

	getLine(buffer);
	preprocessorWord* ret = (preprocessorWord*)malloc(sizeof(preprocessorWord));

	
	if (buffer[0] != '#') {
		printf("no flag found!\n%s", buffer);
		return NULL;
	}

	ret->flag = buffer[1];
	ret->data = buffer + 3;

	return ret;
}


static char* parameterOffsets[4];

#define TYPE 		parameterOffsets[0]
#define VALUE 		parameterOffsets[1]
#define IDENTIFIER 	parameterOffsets[2]
#define MISC		parameterOffsets[3]
#define FILENAME    parameterOffsets[0]

#define ARG0 		cutStringTillCharacter(currentFlag->data, ' ')
#define ARG1		cutStringTillCharacter(currentFlag->data + strlenTillSpaces(currentFlag->data, 1), ' ')
#define ARG2		cutStringTillCharacter(currentFlag->data + strlenTillSpaces(currentFlag->data, 2), ' ')

static void freeArgs(int count){
	for(int i = 0 ; i < count; i++){
		free(parameterOffsets[i]);
	}
}

static void defineSymbol(void){
	Symbol* symbol = translateSymbolValue(TYPE, VALUE);
	addSymbolIntoGlobalTable(symbol, IDENTIFIER);

	if(checkIfSymbolDefined(IDENTIFIER) != 0){
		std::cerr << "Error defining symbol:" << IDENTIFIER << std::endl;
		throw -4;
	}
}

void Preprocessor::includeFile(void) {
	char* buffer = (char*)malloc(MAX_LINE_SIZE);
	FILE* includedFile = fopen(IDENTIFIER, "r");

	if (includedFile == NULL) {
		std::cerr << "Unable to open file: " << IDENTIFIER << std::endl;
		throw -5;
	}

	char currentChar = fgetc(includedFile);

	while (currentChar != EOF) {
		fputc(currentChar, this->outputFile);
		currentChar = fgetc(includedFile);
	}

	fclose(includedFile);
	freeArgs(1);
}

void Preprocessor::processFile(void) {
	char* buffer = (char*)calloc(1, MAX_LINE_SIZE);

	preprocessorWord* currentFlag = getFlag(buffer);

	switch (currentFlag->flag) {
		 case 'c': {
			TYPE = ARG0;
			VALUE = ARG1;
			IDENTIFIER = ARG2;
			defineSymbol();
			break;
		} case 'n':{
			if(checkIfSymbolDefined(IDENTIFIER) == 0){
				printf("Symbol %s is defined!\n", IDENTIFIER);
				break;
			}

			printf("Symbol %s is not defined!\n", IDENTIFIER);

			//Get characters untill '#e'
			char* characterBuffer = (char*)malloc(MAX_LINE_SIZE);
			
			while(true)
			{
				getLine(characterBuffer);

				if(characterBuffer[0] == '#' && characterBuffer[1] == 'e'){
					break;
				}
				
				if (characterBuffer[0] == '#') {
					continue;
				}

				fputs(characterBuffer, this->outputFile);

			}
			
			free(characterBuffer);
			break;

		}  case 'i': {
			FILENAME = ARG0;
			includeFile();
			break;
		} default: {
			break;
		}

	}

	free(buffer);

}