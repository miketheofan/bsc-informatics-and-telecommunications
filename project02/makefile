OBJS = travelMonitor.o Monitor.o BloomFilter.o functions.o messageProtocol.o Citizen.o BloomList.o SkipList.o DirectoriesList.o BloomFiltersList.o RequestList.o RecordsList.o Signals.o
P_OBJS = travelMonitor.o BloomFilter.o functions.o messageProtocol.o Citizen.o BloomList.o SkipList.o DirectoriesList.o BloomFiltersList.o RequestList.o RecordsList.o Signals.o
C_OBJS = Monitor.o BloomFilter.o functions.o messageProtocol.o Citizen.o BloomList.o SkipList.o DirectoriesList.o BloomFiltersList.o RequestList.o RecordsList.o Signals.o
SOURCE = travelMonitor.cpp Monitor.cpp BloomFilter.cpp functions.cpp messageProtocol.cpp Citizen.cpp BloomList.cpp SkipList.cpp DirectoriesList.cpp BloomFiltersList.cpp RequestList.cpp RecordsList.cpp Signals.cpp
HEADER = travelMonitor.hpp Monitor.hpp BloomFilter.hpp functions.hpp messageProtocol.hpp Citizen.hpp BloomList.hpp SkipList.hpp DirectoriesList.hpp BloomFiltersList.hpp RequestList.hpp RecordsList.hpp Signals.hpp
OUT = travelMonitor
OUT2 = Monitor
CC = g++ -std=c++11
FLAGS = -g -c

all: $(P_OBJS) $(C_OBJS)
	$(CC) -g $(P_OBJS) -o $(OUT)
	$(CC) -g $(C_OBJS) -o $(OUT2)

travelMonitor.o: travelMonitor.cpp
	$(CC) $(FLAGS) travelMonitor.cpp

Monitor.o: Monitor.cpp
	$(CC) $(FLAGS) Monitor.cpp

BloomFilter.o: BloomFilter.cpp
	$(CC) $(FLAGS) BloomFilter.cpp

functions.o: functions.cpp
	$(CC) $(FLAGS) functions.cpp

messageProtocol.o: messageProtocol.cpp
	$(CC) $(FLAGS) messageProtocol.cpp

Citizen.o: Citizen.cpp
	$(CC) $(FLAGS) Citizen.cpp

BloomList.o: BloomList.cpp
	$(CC) $(FLAGS) BloomList.cpp

SkipList.o: SkipList.cpp
	$(CC) $(FLAGS) SkipList.cpp

DirectoriesList.o: DirectoriesList.cpp
	$(CC) $(FLAGS) DirectoriesList.cpp

BloomFiltersList.o: BloomFiltersList.cpp
	$(CC) $(FLAGS) BloomFiltersList.cpp

RequestList.o: RequestList.cpp
	$(CC) $(FLAGS) RequestList.cpp

RecordsList.o: RecordsList.cpp
	$(CC) $(FLAGS) RecordsList.cpp

Signals.o: Signals.cpp
	$(CC) $(FLAGS) Signals.cpp

run:
	@./$(OUT) -m 3 -b 300 -s 200 -i input_dir

sh:
	@./testFile.sh citizensRecordsFile.txt input_dir 6

debug:
	@valgrind --leak-check=full ./$(OUT) -m 3 -b 300 -s 200 -i input_dir

clean:
	@rm -rf $(OBJS) $(OUT) $(OUT2)
