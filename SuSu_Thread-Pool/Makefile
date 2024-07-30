include ../config.mk

TARGET=susu_thread-worker.o susu_thread-pool.o

all:$(TARGET)

susu_thread-worker.o:susu_thread-worker.cpp
	$(CC) susu_thread-worker.cpp $(TEMP)susu_timer.o -o $(TEMP)susu_thread-worker.o 

susu_thread-pool.o:susu_thread-pool.cpp
	$(CC) susu_thread-pool.cpp $(TEMP)susu_timer.o -o $(TEMP)susu_thread-pool.o 