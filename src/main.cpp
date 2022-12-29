#include <tahaScript>


namespace tahaScript{
    std::string         inputFileName;
    std::ifstream*      inputFile;
    FILE*               logFile;
    bool                logFileEnabled = false;
    bool                debugEnabled = false;
}

void debugPrintf(const char* fmt, ...){

    if(!tahaScript::debugEnabled){
        return;
    }

    va_list args;
    va_start(args, fmt);
    if(tahaScript::logFileEnabled == false){
        printf("Debug: ");
        vfprintf(stdout, fmt, args);
    } else {
        vfprintf(tahaScript::logFile, fmt, args);
    }
    va_end(args);
}

void throwError(const char* fmt, ...){
    va_list args;
    va_start(args, fmt);
    fprintf(stderr, "In file: %s, on line %d, Fatal Error: \t", tahaScript::inputFileName.c_str(), tahaScript::inputFileLine);
    vfprintf(stderr, fmt, args);
    fprintf(stderr, "\n");
    va_end(args);
    exit(EXIT_FAILURE);
}

int main(int _argc, char** _argv){

    //Scan for flags

    bool filenameFound = false;
    char* _filename = nullptr;

    for(int i = 1; i < _argc; i++){
        if(strcmp(_argv[i], "-d") == 0){

            tahaScript::debugEnabled = true;

        } else if (strcmp(_argv[i], "-f") == 0){

            _filename = _argv[i + 1];

        } else if (strcmp(_argv[i], "--logfile") == 0){
            
            tahaScript::logFileEnabled = true;
            char* logfileName = _argv[i + 1];
            tahaScript::logFile = fopen(logfileName, "w");

        }
    }

    if(_filename == nullptr){
        std::cerr << "No filename provided (use '-f' to specity a code file)";
        return -1;
    }


    tahaScript::inputFileName = std::string(_filename);

    debugPrintf("Trying to open file: '%s'\n", tahaScript::inputFileName.c_str());

    tahaScript::inputFile = new std::ifstream();
    tahaScript::inputFile->open(tahaScript::inputFileName);

    if(!tahaScript::inputFile->is_open()) {
        throwError("Fatal Error: Unable to open file: %s", tahaScript::inputFileName);
    }

    debugPrintf("Opened file: '%s'\n", tahaScript::inputFileName.c_str());

    processFile();

    return EXIT_SUCCESS;
}