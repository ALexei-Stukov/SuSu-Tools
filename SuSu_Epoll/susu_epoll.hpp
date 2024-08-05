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

class susu_epoll{
public:
	susu_epoll(int limit = MAX_EVENTS);	//limit is a useless param in new Linux Kernel.
	~susu_epoll();

	int get_current_event_count();	//get the current fd counts.
	int get_event_limit();			//The MAX fd count can be listened by a SuSu_Epoll object.(event is created by fd).

	int add_an_event(int fd,int linsten_param);		//add a fd to epoll struct.
				  									//in most of times,this function will be call by other codes.
													//If listem_param = EPOLLIN|EPOLLOUT|EPOLLPRI|EPOLLERR|EPOLLHUP|EPOLLET|EPOLLRDHUP|EPOLLONESHOT
													//That means this epoll_event will listen all kinds of events.

	int get_epoll_result(int ms_count);	//get some event from fd,and store the event in array EVENTS;
										//if ms_count = -1, keep waiting

	struct epoll_event* get_enents_array();		//get the array of EVENTS
private:
	int epoll_fd;		//	instance of epoll_struct
	int epoll_count;	//	current event count
	int epoll_limit;   	//	0 <= event_limit <= MAX_EVENTS
		
	struct epoll_event EVENTS[MAX_EVENTS];   // array to store all fd.

};

}//namespace susu_tools

//summary of epoll events:
/*
	EPOLLIN：触发该事件，表示对应的文件描述符上有可读数据。(包括对端SOCKET正常关闭)；
	EPOLLOUT：触发该事件，表示对应的文件描述符上可以写数据；
	EPOLLPRI：表示对应的文件描述符出现了一些紧急状况，在TCP连接中，可能指的是带外数据（比较复杂，需要参考一些资料）；
	EPOLLERR：表示对应的文件描述符发生错误；
	EPOLLRDHUP; 表示对等关闭
	EPOLLHUP：表示对应的文件描述符被挂断；
	EPOLLET：将EPOLL设为边缘触发(Edge Triggered)模式，这是相对于水平触发(Level Triggered)来说的。
	EPOLLONESHOT：只监听一次事件，当监听完这次事件之后，如果还需要继续监听这个socket的话，需要再次把这个socket加入到EPOLL队列里。
*/
