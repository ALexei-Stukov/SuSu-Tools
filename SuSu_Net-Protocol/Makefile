include ../config.mk

TARGET=susu_http.o susu_socket.o

all:$(TARGET)

susu_http.o:susu_http.cpp
	$(CC) susu_http.cpp -o $(TEMP)susu_http.o

susu_socket.o:susu_socket.cpp
	$(CC) susu_socket.cpp -o $(TEMP)susu_socket.o