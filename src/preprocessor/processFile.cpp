#include <tahaScript>
/*
TODO:
1. File including
2. Error handling
3. Optimization
*/

std::string outputBuffer{""};

namespace tahaScript
{
    int lineEnable = 2;
    int inputFileLine = 1;
}


size_t includeFile(std::string inputBuffer){

    std::vector<std::string> params = getParams(inputBuffer, 1, 3);

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

    outputBuffer.append(fileBuffer);

    return fileSize;
}

void evaluatePreprocessorWord(char** buffer, int* bufferPtr) {
    
    switch((*buffer)[*bufferPtr+1]){
    case 'i':
        includeFile(std::string((*buffer) + *bufferPtr));
        break;
    case 'd':
        defineNewSymbol(std::string((*buffer) + *bufferPtr));
        break;
    case 'f':
        if((*buffer)[*bufferPtr+2] == 'v'){
            ifDefinedToAValue(std::string(*(buffer) + *bufferPtr));
            break;
        }
        ifDefined(std::string(*(buffer) + *bufferPtr));
        break;
    case 'n':
        if((*buffer)[*bufferPtr+2] == 'v'){
            ifDefinedToAValue(std::string(*(buffer) + *bufferPtr));
            tahaScript::lineEnable = !(bool)tahaScript::lineEnable;
            break;
        }
        ifDefined(std::string(*(buffer) + *bufferPtr));
        tahaScript::lineEnable = !(bool)tahaScript::lineEnable;
        break;
    case 'e':
        tahaScript::lineEnable = 2;
        break;
    default:
        throwError("Invalid Preprocessor Directive: %c" , (*buffer)[*bufferPtr+1]);
    }

}

std::string processFile() {
    /*
    1. Get file size
    2. Load the file into a memory buffer
    */
    symbolTableInit();

    size_t inputFileLength;
    char* inputFileBuffer;

    std::experimental::filesystem::path filePath{tahaScript::inputFileName};
    inputFileLength = std::experimental::filesystem::file_size(filePath) + 1;

    debugPrintf("File size is %d bytes\n", inputFileLength);

    inputFileBuffer = (char*)calloc(1, inputFileLength);
    
    tahaScript::inputFile->getline(inputFileBuffer, inputFileLength, EOF);

    char currentCharacter = 0;
    char prevCharacter = 0;
    bool commentMode = false;

    for(int currentIndex = 0 ; currentIndex < inputFileLength + 1 ; currentIndex++){

        prevCharacter = currentCharacter;
        currentCharacter = inputFileBuffer[currentIndex];

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
            debugPrintf("'#' found on line: %d\n", tahaScript::inputFileLine);
            evaluatePreprocessorWord(&inputFileBuffer, &currentIndex);
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
    
    free(inputFileBuffer);
    return outputBuffer;

}