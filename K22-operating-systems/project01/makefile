OBJS = mngstd.o functions.o InvertedIndex.o StudentList.o HashTable.o HashNode.o
SOURCE = mngstd.cpp functions.cpp InvertedIndex.cpp StudentList.cpp HashTable.cpp HashNode.cpp
HEADER = types.hpp 
OUT = mngstd
CC = g++ -std=c++11
FLAGS = -g -c

all: $(OBJS)
	$(CC) -g $(OBJS) -o $(OUT)

mngstd.o: mngstd.cpp
	$(CC) $(FLAGS) mngstd.cpp

functions.o: functions.cpp
	$(CC) $(FLAGS) functions.cpp

InvertedIndex.o: InvertedIndex.cpp
	$(CC) $(FLAGS) InvertedIndex.cpp

StudentList.o: StudentList.cpp
	$(CC) $(FLAGS) StudentList.cpp

HashTable.o: HashTable.cpp
	$(CC) $(FLAGS) HashTable.cpp

HashNode.o: HashNode.cpp
	$(CC) $(FLAGS) HashNode.cpp

clean:
	rm -f $(OBJS) $(OUT)