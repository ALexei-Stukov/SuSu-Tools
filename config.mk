#---------------------------------------------------
#	the root path of this repo
ROOT:=$(shell pwd)/
#	we need to call another makefile,so the root path is nesecdary.
#   $(MAKE) xxx ROOT=$(ROOT);  is very important

#---------------------------------------------------
#	source code folders
SOURCE_FOLDER=	susu_timer \
				susu_init-param \
				susu_epoll \
				susu_net-protocol \
				susu_cache \
				susu_httpd \
				susu_thread-pool \
				susu_task-queue	\
				TEST

susu_timer=./SuSu_Timer/
susu_init-param=./SuSu_Init-Param/
susu_epoll=./SuSu_Epoll/
susu_net-protocol=./SuSu_Net-Protocol/
susu_cache=./SuSu_Cache/
susu_httpd=./SuSu_Httpd/
susu_thread-pool=./SuSu_Thread-Pool/
susu_task-queue=./SuSu_Task-Queue/
TEST=./test/

# ALL_SOURCE_FOLDER is used by shell.
# shell-language is different from makefile-language.
# So I had to set the ALL_SOURCE_FOLDER by this way.

ALL_SOURCE_FOLDER := $(foreach n,$(SOURCE_FOLDER),$(ROOT)$($(n)) )

TEMP=$(ROOT)./temp_file/
BIN=$(ROOT)./bin/

#---------------------------------------------------
#	temp folders
TEMP_FOLDER	=	$(BIN)		\
				$(TEMP)


#---------------------------------------------------
#	setting for compile
INCLUDE := $(foreach n,$(ALL_SOURCE_FOLDER),-I $(n))				# foreach is useful,but maybe will add some space at head or tail,need strip function to remove these space
#INCLUDE = -I $(susu_timer) -I $(susu_init-param)......

CC=g++ -std=c++11 -pipe -c $(INCLUDE) -O2 -Wall 
BUILD=g++ -std=c++11 -pipe $(INCLUDE) -O2 -Wall 
LD = -pthread