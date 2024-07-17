CC=g++ -std=c++11 -pipe -O2 -c -Wall $(INCLUDE)
BUILD=g++ -std=c++11 -pipe -O2 -Wall $(INCLUDE)
LD = -pthread

INCLUDE= -I$(susu_timer) -I$(susu_cache) -I$(susu_initparam) -I$(susu_epoll) -I$(susu_netprotocol) -I$(susu_fdstream)

TEST=./test/
BIN=./bin/
TEMP=./temp_file/
CPP=*.cpp

#---------------------------------------------------
#	these code can be complie to *.o.

susu_timer=./SuSu_Timer/
susu_initparam=./SuSu_InitParam/
susu_epoll=./SuSu_Epoll/
susu_net-protocol=./SuSu_NetProtocol/
susu_cache=./SuSu_Cache/
susu_fdstream=./SuSu_FdStream/


#---------------------------------------------------
#	these code can check if the folder exits
	
TEMP_FOLDER_NAME=./bin ./temp_file

ifeq ($(wildcard $(TEMP_FOLDER_NAME)),)
folder_check:
	@echo "Creating $(TEMP_FOLDER_NAME) folder"
	@mkdir -p $(TEMP_FOLDER_NAME)
else
folder_check:
	@echo "$(TEMP_FOLDER_NAME) folder already exists"
	#do nothing
endif





all:folder_check susu-tools test-all

susu-tools:timer.o epoll.o cache.o initparam.o

test-all:folder_check test-cache test-cache-algo test-initparam test-epoll

#--------------------------------------------------
#	all the .o files

timer.o:$(susu_timer)$(CPP)
	$(CC) $(susu_timer)$(CPP) -o $(TEMP)susu_timer.o

epoll.o:$(susu_epoll)$(CPP)
	$(CC) $(susu_epoll)$(CPP) -o $(TEMP)susu_epoll.o


initparam.o:cache.o $(susu_initparam)$(CPP)
	$(CC) $(susu_initparam)$(CPP) -o $(TEMP)susu_initparam.o

cache.o:$(susu_cache)$(CPP)
	$(CC) $(susu_cache)$(CPP) -o $(TEMP)susu_cache.o

http.o:$(susu_net-protocol)susu_http.cpp
	$(CC) $(susu_net-protocol)susu_http -o $(TEMP)susu_http.o

fdstream.o:$(susu_fdstream)$(CPP)
	$(CC) $(susu_fdstream)$(CPP) -o $(TEMP)susu_fdstream.o

#---------------------------------------------------
#	test for some tools.

test-initparam:initparam.o $(TEST)test-initparam.cpp
	$(BUILD) $(TEST)test-initparam.cpp $(TEMP)susu_initparam.o $(TEMP)susu_cache.o -o $(BIN)test-initparam.bin $(LD)	
	cp SuSu_InitParam/example.conf ./bin/
	./bin/test-initparam.bin ./bin/example.conf

test-cache:timer.o cache.o $(TEST)test-cache.cpp
	$(BUILD) $(TEST)test-cache.cpp $(TEMP)susu_timer.o $(TEMP)susu_cache.o -o $(BIN)test-cache.bin $(LD)
	./bin/test-cache.bin

test-epoll:epoll.o $(TEST)test-epoll.cpp
	$(BUILD) $(TEST)test-epoll.cpp $(TEMP)susu_epoll.o -o $(BIN)test-epoll.bin $(LD)
	./bin/test-epoll.bin

test-cache-algo:timer.o cache.o $(TEST)test-cache-algo.cpp
	$(BUILD) $(TEST)test-cache-algo.cpp $(TEMP)susu_timer.o $(TEMP)susu_cache.o -o $(BIN)test-cache-algo.bin $(LD)
	./bin/test-cache-algo.bin

.PHONY:clean
clean:
	rm ./temp_file/*
	rm ./bin/*
