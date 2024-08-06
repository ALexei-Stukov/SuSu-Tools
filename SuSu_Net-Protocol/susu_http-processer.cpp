#include "susu_http-processer.hpp"
#include <iostream>

using namespace susu_tools;

int susu_tools::http_work_loop(susu_http_processer* hp)
{
	return 0;
}

susu_http_processer::susu_http_processer(int event_counts_limit)
{
	epoll_manager = susu_epoll(event_counts_limit);
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

int susu_http_processer::get_epoll_result(int ms_count)
{
	return epoll_manager.get_epoll_result(ms_count);	
}
