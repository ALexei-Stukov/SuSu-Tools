#include <sys/socket.h>
#include <netinet/in.h>



#include <iostream>
using std::cout;
using std::endl;

#include <string.h>

#include "susu_http.hpp"
#include "susu_socket.hpp"

using namespace susu_tools;

int startup();  //开始监听

int main(int argc,char* argv[])
{
    //2.建立并监听socket
    int server_sock = -1;    //待监听的socket
    int client_sock = -1;   //客户端
    
    //sockaddr_in 是 IPV4的套接字地址结构。定义在<netinet/in.h>,参读《TLPI》P1202
    struct sockaddr_in client_name;
    socklen_t client_name_len = sizeof(client_name);

    server_sock = begin_to_listen(9527);
	    //startup(); //因为内部有异常处理，所以无需在此处检查返回值

    int ret=0;

    client_sock = accept(server_sock,
            (struct sockaddr *)&client_name,
            &client_name_len);
    if (client_sock != -1)
    {
    	susu_http_object test(client_sock);

		cout<<"i had do it successful\n";
		
        cout<<test.get_a_line()<<endl;
        cout<<test.get_a_line()<<endl;
        cout<<test.get_a_line()<<endl;

		test.write_something("WDNMD");
    }

    close(server_sock);
    return 0;
}
/*
int startup()   //开始监听
{
    int httpd = 0;
    try
    {
        //sockaddr_in 是 IPV4的套接字地址结构。定义在<netinet/in.h>,参读《TLPI》P1202
        struct sockaddr_in name;
    
        //socket()用于创建一个用于 socket 的描述符，函数包含于<sys/socket.h>。参读《TLPI》P1153
        //这里的PF_INET其实是与 AF_INET同义，具体可以参读《TLPI》P946
        httpd = socket(PF_INET, SOCK_STREAM, 0);
        if (httpd == -1)
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
        if (setsockopt(httpd, SOL_SOCKET, SO_REUSEADDR | SO_REUSEPORT, &opt, sizeof(opt)) < 0) {
            perror("setsockopt");
            exit(EXIT_FAILURE);
        }
        
        //bind()用于绑定地址与 socket。参读《TLPI》P1153
        //如果传进去的sockaddr结构中的 sin_port 指定为0，这时系统会选择一个临时的端口号
        if (bind(httpd, (struct sockaddr *)&name, sizeof(name)) < 0)
        {
            throw "socket bind failed";
        }

        //最初的 BSD socket 实现中，backlog 的上限是5.参读《TLPI》P1156
        //baclkog实际上是tcp的 establish队列的长度，根据经验，若想和其它服务器组件配合得比较好，可以设置为512.这里的其它服务器组件主要是nignx等
        if (listen(httpd, 512) < 0)
        {
            throw "socket listen failed";
        }

    }
    catch(const char* error_string)
    {
        exit(-1);
    }
    return httpd;
}*/
