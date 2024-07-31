include config.mk

#---------------------------------------------------
#	these code can check if the TEMP_FOLDER exits

ifeq ($(wildcard $(TEMP_FOLDER)),)
folder_check:
	@echo "Creating $(TEMP_FOLDER) folder"
	@mkdir -p $(TEMP_FOLDER)
else
folder_check:
	@echo "$(TEMP_FOLDER) folder already exists"
endif

#---------------------------------------------------
#	call other makefile

all:folder_check SUSU_TOOLS TEST_ALL

SUSU_TOOLS:TIMER CACHE INIT-PARAM HTTP SOCKET THREAD-WORKER THREAD-POOL EPOLL
#---------------------------------------------------
#	SUSU_TOOLS
TIMER:
	cd $(susu_timer) && $(MAKE) all ROOT=$(ROOT);
	cd ../

CACHE:
	cd $(susu_cache) && $(MAKE) all ROOT=$(ROOT);
	cd ../

EPOLL:
	cd $(susu_epoll) && $(MAKE) all ROOT=$(ROOT);
	cd ../

INIT-PARAM:CACHE
	cd $(susu_init-param) && $(MAKE) all ROOT=$(ROOT);
	cd ../

HTTP:
	cd $(susu_net-protocol) && $(MAKE) susu_http.o ROOT=$(ROOT);
	cd ../

SOCKET:
	cd $(susu_net-protocol) && $(MAKE) susu_socket.o ROOT=$(ROOT);
	cd ../

THREAD-WORKER:TIMER
	cd $(susu_thread-pool) && $(MAKE) susu_thread-worker.o ROOT=$(ROOT);
	cd ../
THREAD-POOL:TIMER
	cd $(susu_thread-pool) && $(MAKE) susu_thread-pool.o ROOT=$(ROOT);
	cd ../	

#---------------------------------------------------
#	TEST_ALL
TEST_ALL:folder_check test-cache test-cache-algo test-init-param test-epoll
#---------------------------------------------------
#	test for some tools.

test-init-param:INIT-PARAM $(TEST)test-init-param.cpp
	$(BUILD) $(TEST)test-init-param.cpp $(TEMP)susu_init-param.o $(TEMP)susu_cache.o -o $(BIN)test-init-param.bin $(LD)	
	cp SuSu_Init-Param/example.conf $(BIN)
	$(BIN)test-init-param.bin $(BIN)example.conf

test-cache:TIMER CACHE $(TEST)test-cache.cpp
	$(BUILD) $(TEST)test-cache.cpp $(TEMP)susu_timer.o $(TEMP)susu_cache.o -o $(BIN)test-cache.bin $(LD)
	$(BIN)test-cache.bin

test-epoll:EPOLL $(TEST)test-epoll.cpp
	$(BUILD) $(TEST)test-epoll.cpp $(TEMP)susu_epoll.o -o $(BIN)test-epoll.bin $(LD)
	$(BIN)test-epoll.bin

test-cache-algo:TIMER CACHE $(TEST)test-cache-algo.cpp
	$(BUILD) $(TEST)test-cache-algo.cpp $(TEMP)susu_timer.o $(TEMP)susu_cache.o -o $(BIN)test-cache-algo.bin $(LD)
	$(BIN)test-cache-algo.bin

#test-http:socket.o http.o $(TEST)test-http-server.cpp $(TEST)test-http-client.cpp
#	$(BUILD) $(TEST)test-http-server.cpp $(TEMP)susu_http.o $(TEMP)susu_socket.o -o $(BIN)test-http-server.bin $(LD)
#	$(BUILD) $(TEST)test-http-client.cpp $(TEMP)susu_http.o $(TEMP)susu_socket.o -o $(BIN)test-http-client.bin $(LD)
#	nohup $(BIN)test-http.bin > ./bin/test-http.log &


test-thread-pool:THREAD-POLL $(TEST)test-thread-pool.cpp
	$(BUILD) $(TEST)test-thread-pool.cpp $(TEMP)susu_thread-pool.o $(TEMP)susu_thread-worker.o $(TEMP)susu_TIMER -o $(BIN)test-thread.bin $(LD)
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
	$(BUILD) $(susu_httpd)susu_httpd.cpp $(TEMP)susu_init_param.o $(TEMP)susu_cache.o $(TEMP)susu_socket.o $(TEMP)susu_EPOLL -o $(BIN)susu_httpd.bin $(LD)	
	cp $(susu_httpd)/susu_httpd.conf $(BIN)
	$(BIN)susu_httpd.bin $(BIN)susu_httpd.conf

HTTPD:

RUN-HTTPD:


.PHONY:clean
clean:
	rm ./temp_file/*
	rm ./bin/*


#some useless example code

#all:
#names = a b c d
#files := $(foreach n,$(names),$(n).o)
#demo:
# cd subdir && \
# for x in $(files); do\
# echo $$a;\
# done

#susu-tools:
#	for x in $(ALL_SOURCE_FOLDER) ; do \
#		cd $$x && $(MAKE) all ROOT=$(ROOT);	\
#		cd ../ ;	\
#    done
#I think I must describe about $$x
#	in makefile:
#	x = char x
#	$(x) = the value of variable x
#	$($(x)): t = $(x), $($(x)) = $(t)
#	$$x  $x  :makefile have no such code,

#	BUT in shell (bash) :
#	x = char x
#	$x = string "$x"
#	$$x	= the value of variable x