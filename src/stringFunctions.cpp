#include <stringFunctions.h>
#include <stdlib.h>

int strlenTillSpaces(const char* src, int spaceCount) {
	int length = 0;
	int found = 0;
	while (found != spaceCount)  {
		if (*src == ' '){
			found++;
		} else if (*src == '\0' || *src == '\n'){
			return length;
		}
		src++;
		length++;
	}
	return length;
}

int strlenTillCharacter(const char* src, int count, char character){
	int length = 0;
	int found = 0;
	while (found != count)  {
		if (*src == character){
			found++;
		} else if (*src == '\0' || *src == '\n'){
			return length + 1;
		}
		src++;
		length++;
	}
	return length;
}

char* cutStringTillCharacter(const char* src, char character){
	int length = strlenTillCharacter(src, 1, character);
	char* newString = (char*)malloc(length + 1);
	int i;
	for(i = 0; i < length; i++){
		newString[i] = src[i];
	}

	newString[i - 1] = '\0';

	return newString;
}

int strcmpTillSpace(const char* src1, const char* src2) {
	if (strlenTillSpaces(src1, 1) > strlenTillSpaces(src2, 1)) {
		return -1;
	}

	while ((*src1 != '\0') && (*src1 != ' ') && (*src1 != '\n')) {
		if (*src1++ != *src2++) {
			return 1;
		}
	}

	return 0;
}

int strlenTillNewLine(const char* src) {
	int length = 0;
	while (*src != '\n' && *src++ != '\0') {
		length++;
	}
	return length;
}

