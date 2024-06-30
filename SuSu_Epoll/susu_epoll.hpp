#include <sys/socket.h>
#include <sys/types.h>
#include <unistd.h>

#include <thread>

#include <sys/epoll.h>

#include <iostream>
#include <string>
using std::string;

#include <unordered_set>
using std::unordered_set;

#include <mutex>
using std::mutex;

#include <queue>
using std::queue;

#include <vector>
using std::vector;
using std::iterator;

#include <unordered_map>
using std::unordered_map;

#include <functional>
using std::function;

#define MAX_EVENTS 1024*16
    //the fd limit is 16384
    //in most Linux system,the fd limit is 1024*1024
    
namespace susu_tools{

	//打开一个文件描述符-非网络
	//关闭一个文件描述符-非网络
	//注册一个文件描述符
	//移除一个文件描述符
	//对一个描述符读写	

//class susu_epoll_server{
//};
//class susu_epoll_client{
//};

class susu_epoll{
public:
        susu_epoll(int limit)
	{
    		if(limit <= 0 || limit >= MAX_EVENTS)
    		{
        		limit = MAX_EVENTS;
    		}
    		
		epoll_fd=epoll_create(limit); //尽管epoll_create的参数已经被Linux内核忽略，但这里还是设置一个较大的值
   	 	
		if(epoll_fd == -1)
    		{
        		fprintf(stderr,"fail in epoll init\n");
    		}
    		epoll_limit = limit;
    		epoll_count = 0;
	}
	
	~susu_epoll(){
    		if(epoll_fd != -1)
    		{
        		close(epoll_fd);
    		}
	}

        int get_current_event_count(){
    		return epoll_count;
	}
	int get_event_limit(){
    		return epoll_limit;
	}

        int add_a_event(int fd)  //add a fd to epoll struct.
	{
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

	template<typename T,class...Args>
        int epoll_process(T func,Args&&...args)	 //	epoll all fd and process their request
	{
		return func(args...);
	}
    private:
        int epoll_fd;		//	instance of epoll_struct
        int epoll_count;    	//	current event count
        int epoll_limit;   	//	0 <= event_limit <= MAX_EVENTS
        
	struct epoll_event EVENTS[MAX_EVENTS];   //epoll可容纳的事件队列

};

}//namespace susu_tools






		//printf("%d ready events\n", event_count);
		//逐个遍历被挑选出的事件（事件里面封装着文件描述符）。针对不同事件，作出反应
        /*
            EPOLLIN：触发该事件，表示对应的文件描述符上有可读数据。(包括对端SOCKET正常关闭)；
            EPOLLOUT：触发该事件，表示对应的文件描述符上可以写数据；
            EPOLLPRI：表示对应的文件描述符有紧急的数据可读（这里应该表示有带外数据到来）；
            EPOLLERR：表示对应的文件描述符发生错误；
            EPOLLRDHUP; 表示对等关闭
            EPOLLHUP：表示对应的文件描述符被挂断；
            EPOLLET：将EPOLL设为边缘触发(Edge Triggered)模式，这是相对于水平触发(Level Triggered)来说的。
            EPOLLONESHOT：只监听一次事件，当监听完这次事件之后，如果还需要继续监听这个socket的话，需要再次把这个socket加入到EPOLL队列里。
        */
