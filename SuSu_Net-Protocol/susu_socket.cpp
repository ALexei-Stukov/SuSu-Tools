#include "susu_socket.hpp"

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

#include "susu_http.hpp"
using namespace susu_tools;

#define buffersize 1024*1024*128

//try to listen a port and return a socket_fd.
//if listen failed, print error message and return -1;
int begin_to_listen(int port)   //开始监听
{
	int socket_fd = 0;
	try
	{
        	//sockaddr_in is IPV4 Socket address structure ,from <netinet/in.h> ,refer to《TLPI》P1202
        	struct sockaddr_in name;

        	//socket()用于创建一个用于 socket 的描述符，函数包含于<sys/socket.h>。参读《TLPI》P1153
        	//这里的PF_INET其实是与 AF_INET同义，具体可以参读《TLPI》P946
        	socket_fd = socket(PF_INET, SOCK_STREAM, 0);
        	if (socket_fd == -1)
        	{
           		throw "socket init failed";
        	}

        	memset(&name, 0, sizeof(name));
        	name.sin_family = AF_INET;
        	
		//htons()，ntohs() 和 htonl()包含于<arpa/inet.h>, 参读《TLPI》P1199
        	//将Init_Param.port 转换成以网络字节序表示的16位整数
        	name.sin_port = htons(9527);
        
		//INADDR_ANY是一个 IPV4通配地址的常量，包含于<netinet/in.h>
        	//大多实现都将其定义成了0.0.0.0 参读《TLPI》P1187
        	name.sin_addr.s_addr = htonl(INADDR_ANY);
        
        	//设置端口复用，避免意外中断后无法快速启动
        	int opt = 1;
        	if (setsockopt(socket_fd, SOL_SOCKET, SO_REUSEADDR | SO_REUSEPORT, &opt, sizeof(opt)) < 0) {
            		throw "setsockopt failed";
        	}
        
        	//bind()用于绑定地址与 socket。参读《TLPI》P1153
        	//如果传进去的sockaddr结构中的 sin_port 指定为0，这时系统会选择一个临时的端口号
        	if (bind(socket_fd, (struct sockaddr *)&name, sizeof(name)) < 0)
        	{
            		throw "socket bind failed";
        	}

        	//最初的 BSD socket 实现中，backlog 的上限是5.参读《TLPI》P1156
        	//baclkog实际上是tcp的 establish队列的长度，根据经验，若想和其它服务器组件配合得比较好，可以设置为512.这里的其它服务器组件主要是nignx等
        	if (listen(socket_fd, 512) < 0)
        	{
            		throw "socket listen failed";
        	}

    	}
   	catch(const char* error_string)
    	{
		perror(error_string);
        	return -1;
    	}

    	return socket_fd;
}

//try to connect to a URL and return a socket_fd
int conect_to_url(const char* url,int port)
{
        struct sockaddr_in server_addr;
	// 创建socket
        int sock_fd = socket(AF_INET, SOCK_STREAM, 0);
        if (sock_fd == -1) {
		perror("Could not create socket\n");
                return -1;
        }
 
        // 初始化服务器地址
        server_addr.sin_family = AF_INET;
        server_addr.sin_port = htons(port);
        inet_pton(AF_INET, url , &server_addr.sin_addr);
 
                 // 连接到服务器
                 if (connect(sock_fd, (struct sockaddr*)&server_addr, sizeof(server_addr)) < 0) {
                     perror("Connection failed\n");
                     return -1;
                 }
                     //设置收发超时时间
                     struct timeval timeout;
                     timeout.tv_sec = 10; // 设置超时时间为10秒
                     timeout.tv_usec = 0;
                     if (setsockopt(sock_fd, SOL_SOCKET, SO_RCVTIMEO, &timeout, sizeof(timeout)) == -1) {
                     	perror("Setsockopt failed");
                     	return -1;
                     }
    
    int keepalive = 1; // 开启keepalive属性
    int keepidle = 60; // 如果60秒内没有任何数据交换，则开始发送keepalive探测
    int keepinterval = 60; // 如果开始发送探测，则每隔60秒发送一次
    int keepcount = 5; // 如果5次探测都失败，则认为连接已断并关闭连接
 
    if (setsockopt(sock_fd, SOL_SOCKET, SO_KEEPALIVE, &keepalive, sizeof(keepalive)) == -1) {
                     	perror("Setsockopt failed");
			return -1;
    }
 
    if (setsockopt(sock_fd, SOL_TCP, TCP_KEEPIDLE, &keepidle, sizeof(keepidle)) == -1) {
                     	perror("Setsockopt failed");
			return -1;
    }
 
    if (setsockopt(sock_fd, SOL_TCP, TCP_KEEPINTVL, &keepinterval, sizeof(keepinterval)) == -1) {
                     	perror("Setsockopt failed");
			return -1;
    }
 
 
    if (setsockopt(sock_fd, SOL_TCP, TCP_KEEPCNT, &keepcount, sizeof(keepcount)) == -1) {
                     	perror("Setsockopt failed");
    }
    return sock_fd;
}

