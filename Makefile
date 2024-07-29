CC=g++ -std=c++11 -pipe -c $(INCLUDE) -O2 -Wall
BUILD=g++ -std=c++11 -pipe $(INCLUDE) -O2 -Wall
LD = -pthread

INCLUDE=-I $(susu_timer) -I $(susu_cache) -I $(susu_init-param) -I $(susu_epoll) -I $(susu_net-protocol) -I $(susu_thread-pool) -I $(susu_task-queue)

TEST=./test/
BIN=./bin/
TEMP=./temp_file/
CPP=*.cpp

#---------------------------------------------------

susu_timer=./SuSu_Timer/
susu_init-param=./SuSu_Init-Param/
susu_epoll=./SuSu_Epoll/
susu_net-protocol=./SuSu_Net-Protocol/
susu_cache=./SuSu_Cache/
susu_httpd=./SuSu_Httpd/
susu_thread-pool=./SuSu_Thread-Pool/
susu_task-queue=./SuSu_Task-Queue/

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


init-param.o:cache.o $(susu_init-param)$(CPP)
	$(CC) $(susu_init-param)$(CPP) -o $(TEMP)susu_init_param.o

cache.o:$(susu_cache)$(CPP)
	$(CC) $(susu_cache)$(CPP) -o $(TEMP)susu_cache.o

http.o:$(susu_net-protocol)susu_http.cpp
	$(CC) $(susu_net-protocol)susu_http.cpp -o $(TEMP)susu_http.o

socket.o:$(susu_net-protocol)susu_socket.cpp
	$(CC) $(susu_net-protocol)susu_socket.cpp -o $(TEMP)susu_socket.o

thread-worker.o:timer.o $(susu_thread-pool)susu_thread_worker.cpp
	$(CC) $(susu_thread-pool)susu_thread_worker.cpp $(TEMP)susu_timer.o -o $(TEMP)susu_thread_worker.o

thread-pool.o:thread-worker.o $(susu_thread-pool)susu_thread_pool.cpp
	$(CC) $(susu_thread-pool)susu_thread_pool.cpp $(TEMP)susu_thread_worker.o -o $(TEMP)susu_thread_pool.o

#---------------------------------------------------
#	test for some tools.

test-init-param:init-param.o $(TEST)test-init-param.cpp
	$(BUILD) $(TEST)test-init-param.cpp $(TEMP)susu_init_param.o $(TEMP)susu_cache.o -o $(BIN)test-init-param.bin $(LD)	
	cp SuSu_Init-Param/example.conf $(BIN)
	$(BIN)test-init-param.bin $(BIN)example.conf

test-cache:timer.o cache.o $(TEST)test-cache.cpp
	$(BUILD) $(TEST)test-cache.cpp $(TEMP)susu_timer.o $(TEMP)susu_cache.o -o $(BIN)test-cache.bin $(LD)
	$(BIN)test-cache.bin

test-epoll:epoll.o $(TEST)test-epoll.cpp
	$(BUILD) $(TEST)test-epoll.cpp $(TEMP)susu_epoll.o -o $(BIN)test-epoll.bin $(LD)
	$(BIN)test-epoll.bin

test-cache-algo:timer.o cache.o $(TEST)test-cache-algo.cpp
	$(BUILD) $(TEST)test-cache-algo.cpp $(TEMP)susu_timer.o $(TEMP)susu_cache.o -o $(BIN)test-cache-algo.bin $(LD)
	$(BIN)test-cache-algo.bin

#test-http:socket.o http.o $(TEST)test-http-server.cpp $(TEST)test-http-client.cpp
#	$(BUILD) $(TEST)test-http-server.cpp $(TEMP)susu_http.o $(TEMP)susu_socket.o -o $(BIN)test-http-server.bin $(LD)
#	$(BUILD) $(TEST)test-http-client.cpp $(TEMP)susu_http.o $(TEMP)susu_socket.o -o $(BIN)test-http-client.bin $(LD)
#	nohup $(BIN)test-http.bin > ./bin/test-http.log &


test-thread-pool:thread-pool.o $(TEST)test-thread-pool.cpp
	$(BUILD) $(TEST)test-thread-pool.cpp $(TEMP)susu_thread_pool.o $(TEMP)susu_thread_worker.o $(TEMP)susu_timer.o -o $(BIN)test-thread.bin $(LD)
	$(BIN)test-thread.bin

test-task-queue:$(TEST)test-task-queue.cpp
	$(BUILD) $(TEST)test-task-queue.cpp -o $(BIN)test-task-queue.bin $(LD)
	$(BIN)test-task-queue.bin

#ifeq ($(curl --version | grep http),"")
#test-http:
#	@echo "Need To Install Curl to test susu_http"
#else
#test-http:$(TEST)test-http.cpp
#	$(BUILD) $(TEST)test-http.cpp $(TEMP)susu_http.o -o $(BIN)test-http.bin $(LD)
#	nohup $(BIN)test-http.bin > ./bin/test-http.log &
#	
#
#	curl --http0.9 "http://127.0.0.1:9527" -X POST -H "Content-Type: application/json" -d "{\"username\":\"admin\",\"password\":\"123456\"}" --output log.txt
#endif


#---------------------------------------------------
#	some useful application.
#
test-httpd:init-param.o $(susu_httpd)susu_httpd.cpp
	$(BUILD) $(susu_httpd)susu_httpd.cpp $(TEMP)susu_init_param.o $(TEMP)susu_cache.o $(TEMP)susu_socket.o $(TEMP)susu_epoll.o -o $(BIN)susu_httpd.bin $(LD)	
	cp $(susu_httpd)/susu_httpd.conf $(BIN)
	$(BIN)susu_httpd.bin $(BIN)susu_httpd.conf

.PHONY:clean
clean:
	rm ./temp_file/*
	rm ./bin/*
