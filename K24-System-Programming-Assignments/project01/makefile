OBJS = vaccineMonitor.o BloomFilter.o functions.o SkipList.o Citizens.o virusesList.o citizensList.o countriesList.o agesList.o stringsList.o
SOURCE = vaccineMonitor.cpp BloomFilter.cpp functions.cpp SkipList.cpp Citizens.cpp virusesList.cpp citizensList.cpp countriesList.cpp agesList.cpp stringsList.cpp
HEADER = vaccineMonitor.hpp BloomFilter.hpp functions.hpp SkipList.hpp Citizens.hpp virusesList.hpp citizensList.hpp countriesList.hpp agesList.hpp stringsList.hpp
OUT = vaccineMonitor
CC = g++ -std=c++11
FLAGS = -g -c

all: $(OBJS)
	$(CC) -g $(OBJS) -o $(OUT)

vaccineMonitor.o: vaccineMonitor.cpp
	$(CC) $(FLAGS) vaccineMonitor.cpp

BloomFilter.o: BloomFilter.cpp
	$(CC) $(FLAGS) BloomFilter.cpp

functions.o: functions.cpp
	$(CC) $(FLAGS) functions.cpp

SkipList.o: SkipList.cpp
	$(CC) $(FLAGS) SkipList.cpp

Citizens.o: Citizens.cpp
	$(CC) $(FLAGS) Citizens.cpp

virusesList.o: virusesList.cpp
	$(CC) $(FLAGS) virusesList.cpp

citizensList.o: citizensList.cpp
	$(CC) $(FLAGS) citizensList.cpp

countriesList.o: countriesList.cpp
	$(CC) $(FLAGS) countriesList.cpp

agesList.o: agesList.cpp
	$(CC) $(FLAGS) agesList.cpp

stringsList.o: stringsList.cpp
	$(CC) $(FLAGS) stringsList.cpp

run:
	@./$(OUT) -c citizenRecordsFile.txt -b 800

sh:
	@./testFile.sh virusesFile countriesFile 500 0

debug:
	@valgrind ./$(OUT) -c citizenRecordsFile.txt -b 800

leaks:
	@valgrind --leak-check=full ./$(OUT) -c citizenRecordsFile.txt -b 800

clean:
	@rm -rf $(OBJS) $(OUT)
