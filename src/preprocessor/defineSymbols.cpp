#include <tahaScript>

int strlenTillCharacter(const char* src, char c = '\0'){
    int length = 0;
    while(*src++ != c){
        length++;
    }
    return length;
}

class Symbol{
protected:
    std::string value;
    std::string identifier;
public:
    Symbol(){
        this->value = "";
        this->identifier = "";
    }

    void setValue(const std::string src){
        this->value = src;
    }

    void setIdentifier(const std::string src){
        this->identifier = src;
    }

    std::string& getValue(){
        return this->value;
    }

    std::string& getIdentifier(){
        return this->identifier;
    }

};

struct SymbolTableEntry{
    Symbol data;
    SymbolTableEntry* next = nullptr;
};

SymbolTableEntry* nextFreeEntry;
SymbolTableEntry* firstSymbolEntry;

void symbolTableInit(){
    firstSymbolEntry = (SymbolTableEntry*)calloc(1, sizeof(SymbolTableEntry));
    nextFreeEntry = firstSymbolEntry;
}

void defineNewSymbol(std::string data){
    std::string identifier, value;
    int identifierOffset = 3;
    int identifierSize = strlenTillCharacter(data.c_str() + identifierOffset, ' ');
    int dataOffset = identifierOffset + identifierSize + 1;
    int dataSize = strlenTillCharacter(data.c_str() + dataOffset, '\n') - 1;


    identifier = data.substr(identifierOffset, identifierSize);
    value = data.substr(dataOffset, dataSize);

    nextFreeEntry->data.setValue(value);
    nextFreeEntry->data.setIdentifier(identifier);

    
    debugPrintf("DEF:\t Identifier: %s\tValue: %s\n", nextFreeEntry->data.getIdentifier().c_str(), nextFreeEntry->data.getValue().c_str());
    
    nextFreeEntry->next = (SymbolTableEntry*)calloc(1, sizeof(SymbolTableEntry));
    nextFreeEntry = nextFreeEntry->next;
}

SymbolTableEntry* getSymbol(std::string& identifier){
    SymbolTableEntry* currentSymbol = firstSymbolEntry;
    while(currentSymbol != nullptr){
        if(currentSymbol->data.getIdentifier().compare(identifier) == 0){
            return currentSymbol;
        }
        currentSymbol = currentSymbol->next;
    }

    return nullptr;
}

std::vector<std::string>  getParams(std::string buffer, int num, int firstParamOffset){
    std::vector<std::string> parameters;
    int currentOffset {firstParamOffset - 1};
    int currentSize = 0;

    //Get offsets and sizes, then get the parameter

    for(int i = 0; i < num; i++){
        currentOffset += currentSize + 1;
        currentSize = strlenTillCharacter(buffer.c_str() + currentOffset, (i == num - 1 ? '\n' : ' ')) - (i == num - 1);
        parameters.push_back(buffer.substr(currentOffset, currentSize));
        debugPrintf("Param #%d: %s\n", i, parameters[i].c_str());
    }

    return parameters;

}

void ifDefined(std::string buffer){
    std::string identifier = (getParams(buffer, 1, 3))[0];
    if(getSymbol(identifier)){
        tahaScript::lineEnable = 1;
    } else {
        tahaScript::lineEnable = 0;
    }
}


void ifDefinedToAValue(std::string buffer){
    std::vector<std::string> params = getParams(buffer, 2, 4);
    SymbolTableEntry* symbol = getSymbol(params[0]);
    if(symbol == nullptr){
        tahaScript::lineEnable = 0;
        return;
    }

    if(symbol->data.getValue().compare(params[1]) == 0){
        tahaScript::lineEnable = 1;
    } else {
        tahaScript::lineEnable = 0;
    }

}