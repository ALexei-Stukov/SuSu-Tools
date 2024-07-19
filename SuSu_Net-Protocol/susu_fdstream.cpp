#include "susu_fdstream.hpp"
#include <malloc.h>

using namespace susu_tools;


susu_fdstream::susu_fdstream(int fd)
{
	this->fd = fd;
}

int susu_fdstream::get_fd()
{
	return fd;
}

int susu_fdstream::fd_write(const char * buffer,size_t n) {
	if(fd>0)
	{
		return write(fd,buffer,n);
	}
	else
	{
		return -1;
	}
}


int susu_fdstream::fd_read(char * buffer,size_t n) {
	if(fd>0)
	{
		return read(fd,buffer,n);
	}
	else
	{
		return -1;
	}
} 

char* susu_fdstream::get_buffer()
{
	return buf;
}
