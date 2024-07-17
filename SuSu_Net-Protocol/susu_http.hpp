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

//#include "../SUSU_LogPrinter/susu_log_printer"

#include "susu_cache.hpp"
#include "susu_initparam.hpp"

//#define SERVER_STRING "Server: susu_httpd\r\n"

#define line_length_limit 1024

namespace susu_tools{

class susu_http_object{
public:
    void get_request_and_header(int fd);//read the http request head
					
    int get_a_line(int sock, char *buf, int size);//get a line from http request,the line must be end with \r\n
    
    bool check_all_space(char* str); // check if all the char is space in a line
     
    void clear_head_body(); // clean all message.
			    
private:
    char buffer[line_length_limit];	//the buffer to read request
    
    string method;	//what method about the request,POST? GET?
    string url;		
    string version;	// http version
    
    susu_cache http_request_kv;
    /*
    	char method[line_limit] = {0};
    	char url[line_limit] = {0};
    	char version[line_limit] = {0};
    */
    //unordered_map<string,string> http_head_kv;
};

}//namespace susu_tools
