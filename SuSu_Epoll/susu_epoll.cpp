#include "susu_epoll_object"
#include "../SUSU_HttpObject/susu_http_object"
#include "../SUSU_LogPrinter/susu_log_printer"

namespace susu_tools{
epoll_object::epoll_object(int limit){
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
epoll_object::~epoll_object(){
    if(epoll_fd != -1)
    {
        close(epoll_fd);
    }
}

int epoll_object::get_current_event_count(){
    return epoll_count;
}
int epoll_object::get_event_limit(){
    return epoll_limit;
}

int epoll_object::add_a_event(int fd){ //这个函数由上层调用。epoll_object对象一般不主动调用这个函数
    Log_Printer* log = Log_Printer::get_Log_Printer();
    string temp_log_str;
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

            temp_log_str = std::move("Failed to add file descriptor to epoll,epoll pointer is" + std::to_string(gettid()));
            log->print_a_line(3,temp_log_str);
            return -1;
        }
        return 0;
    }
    else
    {
        fprintf(stderr, "to much socket in epoll\n");
        
        temp_log_str = std::move("to much socket in epoll,epoll pointer is" + std::to_string(gettid()));
        log->print_a_line(3,temp_log_str);


        return -1;
    }
}

void epoll_object::epoll_process(){   //持续对管理的文件描述符fd进行epoll,并处理它们的请求。
    Log_Printer* log = Log_Printer::get_Log_Printer();
    string temp_log_str;
    susu_http_object temp;
    while(true)
    {
        //printf("i am waiting %p\n",this);
        //printf("while is running\n");

        //对event进行监听
        int event_count = epoll_wait(epoll_fd,EVENTS, MAX_EVENTS,100);  //100毫秒
        /*if(event_count == 0)
        {
            ::std::this_thread::sleep_for(::std::chrono::milliseconds(1000));   //没有任何事件则休眠1秒
        }*/
        for (int i = 0; i < event_count; i++) {

            int client_socket = EVENTS[i].data.fd;
                //发生错误或中断
                if((EVENTS[i].events & EPOLLERR) || (EVENTS[i].events & EPOLLERR) || (EVENTS[i].events & EPOLLRDHUP))
                {
                    //关闭该文件描述符并移除
                    if(epoll_ctl(epoll_fd, EPOLL_CTL_DEL, client_socket,NULL)  != -1)
	                {
                        epoll_count--;
                        close(client_socket);
                        temp_log_str = std::move("remove and close a socket" + std::to_string(client_socket));
                        log->print_a_line(3,temp_log_str); 
	                }
                    else
                    {
                        fprintf(stderr,"error in remove socket %d\n",client_socket);
                        temp_log_str = std::move("error in remove socket " + std::to_string(client_socket));
                        log->print_a_line(3,temp_log_str);
                    }
                }
                else if( (EVENTS[i].events & EPOLLIN) )
                {
                    //printf("now i get a event:%p\n",this);
                    //如果是正常的可读事件，则把它交给某个task
                    //printf("need to read something in %d\n",client_socket);
                    temp.get_request_and_header(client_socket);
                    temp.process(client_socket);
                    temp.clear_head_body();
                }
                else
                {
                    //printf("nothing happened\n");
                }
		}
        //::std::this_thread::sleep_for(::std::chrono::milliseconds(5));
    }
    return ;
}

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
}