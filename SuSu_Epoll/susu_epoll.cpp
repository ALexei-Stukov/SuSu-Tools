#include "susu_epoll.hpp"

namespace susu_tools{

susu_epoll::susu_epoll(int limit){

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
}
    	

susu_epoll::~susu_epoll(){
	//try to close the epoll_fd,the fd array also will be close by kernel.
	if(epoll_fd != -1)
	{
		if( 0 == close(epoll_fd) )
		{
			epoll_fd = -1;
		}
	}
}

int susu_epoll::get_current_event_count(){
	return epoll_count;
}
int susu_epoll::get_event_limit(){
	return epoll_limit;
}

int susu_epoll::add_a_event(int fd){ //这个函数由上层调用。susu_epoll对象一般不主动调用这个函数
	if(epoll_count + 1 < epoll_limit)
	{
        struct epoll_event* event = (struct epoll_event*)malloc(sizeof(epoll_event));   //此处不能使用临时对象，需要创建一个指针
	   	
	    event->events = EPOLLIN|EPOLLERR|EPOLLHUP|EPOLLET|EPOLLRDHUP;
	    event->data.fd = fd;
        if(epoll_ctl(epoll_fd, EPOLL_CTL_ADD, event->data.fd,event) != -1)
	    {
        	epoll_count++;
	    }
        else
        {
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

int susu_epoll::get_epoll_result(int ms_count){
	int event_count = epoll_wait(epoll_fd,EVENTS, MAX_EVENTS,ms_count);  // wait for xxx ms
    return event_count;
}

}//namespace susu_tools
