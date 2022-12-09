#include <iostream>
#include <symbols.h>
#include "Preprocessor.h"

int main(int argc, char** argv)
{
    const char* defaultFileName = "a.out";
    Preprocessor* p;

    try{
        for(int i = 0; i < argc; i++) {
            
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
