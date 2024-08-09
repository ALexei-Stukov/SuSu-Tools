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
using std::string;

#include <string.h>
#include <vector>
using std::vector;

#include "susu_tcp-object.hpp"

using namespace susu_tools;

#define buffersize 1024

int main(int argc, char *argv[]) {
	
	susu_tcp_object tcp(9527);

	int fd = tcp.client_conect_to_url("127.0.0.1");

	vector<string> message;
	message.push_back( "wdnmd " );
	message.push_back( "zbgyd " );
	message.push_back( "CAO\r\n" );
	message.push_back( "  \r\n" );

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
	getchar();
	getchar();
    //recv(fd,buffer, strlen(buffer), 0);
	//本次包有多少个字节
    return 0;
}

