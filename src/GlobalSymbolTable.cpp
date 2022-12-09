#include <symbols.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>

symbolTableEntry g_symbolTable[512];
static int nextSymbol = 0;

void addSymbolIntoGlobalTable(Symbol* symbol, const char* identifier){
    g_symbolTable[nextSymbol].symbol = *symbol;
    g_symbolTable[nextSymbol].identifier  = identifier;

    nextSymbol++;
}

int checkIfSymbolDefined(const char* identifier){
    for(int i = 0; i < nextSymbol; i++){
        if(strlen(identifier) == strlen(g_symbolTable[i].identifier)){
            if(strcmp(identifier, g_symbolTable[i].identifier) == 0) {
                return 0;
            }
        }
    }

    return 1;
}


Symbol* translateSymbolValue(const char* type, const char* value){
    Symbol* returnVal = new Symbol;
    if (strcmp(type, "int") == 0){
        returnVal->value.integer = atoi(value);
        returnVal->type = TYPE_INTEGER; 
    } else if (strcmp(type, "char") == 0){
        returnVal->value.character = atoi(value);
        returnVal->type = TYPE_CHARACTER; 
    } else if (strcmp(type, "long") == 0){
        returnVal->value.character = atoi(value);
        returnVal->type = TYPE_LONG_INTEGER; 
    } else if (strcmp(type, "short") == 0){
        returnVal->value.character = atoi(value);
        returnVal->type = TYPE_SHORT_INTEGER; 
    } else {
        printf("Invalid variable type-> %s!\n", type);
        throw -1;
    }
    return returnVal;
}