#include "susu_http-writer.hpp"
#include <iostream>

using namespace susu_tools;
using std::cout;
using std::endl;

int susu_http_writer::write_fd(int fd,char* buffer,size_t count)	//封装一下send函数
{
	send(fd,buffer,count,0);
	return 0;
}