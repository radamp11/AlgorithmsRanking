CC = g++
CFLAGS = -Wall -std=c++11 -lm

all: main.o ackley.o individual.o algorithm.o
	$(CC) $(CFLAGS) -o fun main.o ackley.o algorithm.o individual.o

individual.o: Individual.hpp Individual.cpp
	$(CC) $(CFLAGS) -c -o individual.o Individual.cpp

algorithm.o: algorithm.hpp algorithm.cpp 
	$(CC) $(CFLAGS) -c -o algorithm.o algorithm.cpp

ackley.o: ackley.hpp ackley.cpp 
	$(CC) $(CFLAGS) -c -o ackley.o ackley.cpp

main.o: main.cpp
	$(CC) $(CFLAGS) -c -o main.o main.cpp

clean:
	rm -f *.o
