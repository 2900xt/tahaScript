#pragma once
#include <stddef.h>

union symbolValue
{
	int integer;
	char character;
	long longInteger;
	short shortInteger;
	double floatingPoint;
	void* pointer;
};

enum symbolType
{
	TYPE_INTEGER 			= 0x1,
	TYPE_CHARACTER			= 0x2,
	TYPE_LONG_INTEGER 		= 0x3,
	TYPE_FLOATING_POINT 	= 0x4,
	TYPE_SHORT_INTEGER		= 0x5,
	TYPE_POINTER			= 0x6
};

class Symbol
{
public:
	symbolValue value;
	symbolType type;
};

class symbolTableEntry
{
public:
	const char* identifier;
	Symbol symbol;

};



Symbol* translateSymbolValue(const char* type, const char* value);
void addSymbolIntoGlobalTable(Symbol* value, const char* identifier);
int checkIfSymbolDefined(const char* identifier);