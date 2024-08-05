include ../config.mk

TARGET=susu_http.o susu_socket.o

all:$(TARGET)

susu_http-object.o:susu_tcp-object.o susu_http-analyser.o susu_http-object.cpp
	$(CC) susu_http-object.cpp $(TEMP)susu_http-analyser.o $(TEMP)susu_tcp-object.o -o $(TEMP)susu_http-object.o

susu_http-analyser.o:susu_http-analyser.cpp
	$(CC) susu_http-analyser.cpp -o $(TEMP)susu_http-analyser.o

susu_tcp-object.o:susu_tcp-object.cpp
	$(CC) susu_tcp-object.cpp -o $(TEMP)susu_tcp-object.o