#pragma once
#include <stdio.h>

int strlenTillSpaces(const char* src, int spaceCount);
int strlenTillNewLine(const char* src);
int strcmpTillSpace(const char* src1, const char* src2);
int strlenTillNewLine(const char* src);
char* cutStringTillCharacter(const char* src, char character);
int strlenTillCharacter(const char* src, int count, char character);