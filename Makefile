CXX = g++
CFLAGS = -O2 -I lib -lstdc++fs --std=c++17

LD = g++
LDFLAGS = -lstdc++fs

OBJS = $(OBJDIR)/main.o $(OBJDIR)/defineSymbols.o $(OBJDIR)/processFile.o 
OBJDIR = ./bin

IDIR = ./lib
DEPS = $(IDIR)/tahaScript

SRCDIR = ./src
SRC = $(SRCDIR)/main.cpp $(SRCDIR)/preprocessor/processFile.cpp $(SRCDIR)/preprocessor/defineSymbols.cpp

TSC = ./tscc

objs: $(SRC) $(DEPS)
	$(CXX) -c -o $(OBJDIR)/main.o $(SRCDIR)/main.cpp $(CFLAGS)
	$(CXX) -c -o $(OBJDIR)/processFile.o $(SRCDIR)/preprocessor/processFile.cpp $(CFLAGS)
	$(CXX) -c -o $(OBJDIR)/defineSymbols.o $(SRCDIR)/preprocessor/defineSymbols.cpp $(CFLAGS)


$(TSC): objs
	$(LD) $(OBJS) -o $(TSC) $(LDFLAGS)


.PHONY: test

test: $(TSC)
	$(TSC) -f test/code.tsc -d --logfile log.txt
