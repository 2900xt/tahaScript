#include <iostream>
#include "Preprocessor.h"

int main(int argc, char** argv)
{
    Preprocessor* p = new Preprocessor("in.tsc", "out.txt");
    p->processFile();

}
