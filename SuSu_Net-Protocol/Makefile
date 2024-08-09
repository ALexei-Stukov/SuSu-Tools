include ../config.mk

TARGET=susu_http-processer.o susu_tcp-object.o susu_http-analyser.o susu_http-writer.o

all:$(TARGET)

susu_http-processer.o:susu_http-processer.cpp susu_http-writer.o susu_http-reader.o
	$(CC) susu_http-processer.cpp $(TEMP)susu_http-reader.o $(TEMP)susu_epoll.o $(TEMP)susu_http-writer.o -o $(TEMP)susu_http-processer.o

susu_tcp-object.o:susu_tcp-object.cpp
	$(CC) susu_tcp-object.cpp -o $(TEMP)susu_tcp-object.o

susu_http-reader.o:susu_http-reader.cpp
	$(CC) susu_http-reader.cpp -o $(TEMP)susu_http-reader.o

susu_http-writer.o:susu_http-writer.cpp
	$(CC) susu_http-writer.cpp -o $(TEMP)susu_http-writer.o