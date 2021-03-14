OBJS = quic.o copyFunctions.o deleteFunctions.o
SOURCE = quic.cpp copyFunctions.cpp deleteFunctions.cpp
OUT = quic
CC = g++ -std=c++11
FLAGS = -g -c

all: $(OBJS)
	$(CC) -g $(OBJS) -o $(OUT)

quic.o: quic.cpp
	$(CC) $(FLAGS) quic.cpp

copyFunctions.o: copyFunctions.cpp
	$(CC) $(FLAGS) copyFunctions.cpp

deleteFunctions.o: deleteFunctions.cpp
	$(CC) $(FLAGS) deleteFunctions.cpp

clean:
	rm $(OBJS) $(OUT)