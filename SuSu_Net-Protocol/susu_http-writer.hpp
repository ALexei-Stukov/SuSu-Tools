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

#include "susu_cache.hpp"
//#include "susu_initparam.hpp"

#define line_length_limit 4096

namespace susu_tools{

class susu_http_writer{
public:
	int write_fd(int fd,char* buffer,size_t count);	//封装一下send函数
private:	
};

}//namespace susu_tools