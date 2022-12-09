SRCDIR = ./src
LIBDIR = ./lib
OBJDIR = ./bin

CFLAGS = -I $(LIBDIR)
CC = g++ $(CFLAGS)

LDFLAGS = 
LD = g++ $(LDFLAGS)

.PHONY: all

all:
	clear
	$(CC) -c $(SRCDIR)/TahaScript.cpp -o $(OBJDIR)/Main.o
	$(CC) -c $(SRCDIR)/Preprocessor.cpp -o $(OBJDIR)/Preprocessor.o
	$(CC) -c $(SRCDIR)/stringFunctions.cpp -o $(OBJDIR)/stringFunctions.o
	$(CC) -c $(SRCDIR)/GlobalSymbolTable.cpp -o $(OBJDIR)/GlobalSymbolTable.o

	$(LD) $(OBJDIR)/Main.o $(OBJDIR)/Preprocessor.o $(OBJDIR)/stringFunctions.o $(OBJDIR)/GlobalSymbolTable.o -o ./tscc