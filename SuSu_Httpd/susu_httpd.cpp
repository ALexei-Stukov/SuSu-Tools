#include <sys/socket.h>
#include <netinet/in.h>
#include <unistd.h>

#include "susu_init-param.hpp"
#include "susu_tcp-object.hpp"
#include "susu_http-processer.hpp"
#include "susu_thread-pool.hpp"

#include <iostream>
#include <vector>
using namespace susu_tools;
using namespace std;

int main(int argc,char** argv)
{
    if(argc < 2 )
    {
            printf("too less param\n");
            return 0;
    }

    //get the params
    auto ins = susu_init_param::get_Init_Param_instance();

    cout<<"load param file:"<<argv[1]<<endl;
    ins->load_init_param(argv[1]);
    cout<<endl;



	//set the socket
	int server_socket = -1;// fd;
	int port = stoi(ins->get_value("port"));

	susu_tcp_object* server = new susu_tcp_object(port);
	server->server_listen_the_port();

	if( server->get_fd() <= 0 )
	{
		cout<<"listen port "<<port<<" failed.\n";
		return -1;
	}

	//set the threads    
	int thread_count = stoi(ins->get_value("thread_init_count"));
	susu_thread_pool::set_init_count(thread_count);
	susu_thread_pool* tp = susu_thread_pool::get_susu_thread_pool();
	tp->set_dynamic_param(stoi(ins->get_value("thread_expansion_param")));//设置线程扩缩容参数，若为0，则不扩容


	vector<susu_http_processer*> http_processer_vector;
    for(int i=0;i<stoi(ins->get_value("thread_init_count"));i++)
    {
        //新建一个epoll对象,每个对象最多管理 Init_Param.epoll_count 个socket
        susu_http_processer* hp = new susu_http_processer(stoi(ins->get_value("epoll_count")),ins->get_value("script_config"));
        //把对象的loop函数作为可执行的task添加到线程池
        tp->add_task(susu_tools::http_work,hp);
        //由于load_balace的设计，每个task都独占1个线程

        //把epoll对象添加到vector内，方便管理
        http_processer_vector.push_back(hp);
    }

	struct sockaddr_in client_addr;
    socklen_t client_addr_size = sizeof(client_addr);
	int client_socket = -1;

	int target_index = 0;	//即将要添加fd的processer编号
	int min_fd_counts = 1024*1024*1024;	//最空闲的http-process有多少个fd
	while(true)
	{
    	client_socket = accept(server->get_fd(),(struct sockaddr *)&client_addr,&client_addr_size);
		printf("received a client socket:%d\n",client_socket);
    	if (client_socket != -1)
    	{
			//主线程将把fd分给 "最空闲"的 http-processer。由于processer内部有一个循环，且一个processer和一个线程绑定，所以最终，也可以认为主线程把fd交给了最空闲、算力最充裕的线程。
			http_processer_vector[target_index]->add_an_event(client_socket);
			for(int index = 0;index < http_processer_vector.size();index++)
			{
				if(http_processer_vector[index]->get_current_fd_count() < min_fd_counts)
				{
					min_fd_counts = http_processer_vector[index]->get_current_fd_count();
					target_index = index;
				}
			}
    	}
	}
	
	close(server_socket);
    return 0;
}
