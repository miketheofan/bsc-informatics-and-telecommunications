OBJS = travelMonitorClient.o monitorServer.o DirectoriesList.o messageProtocol.o BloomFilter.o BloomFiltersList.o BloomList.o Citizen.o functions.o RecordsList.o RequestList.o SkipList.o DirectoriesQueue.o CitizensHash.o
P_OBJS = travelMonitorClient.o DirectoriesList.o messageProtocol.o BloomFilter.o BloomFiltersList.o BloomList.o Citizen.o functions.o RecordsList.o RequestList.o SkipList.o DirectoriesQueue.o CitizensHash.o
C_OBJS = monitorServer.o DirectoriesList.o messageProtocol.o BloomFilter.o BloomFiltersList.o BloomList.o Citizen.o functions.o RecordsList.o RequestList.o SkipList.o DirectoriesQueue.o CitizensHash.o
SOURCE = travelMonitorClient.cpp monitorServer.cpp DirectoriesList.cpp messageProtocol.cpp BloomFilter.cpp BloomFiltersList.cpp BloomList.cpp Citizen.cpp functions.cpp RecordsList.cpp RequestList.cpp SkipList.cpp DirectoriesQueue.cpp CitizensHash.cpp
HEADER = travelMonitorClient.hpp monitorServer.hpp DirectoriesList.hpp messageProtocol.hpp BloomFilter.hpp BloomFiltersList.hpp BloomList.hpp Citizen.hpp functions.hpp RecordsList.hpp RequestList.hpp SkipList.hpp DirectoriesQueue.hpp CitizensHash.hpp
OUT = travelMonitorClient
OUT2 = monitorServer
CC = g++ -std=c++11 -pthread -lpthread
FLAGS = -g -c

all: $(P_OBJS) $(C_OBJS)
	$(CC) -g $(P_OBJS) -o $(OUT)
	$(CC) -g $(C_OBJS) -o $(OUT2)

travelMonitorClient.o: travelMonitorClient.cpp
	$(CC) $(FLAGS) travelMonitorClient.cpp

monitorServer.o: monitorServer.cpp
	$(CC) $(FLAGS) monitorServer.cpp

DirectoriesList.o: DirectoriesList.cpp
	$(CC) $(FLAGS) DirectoriesList.cpp

messageProtocol.o: messageProtocol.cpp
	$(CC) $(FLAGS) messageProtocol.cpp

BloomFilter.o: BloomFilter.cpp
	$(CC) $(FLAGS) BloomFilter.cpp

BloomFiltersList.o: BloomFiltersList.cpp
	$(CC) $(FLAGS) BloomFiltersList.cpp

BloomList.o: BloomList.cpp
	$(CC) $(FLAGS) BloomList.cpp

Citizen.o: Citizen.cpp
	$(CC) $(FLAGS) Citizen.cpp

functions.o: functions.cpp
	$(CC) $(FLAGS) functions.cpp

RecordsList.o: RecordsList.cpp
	$(CC) $(FLAGS) RecordsList.cpp

RequestList.o: RequestList.cpp
	$(CC) $(FLAGS) RequestList.cpp

SkipList.o: SkipList.cpp
	$(CC) $(FLAGS) SkipList.cpp

DirectoriesQueue.o: DirectoriesQueue.cpp
	$(CC) $(FLAGS) DirectoriesQueue.cpp

CitizensHash.o: CitizensHash.cpp
	$(CC) $(FLAGS) CitizensHash.cpp

run:
	@./$(OUT) -m 3 -b 300 -c 5 -s 200 -i input_dir -t 4

debug:
	@valgrind --trace-children=yes ./$(OUT) -m 1 -b 300 -c 5 -s 200 -i input_dir -t 2

clean:
	@rm -rf $(OBJS) $(OUT) $(OUT2)