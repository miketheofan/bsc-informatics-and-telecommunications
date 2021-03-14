OBJS = chef.o saladmaker.o functions.o logfile.o list.o
SOURCE = chef.cpp saladmaker.cpp functions.cpp logfile.cpp list.cpp
CHEF_PROCESS = chef.o functions.o logfile.o list.o
SALADMAKER_PROCESS = saladmaker.o functions.o logfile.o list.o
OUT1 = chef
OUT2 = saladmaker
CC = g++ -std=c++11
FLAGS = -g -c
THREADS = -lpthread

all: $(OBJS) $(CHEF_PROCESS) $(SALADMAKER_PROCESS)
	$(CC) -g $(CHEF_PROCESS) -o $(OUT1) $(THREADS)
	$(CC) -g $(SALADMAKER_PROCESS) -o $(OUT2) $(THREADS)

chef.o: chef.cpp
	$(CC) $(FLAGS) chef.cpp

saladmaker.o: saladmaker.cpp
	$(CC) $(FLAGS) saladmaker.cpp

functions.o: functions.cpp
	$(CC) $(FLAGS) functions.cpp

logfile.o: logfile.cpp
	$(CC) $(FLAGS) logfile.cpp

list.o: list.cpp
	$(CC) $(FLAGS) list.cpp

runc:
	@./$(OUT1) -n 14 -m 3

runs:
	@read -p "Give me the shmid:" shmid; \
	read -p "Give me the maker's order:" order; \
	./$(OUT2) -t1 1 -t2 6 -s $$shmid -o $$order

valgrindc:
	@valgrind --leak-check=full --show-leak-kinds=all ./$(OUT1) -n 40 -m 3

valgrinds:
	@read -p "Give me the shmid:" shmid; \
	read -p "Give me the maker's order:" order; \
	valgrind --leak-check=full --show-leak-kinds=all ./$(OUT2) -t1 1 -t2 6 -s $$shmid -o $$order

clean:
	@rm *.txt *.o chef saladmaker