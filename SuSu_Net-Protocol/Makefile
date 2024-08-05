include ../config.mk

TARGET=susu_http.o susu_socket.o

all:$(TARGET)

susu_http-processer.o:susu_http-processer.cpp
	$(CC) susu_http-processer.cpp -o $(TEMP)susu_http-processer.o

susu_tcp-object.o:susu_tcp-object.cpp
	$(CC) susu_tcp-object.cpp -o $(TEMP)susu_tcp-object.o