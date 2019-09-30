
OPTIONS = -O0 -Wall

all : test_pile test_file lab4

sudoku: sudoku.c pile.h thread.h reader.h
	g++ $(OPTIONS) -o sudoku sudoku.c

test_pile: test_pile.cpp pile.h
	g++ $(OPTIONS) -o test_pile test_pile.cpp

test_file: test_file.cpp file.h
	g++ $(OPTIONS) -o test_file test_file.cpp

clean :
	rm -rf test_pile test_file
	rm -f lab4
