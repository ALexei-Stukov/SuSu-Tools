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
#include "susu_http-reader.hpp"
#include "susu_http-writer.hpp"


#define line_length_limit 4096

namespace susu_tools{



class susu_http_processer{
public:
	susu_http_processer(int event_counts_limit,string script_path);

	// functions work with epoll
	int get_current_fd_count();

	int get_event_limit();
	
	int add_an_event(int fd);

	int remove_an_event(int fd);

	int get_epoll_result(int ms_count);

	int process_fd(int count);

	int load_script_list(const char* str);

private:
	susu_cache	script_list;	//to store the script can be used.
	susu_epoll*	epoll_manager;
	susu_http_reader	reader;
	susu_http_writer	writer;
};

int http_work(susu_http_processer* hp);

}//namespace susu_tools