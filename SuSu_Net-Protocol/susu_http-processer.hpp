#include <stdio.h>
#include <string.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>

#include <ctype.h>

#include <sys/socket.h>

#include <iostream>
using std::string;

#include "susu_epoll.hpp"
#include "susu_http-analyser.hpp"



#define line_length_limit 4096

namespace susu_tools{



class susu_http_processer{
public:
	susu_http_processer(int event_counts_limit);

	// functions work with epoll
	int get_current_fd_count();

	int get_event_limit();
	
	int add_an_event(int fd);

	int get_epoll_result(int ms_count);

	// functions work with http-analyser

private:
	susu_epoll	epoll_manager;
	//susu_http-ayalyser	request_analyser;
};

int http_work_loop(susu_http_processer* hp);

}//namespace susu_tools