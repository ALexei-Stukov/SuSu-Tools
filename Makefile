include config.mk

#---------------------------------------------------
#	these code can check if the TEMP_FOLDER exits

folder_check:$(BIN) $(TEMP)

$(BIN):
	@echo "Folder $(BIN) does not exist"
	mkdir -p $@

$(TEMP):
	@echo "Folder $(TEMP) does not exist"
	mkdir -p $@
#ifeq ($(wildcard $(TEMP_FOLDER)),)
#folder_check:
#	@echo "Creating $(TEMP_FOLDER) folder"
#	@mkdir -p $(TEMP_FOLDER)
#else
#folder_check:
#	@echo "$(TEMP_FOLDER) folder already exists"
#endif

#---------------------------------------------------
#	call other makefile

all:folder_check SUSU_TOOLS TEST_ALL

SUSU_TOOLS:TIMER CACHE INIT-PARAM TCP-OBJECT HTTP-OBJECT SOCKET THREAD-WORKER THREAD-POOL EPOLL 
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

TCP-OBJECT:
	cd $(susu_net-protocol) && $(MAKE) susu_tcp-object.o ROOT=$(ROOT);
	cd ../

HTTP-PROCESSER:HTTP-READER HTTP-WRITER EPOLL
	cd $(susu_net-protocol) && $(MAKE) susu_http-processer.o ROOT=$(ROOT);
	cd ../

HTTP-READER:
	cd $(susu_net-protocol) && $(MAKE) susu_http-reader.o ROOT=$(ROOT);
	cd ../

HTTP-WRITER:
	cd $(susu_net-protocol) && $(MAKE) susu_http-writer.o ROOT=$(ROOT);
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
TEST_ALL:folder_check test-cache test-cache-algo test-init-param test-thread-pool test-task-queue 
#---------------------------------------------------
#	test for some tools.

test-init-param:INIT-PARAM $(TEST)test-init-param.cpp
	$(BUILD) $(TEST)test-init-param.cpp $(TEMP)susu_init-param.o $(TEMP)susu_cache.o -o $(BIN)test-init-param.bin $(LD)	
	cp SuSu_Init-Param/example.conf $(BIN)
	$(BIN)test-init-param.bin $(BIN)example.conf

test-cache:TIMER CACHE $(TEST)test-cache.cpp
	$(BUILD) $(TEST)test-cache.cpp $(TEMP)susu_timer.o $(TEMP)susu_cache.o -o $(BIN)test-cache.bin $(LD)
	$(BIN)test-cache.bin

test-cache-algo:TIMER CACHE $(TEST)test-cache-algo.cpp
	$(BUILD) $(TEST)test-cache-algo.cpp $(TEMP)susu_timer.o $(TEMP)susu_cache.o -o $(BIN)test-cache-algo.bin $(LD)
	$(BIN)test-cache-algo.bin

#test-http-client:
#	$(BUILD) $(TEST)test-http-server.cpp $(TEMP)susu_http.o $(TEMP)susu_socket.o -o $(BIN)test-http-server.bin $(LD)
#	$(BUILD) $(TEST)test-http-client.cpp $(TEMP)susu_tcp-object.o -o $(BIN)test-http-client.bin $(LD)
#	nohup $(BIN)test-http.bin > ./bin/test-http.log &


test-thread-pool:THREAD-POOL $(TEST)test-thread-pool.cpp
	$(BUILD) $(TEST)test-thread-pool.cpp $(TEMP)susu_thread-pool.o $(TEMP)susu_thread-worker.o $(TEMP)susu_TIMER -o $(BIN)test-thread.bin $(LD)
	$(BIN)test-thread.bin

test-task-queue:$(TEST)test-task-queue.cpp
	$(BUILD) $(TEST)test-task-queue.cpp -o $(BIN)test-task-queue.bin $(LD)
	$(BIN)test-task-queue.bin

#---------------------------------------------------
#	some useful application.
#
test-httpd:INIT-PARAM TCP-OBJECT THREAD-WORKER THREAD-POOL HTTP-PROCESSER $(susu_httpd)susu_httpd.cpp
	$(DEBUG) $(susu_httpd)susu_httpd.cpp $(TEMP)susu_init-param.o $(TEMP)susu_thread-worker.o $(TEMP)susu_thread-pool.o $(TEMP)susu_cache.o $(TEMP)susu_tcp-object.o $(TEMP)susu_epoll.o $(TEMP)susu_http-processer.o $(TEMP)susu_http-reader.o $(TEMP)susu_http-writer.o -o $(BIN)susu_httpd.bin $(LD)	
	cp $(susu_httpd)*.conf $(BIN)
	cp $(susu_httpd)*.lua $(BIN)
	cp $(susu_httpd)*.html $(BIN)
	cp $(susu_httpd)*.ico $(BIN)
	cd $(BIN) && $(BIN)susu_httpd.bin $(BIN)susu_httpd.conf

HTTPD:

RUN-HTTPD:


.PHONY:clean


clean:
ifeq ($(shell ls "$(BIN)"),)
	@echo $(BIN) is empty
else
	rm -r $(BIN)*
endif

ifeq ($(shell ls "$(TEMP)"),)
	@echo $(TEMP) is empty
else
	rm -r $(TEMP)*
endif

#ifeq ($(ls $(BIN)),)
#else
#	@rm $(BIN) -r 
#endif

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
