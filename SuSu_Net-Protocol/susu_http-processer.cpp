#include "susu_http-processer.hpp"
#include <iostream>

using namespace susu_tools;



//don't write a loop in this function
int susu_tools::http_work(susu_http_processer* hp)
{
	int count = hp->get_epoll_result(100);
	if(count > 0)
	{
		hp->get_fd_queue(count);
		hp->process_fd_queue();
	}
	return 0;
}

susu_http_processer::susu_http_processer(int event_counts_limit)
{
	epoll_manager = susu_epoll(event_counts_limit);
}


int susu_http_processer::get_fd_queue(int count)
{
	for (int i = 0; i < count; i++)
	{
		auto array = epoll_manager.get_enents_array();
		int epoll_fd = epoll_manager.get_epoll_fd();
		int client_socket = array[i].data.fd;

        //发生错误或中断
        if((array[i].events & EPOLLPRI) || (array[i].events & EPOLLERR) || (array[i].events & EPOLLRDHUP))
        {
            //关闭该文件描述符并移除
			remove_an_event(client_socket);
        }
        else if( (array[i].events & EPOLLIN) )
        {
            //如果是正常的可读事件，则把它插入到fd队列中
			fd_queue.push(client_socket);
        }
        else
        {
            //printf("nothing happened\n");
        }
	}
}
int susu_http_processer::process_fd_queue()
{
	while(fd_queue.size() > 0)
	{
		int fd = fd_queue.front();
		fd_queue.pop();

		
	}
	return 0;
}


int susu_http_processer::get_current_fd_count()
{
	return epoll_manager.get_current_event_count();
}

int susu_http_processer::get_event_limit()
{
	return epoll_manager.get_event_limit();	
}
	
int susu_http_processer::add_an_event(int fd)
{
	return epoll_manager.add_an_event(fd, EPOLLIN|EPOLLOUT|EPOLLPRI|EPOLLERR|EPOLLHUP|EPOLLET|EPOLLRDHUP|EPOLLONESHOT);
}

int susu_http_processer::remove_an_event(int fd)
{
	return epoll_manager.remove_an_event(fd);
}

int susu_http_processer::get_epoll_result(int ms_count)
{
	return epoll_manager.get_epoll_result(ms_count);	
}
