CC = g++
CFLAGS = -Wall -std=c++11 -lm -lsqlite3

all: main.o database.o
	$(CC) $(CFLAGS) -o fun main.o database.o

database.o: DataBase.hpp DataBase.cpp 
	$(CC) $(CFLAGS) -c -o database.o DataBase.cpp

main.o: main.cpp
	$(CC) $(CFLAGS) -c -o main.o main.cpp

clean:
	rm -f *.o