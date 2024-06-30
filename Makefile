CC=g++ -std=c++11 -pipe -O2 -c -Wall $(INCLUDE)
BUILD=g++ -std=c++11 -pipe -O2 -Wall $(INCLUDE)
LD = -pthread

INCLUDE= -I$(susu_timer) -I$(susu_cache) -I$(susu_initparam) -I$(susu_epoll)

TEST=./test/
BIN=./bin/
TEMP=./temp_file/
CPP=*.cpp

#---------------------------------------------------
#	these code can be complie to *.o.

susu_timer=./SuSu_Timer/
susu_initparam=./SuSu_InitParam/
susu_epoll=./SuSu_Epoll/

#---------------------------------------------------
#	Some test bins abut template-tools.
#	Because of template,these code can't be complie to *.o.
#	If you wan to use these template-tools,just	[  #include "path_to_the_*.hpp"  ]

susu_cache=./SuSu_Cache/






all:susu-tools test-all

susu-tools:timer.o 

test-all:test-cache test-initparam



#--------------------------------------------------
#	all the .o files

timer.o:$(susu_timer)susu_timer.cpp
	$(CC) $(susu_timer)susu_timer.cpp -o $(TEMP)susu_timer.o

epoll.o:$(susu_epoll)susu_epoll.cpp
	$(CC) $(susu_timer)susu_timer.cpp -o $(TEMP)susu_timer.o


initparam.o:$(susu_initparam)susu_initparam.cpp
	$(CC) $(susu_initparam)susu_initparam.cpp -o $(TEMP)susu_initparam.o

cache.o:$(susu_cache)$(CPP)
	$(CC) $(susu_cache)$(CPP) -o $(TEMP)susu_cache.o




#---------------------------------------------------
#	test for some tools.

test-initparam:initparam.o $(TEST)test-initparam.cpp
	$(BUILD) $(TEST)test-initparam.cpp $(TEMP)susu_initparam.o -o $(BIN)test-initparam.bin $(LD)	
	cp SuSu_InitParam/example.conf ./bin/
	./bin/test-initparam.bin ./bin/example.conf

test-cache:timer.o cache.o $(TEST)test-cache.cpp
	$(BUILD) $(TEST)test-cache.cpp $(TEMP)susu_timer.o $(TEMP)susu_cache.o -o $(BIN)test-cache.bin $(LD)
	./bin/test-cache.bin

test-epoll:$(TEST)test-epoll.cpp
	$(BUILD) $(TEST)test-epoll.cpp -o $(BIN)test-epoll.bin $(LD)
	./bin/test-epoll.bin




.PHONY:clean
clean:
	rm ./temp_file/*
	rm ./bin/*
