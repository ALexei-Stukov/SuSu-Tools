#include <streambuf>
#include <iostream>
#include <unistd.h>

namespace susu_tools{

#define stream_buf_size 8192

class susu_fdstream{
public:
        susu_fdstream(int fd);	// provide a fd to construct a project, in this situation, the file_pointer is nullptr
	
	int get_fd();		//return the fd
				//due to security issues, we can't reset the fd.

	int fd_write(const char* buffer,size_t n);
        int fd_read(char* buffer,size_t n);

	char* get_buffer();
private:
        int fd;
	char buf[stream_buf_size];
};

}//namespace susu_tools


