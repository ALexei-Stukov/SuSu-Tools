#include "susu_epoll.hpp"
#include <errno.h>
#include <string.h>
#include <stdio.h>

namespace susu_tools{

susu_epoll::susu_epoll(int limit)
{
	//set limit
	if(limit <= 0 || limit >= MAX_EVENTS)
	{
		limit = MAX_EVENTS;
	}
	
	epoll_limit = limit;
   	epoll_count = 0;


	// The param of epoll_create() had been ignored in most of kernel.
	// But I still set a value for this funcion.
	epoll_fd = epoll_create( limit );
	
	//if init success,the epoll_fd != -1
	if(epoll_fd == -1)
	{
		fprintf(stderr,"fail in epoll init\n");
	}
	printf("I get a epoll_fd:%d\n",epoll_fd);
}
		

susu_epoll::~susu_epoll()
{
	//try to close the epoll_fd,the fd array also will be close by kernel.
	if(epoll_fd != -1)
	{
		if( 0 == close(epoll_fd) )
		{
			epoll_fd = -1;
		}
	}
}

int susu_epoll::get_current_event_count()
{
	return epoll_count;
}
int susu_epoll::get_event_limit()
{
	return epoll_limit;
}

int susu_epoll::add_an_event(int fd,int linsten_param)
{
	printf("the epoll_fd is %d\n",epoll_fd);
	if(epoll_count + 1 < epoll_limit)
	{
		struct epoll_event* event = (struct epoll_event*)malloc(sizeof(epoll_event));   //must use malloc to build a epoll_event struct

		event->events = linsten_param;	//If listem_param = EPOLLIN|EPOLLOUT|EPOLLPRI|EPOLLERR|EPOLLHUP|EPOLLET|EPOLLRDHUP|EPOLLONESHOT
										//That means this epoll_event will listen all kinds of events.
		event->data.fd = fd;
		if(epoll_ctl(epoll_fd, EPOLL_CTL_ADD, event->data.fd,event) != -1)
		{
			epoll_count++;
		}
		else
		{
			printf("errno = %d\n",errno);
			printf("Error: %s\n", strerror(errno));
			free(event);
			fprintf(stderr, "Failed to add file descriptor to epoll\n");

			return -1;
		}
		return 0;
	}
	else
	{
		fprintf(stderr, "to much socket in epoll\n");
		
		return -1;
	}
}

int susu_epoll::get_epoll_result(int ms_count)
{
	int event_count = epoll_wait(epoll_fd,EVENTS, MAX_EVENTS,ms_count);  // wait for xxx ms
	return event_count;
}

int susu_epoll::remove_an_event(int fd)
{
	printf("now I try to remove fd %d\n",fd);
	if(epoll_ctl(epoll_fd, EPOLL_CTL_DEL, fd ,NULL)  != -1)
    {
        epoll_count--;
		close(fd);
    }
	return 0;
}

int susu_epoll::get_epoll_fd()
{
	printf("now the epoll_fd is :%d\n",epoll_fd);
	return epoll_fd;
}

struct epoll_event* susu_epoll::get_enents_array()
{
	return EVENTS;
}

}//namespace susu_tools
