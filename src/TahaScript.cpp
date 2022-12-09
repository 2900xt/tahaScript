#include <iostream>
#include <symbols.h>
#include "Preprocessor.h"

int main(int argc, char** argv)
{
    const char* defaultFileName = "a.out";
    Preprocessor* p;

    try{
        if(argc == 2){
            p = new Preprocessor(argv[1], defaultFileName);
        } else {
            p = new Preprocessor(argv[1], argv[2]);
        }
    } catch (...) {
        printf("Usage: <inputFile> <outputFile>\n");
        return -1;
    }
    
    try{
        p->processFile();
    } catch(int err) {
        printf("Fatal Error: %d\n", err);
    }
    catch (const char* error) {
        printf("%s\n");
        return -1; 
    } catch (...){
        printf("Unknown Error Occurred!\n");
    }
}
