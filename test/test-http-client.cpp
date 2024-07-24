#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/socket.h>
#include <netinet/tcp.h>

#include <arpa/inet.h>
#include <sys/termios.h>

#include <iostream>
using std::cout;
using std::endl;

#include <string.h>
#include <vector>
using std::vector;

#include "susu_http.hpp"
#include "susu_socket.hpp"
using namespace susu_tools;

#define buffersize 1024*1024*128

int main(int argc, char *argv[]) {
	int fd = conect_to_url("127.0.0.1",9527);

	vector<string> message;
	message.push_back( "123456789\r\n" );
	message.push_back( "777  092\r\n" );
	message.push_back( "It\'s been a long day,without you my friend.\r\n" );

	string sum = "";
	for(auto it = message.begin();it<message.end();it++)
	{
		sum += *it;
	}
	
	printf("the post is %d:%s",strlen(sum.c_str()),sum.c_str());
    
	if (send(fd,sum.c_str(), strlen(sum.c_str()), 0) < 0) {
        	printf("Send failed\n");
        	//return -1;
    	}
    //recv(fd,buffer, strlen(buffer), 0);
	        //本次包有多少个字节
    return 0;
}

