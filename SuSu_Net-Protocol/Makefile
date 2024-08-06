include ../config.mk

TARGET=susu_http-processer.o susu_tcp-object.o susu_http-analyser.o

all:$(TARGET)

susu_http-processer.o:susu_http-processer.cpp
	$(CC) susu_http-processer.cpp $(TEMP)susu_epoll.o -o $(TEMP)susu_http-processer.o

susu_tcp-object.o:susu_tcp-object.cpp
	$(CC) susu_tcp-object.cpp -o $(TEMP)susu_tcp-object.o

susu_http-analyser.o:susu_http-analyser.cpp
	$(CC) susu_http-analyser.cpp -o $(TEMP)susu_http-analyser.o