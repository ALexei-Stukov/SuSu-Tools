#include <stdio.h>
#include <string.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>

#include <ctype.h>

#include <sys/socket.h>

#include <iostream>
#include <queue>
using std::string;
using std::queue;

#include "susu_cache.hpp"
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

	int remove_an_event(int fd);

	int get_epoll_result(int ms_count);

	int process_fd(int count);

	int http_read();
	int http_write();

private:
	susu_epoll	epoll_manager;
	susu_http_analyser	request_analyser;	//其实这个名称还是有待商榷，这是记录http头的东西。
	susu_cache	process_script;
};

int http_work(susu_http_processer* hp);

}//namespace susu_tools