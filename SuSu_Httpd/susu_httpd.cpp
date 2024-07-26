#include <sys/socket.h>
#include <netinet/in.h>
#include <unistd.h>

#include "susu_initparam.hpp"
#include "susu_socket.hpp"
#include "susu_epoll.hpp"


#include <iostream>
#include <vector>
using namespace susu_tools;
using namespace std;

int main(int argc,char** argv)
{
        if(argc <= 1 )
        {
                printf("too less param\n");
                return 0;
        }
        auto ins = susu_initparam::get_Init_Param_instance();

        cout<<"load param file:"<<argv[1]<<endl;
        ins->load_init_param(argv[1]);
        cout<<endl;


	int server_socket = -1;// fd;
	int port = stoi(ins->get_value("port"));

	server_socket = begin_to_listen(port);
	if( server_socket <= 0 )
	{
		cout<<"listen port "<<port<<" failed.\n";
		return -1;
	}
	
	susu_epoll epoll_object;
	
	

	struct sockaddr_in client_addr;
    	socklen_t client_addr_size = sizeof(client_addr);
	int client_socket = -1;
	while(true)
	{
    		client_socket = accept(server_socket,
            			(struct sockaddr *)&client_addr,
            			&client_addr_size);

    		if (client_socket != -1)
    		{
			epoll_object.add_a_event(client_socket);
			//add new connect to the epoll control object
			
			//主线程接收外部链接。将产生的fd交给子线程。（不是以fork复制进程的形式来实现并发。）

			//每个线程都有一个epoll对象。主线程将把fd分给 最空闲的 epoll对象。
    		}
	}
	
	close(server_socket);
        return 0;
}
