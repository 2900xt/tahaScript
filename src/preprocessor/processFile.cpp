#include <tahaScript>

namespace tahaScript
{
    int lineEnable = 2;
    int inputFileLine = 1;
    int currentPos= 0;
}

size_t includeFile(std::string& inputBuffer, int pos){

    std::string paramsSRC = std::string(inputBuffer.c_str() + pos);

    std::vector<std::string> params = getParams(paramsSRC, 1, 3);

    std::experimental::filesystem::path path{params[0]};

    std::ifstream file{path.c_str()};
    size_t fileSize = std::experimental::filesystem::file_size(path) + 1;

    if(fileSize < 1){
        throwError("Invalid file path or empty file: %s", path.c_str());
    }

    if(!file.is_open()){
        throwError("Unable to open file: %s", path.c_str());
    }

    char* fileBuffer = (char*)calloc(1, fileSize);

    file.getline(fileBuffer, fileSize, EOF);

    inputBuffer.insert(pos + strlenTillCharacter(paramsSRC.c_str(), '\n') + 1, fileBuffer);

    return fileSize;
}

void evaluatePreprocessorWord(std::string& inputbuffer) {
    
    const char* ptr = inputbuffer.c_str() + tahaScript::currentPos;

    switch(ptr[1]){
    case 'i':
        includeFile(inputbuffer, tahaScript::currentPos);
        break;
    case 'd':
        defineNewSymbol(std::string(ptr));
        break;
    case 'f':
        if(ptr[2] == 'v') {
            ifDefinedToAValue(std::string(ptr));
            break;
        }
        ifDefined(std::string(ptr));
        break;
    case 'n':
        if(ptr[2] == 'v'){
            ifDefinedToAValue(std::string(ptr));
            tahaScript::lineEnable = !(bool)tahaScript::lineEnable;
            break;
        }
        ifDefined(std::string(ptr));
        tahaScript::lineEnable = !(bool)tahaScript::lineEnable;
        break;
    case 'e':
        tahaScript::lineEnable = 2;
        break;
    default:
        throwError("Invalid Preprocessor Directive: %c" , ptr[1]);
    }

}

std::string getLine(const std::string& src, int offset){
    int _offset = offset;
    while(src[offset++] != '\n');
    std::string out = src.substr(_offset, offset - _offset);
    return out;
}

std::vector<std::string>* processFile() {
    /*
    1. Get file size
    2. Load the file into a memory buffer
    */
    symbolTableInit();

    size_t inputFileLength;
    std::string inputFileBuffer(500, ' ');
    std::string outputBuffer;

    std::experimental::filesystem::path filePath{tahaScript::inputFileName};
    inputFileLength = std::experimental::filesystem::file_size(filePath) + 1;

    debugPrintf("File size is %d bytes\n", inputFileLength);

    std::getline(*tahaScript::inputFile, inputFileBuffer, (char)EOF);

    char currentCharacter = 0;
    char prevCharacter = 0;
    bool commentMode = false;

    for(; tahaScript::currentPos < inputFileBuffer.length() + 1 ; tahaScript::currentPos++){

        prevCharacter = currentCharacter;
        currentCharacter = inputFileBuffer[tahaScript::currentPos];

        if(currentCharacter == '\n'){
            commentMode = false;
            tahaScript::inputFileLine++;
        }

        else if(commentMode)
            continue;
        
        else if(currentCharacter == '<'){
            commentMode = true;
            debugPrintf("Comment on line: %d\n", tahaScript::inputFileLine);
            continue;
        }

        else if(currentCharacter == '#' && (prevCharacter == '\n' || prevCharacter == 0)){
            debugPrintf("'#%c' found on line: %d\n", inputFileBuffer[tahaScript::currentPos+1], tahaScript::inputFileLine);
            evaluatePreprocessorWord(inputFileBuffer);
            commentMode = true;
            continue;
        } 
        
        else if (currentCharacter == '#'){
            throwError("'#' must be on a new line!");
        }

        if(tahaScript::lineEnable)
            outputBuffer.push_back(currentCharacter);

    }

    debugPrintf(outputBuffer.c_str());

    if(tahaScript::lineEnable < 2){
        throwError("Missing an \"#e\" statement to end the if block!");
    }

    //store each line as a vector of strings

    std::vector<std::string>* output = new std::vector<std::string>{1};

    int currentIndex = 0;
    for(int i = 0; i < outputBuffer.length(); i++){
        if(outputBuffer[i] == '\n'){
            currentIndex++;
            output->push_back("");
            continue;
        }

        output->at(currentIndex).push_back(outputBuffer[i]);
    }

    return output;

}