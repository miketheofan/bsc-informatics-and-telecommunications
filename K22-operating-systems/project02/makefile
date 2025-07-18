OBJS = myprimes.o functions.o list.o
L_PROCESS = l.o functions.o list.o
W_PROCESS = w.o functions.o
ALL_OBJS = myprimes.o l.o w.o functions.o list.o
ALL_OUTS = myprimes l w
SOURCE = myprimes.cpp l.cpp w.cpp functions.cpp 
HEADER = types.hpp list.hpp l.hpp w.hpp myprimes.hpp functions.hpp
OUT1 = myprimes
OUT2 = l
OUT3 = w
CC = g++ -std=c++11
FLAGS = -g -c

all: $(OBJS) $(L_PROCESS) $(W_PROCESS)
	$(CC) -g $(L_PROCESS) -o $(OUT2)
	$(CC) -g $(W_PROCESS) -o $(OUT3)
	$(CC) -g $(OBJS) -o $(OUT1)

myprimes.o: myprimes.cpp
	$(CC) $(FLAGS) myprimes.cpp

l.o: l.cpp
	$(CC) $(FLAGS) l.cpp

w.o: w.cpp
	$(CC) $(FLAGS) w.cpp

functions.o: functions.cpp
	$(CC) $(FLAGS) functions.cpp

list.o: list.cpp
	$(CC) $(FLAGS) list.cpp

clean:
	rm -f $(ALL_OBJS) $(ALL_OUTS)
