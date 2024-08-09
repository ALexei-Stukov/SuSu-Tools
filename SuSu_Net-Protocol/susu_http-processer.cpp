#include "susu_http-processer.hpp"
#include <iostream>

using namespace susu_tools;



//don't write a loop in this function
int susu_tools::http_work(susu_http_processer* hp)
{
	//std::cout<<"do it"<<std::endl;
	while(true)
	{
		int count = hp->get_epoll_result(100);
		if(count > 0)
		{
			hp->process_fd(count);
		}
		//sleep(1);
	}
	return 0;
}

susu_http_processer::susu_http_processer(int event_counts_limit)
{
	epoll_manager = new susu_epoll(event_counts_limit);
}

int susu_http_processer::process_fd(int count)
{
	for (int i = 0; i < count; i++)
	{
		auto array = epoll_manager->get_enents_array();

		int epoll_fd = epoll_manager->get_epoll_fd();
		int client_socket = array[i].data.fd;

		printf("receive an fd:%d\n",client_socket);
		printf("receive an event:%d\n",array[i].events);
        //发生错误或中断
        if((array[i].events & EPOLLERR) || (array[i].events & EPOLLRDHUP) || (array[i].events & EPOLLHUP) )
        {
            //关闭该文件描述符并移除
			remove_an_event(client_socket);
        }
        else if( array[i].events & EPOLLIN )
        {
			//正常关闭也会触发EPOLLIN，要做一下区分
			char ch;
			int numchars = recv(client_socket, &ch, 1, MSG_PEEK);//试着读一下
			if( numchars < 0 )
			{
				//已关闭
            	//关闭该文件描述符并移除
				remove_an_event(client_socket);
			}
			else
			{
				reader.analyse(client_socket);	//获得HTTP请求信息，存储在reader内。
					//根据不同情况，调用不同的函数，产生不同的结果。
				char* temp = "WDNMD, I had received your request!";
				writer.write_fd(client_socket,temp,strlen(temp));//把结果发送给调用方
			}
        }
        else
        {
            //printf("nothing happened\n");
        }
	}
	return 0;
}

int susu_http_processer::get_current_fd_count()
{
	return epoll_manager->get_current_event_count();
}

int susu_http_processer::get_event_limit()
{
	return epoll_manager->get_event_limit();	
}
	
int susu_http_processer::add_an_event(int fd)
{
	return epoll_manager->add_an_event(fd, EPOLLIN);
}

int susu_http_processer::remove_an_event(int fd)
{
	return epoll_manager->remove_an_event(fd);
}

int susu_http_processer::get_epoll_result(int ms_count)
{
	return epoll_manager->get_epoll_result(ms_count);	
}
