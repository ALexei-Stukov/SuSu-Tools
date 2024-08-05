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

//#include "susu_cache.hpp"
//#include "susu_initparam.hpp"

#define line_length_limit 4096

namespace susu_tools{

class susu_http_processer{
public:
	susu_http_processer(int fd);	//we should get the fd at first
	int get_fd();		//return the fd
				
	int get_a_line();//get a line from http request,the line must be end with \r\n
			 //if the size is too long (more than [line_length_limit] ),this function will stop reading and make sure buffer is end with \r\n.
			 //
			 //for example:
			 //	data = "1234567890\n"
			 //	buf[5];
			 //	use get_a_line(), then  buf = "12\r\n\0", data = "34567890\r\n"()
	
	void get_request_and_header();//read the http request head
	
	
	bool check_all_space(char* str); // check if all the char is space in a line
					 // all space line means the head is end,the next part is body.
	 
	void clear_head_message(); // clean all message.

	void write_something(char* buf)
	{
		write(fd,buf,sizeof(buf));
	}
	
private:
	char buffer[line_length_limit];	//the buffer to read request
		
		//susu_cache http_request_kv;	//to store all K-V on http head

		//the first line of http head = method + url + version
	char method[line_length_limit] = {0};
	char url[line_length_limit] = {0};
	char version[line_length_limit] = {0};
	
	int fd;	// file describe,we should get the fd at first.
};

}//namespace susu_tools
