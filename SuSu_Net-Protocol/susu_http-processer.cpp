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

susu_http_processer::susu_http_processer(int event_counts_limit,string script_path)
{
	epoll_manager = new susu_epoll(event_counts_limit);
	load_script_list(script_path.c_str());
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
			if( numchars <= 0 )
			{
				//已关闭
            	//关闭该文件描述符并移除
				remove_an_event(client_socket);
			}
			else
			{
				if( reader.analyse(client_socket) == -1)	//获得HTTP请求信息，存储在reader内。根据不同情况，调用不同的函数，产生不同的结果。
				{
					//通过简单的检查，发现不是http包的情形
					remove_an_event(client_socket);
					continue;
				}

				/*调用逻辑：
					1.Method = GET，调用GET.lua
					2.Method = POST，调用POST.lua

					方法头的处理在httpd内进行，方法头将决定如何调用脚本
				*/
				if( reader.get_kv_store().find("Method") == SUCCESS )
				{
					//以管道形式调用脚本，并接收参数
					char ret[1024];//文件缓冲区
					FILE *fp;//文件流

					char exe_cmd[2048];//待执行的命令

					auto ins = susu_init_param::get_Init_Param_instance();	//获得httpd的启动参数管理器
					string ROOT_PATH = ins->get_value("root_path");	//获得root_path的位置

					string METHOD = *(reader.get_kv_store().get<string>("Method"));	//查看请求的Method字段
					string SCRIPT;	//待执行的脚本名称
					if(script_list.find(METHOD) == SUCCESS)	//检查该字段是否在script列表内
					{
						SCRIPT = *(reader.get_kv_store().get<string>("Method"))+".lua";	//本httpd默认使用lua脚本，由于httpd只负责转发操作，所以修改起来是很方便的。
					}
					else
					{
						SCRIPT = *(reader.get_kv_store().get<string>("undefined"))+".lua";
					}
					SCRIPT = ROOT_PATH + "/" + SCRIPT;
					//std::cout<<"the SCRIPT is"<<SCRIPT<<std::endl;

					string URL = *(reader.get_kv_store().get<string>("Url"));	//看看客户端想要的文件名
					if (URL == "/")
					{
						URL = "/index.html";
					}
					URL = ROOT_PATH + URL;
					//std::cout<<"the UTL is"<<URL<<std::endl;
					
					string ACCEPT =*(reader.get_kv_store().get<string>("Accept"));	//看看客户端接受哪些文件类型
					
					sprintf(exe_cmd,"lua %s %s %s\0",SCRIPT.c_str(),URL.c_str(),ACCEPT.c_str());	//调用对应的script
					//printf("%s\n",exe_cmd);

					fp = popen(exe_cmd,"r");
					int size = fread(ret,1,1024,fp);
					while(size > 0)
    				{
						writer.write_fd(client_socket,ret,size);//把结果发送给调用方
        				//printf("%s",ret);
        				size = fread(ret,1,1024,fp);
    				}
   					pclose(fp);							
				}
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

int susu_http_processer::load_script_list(const char* str)  //加载参数
{
	try{
		//open the target file
		FILE* init_file = fopen(str,"r");
		char key[128] = {0};
		char value[1024] = {0};
		char line[1280] = {0};

		if(init_file != NULL)
		{
			memset(key,128,0);
			memset(value,1024,0);
			memset(line,1280,0);
			while(fscanf(init_file,"%s\n",line) != EOF)
			{
				int i;
				int check = 1;
				for(i=0;i<strlen(line);i++)
				{
					if(line[i] == '=' && i <= 128)
					{
						strncpy(key,line,i);
						strncpy(value,line+i+1,strlen(line)-i-1);
						check = 0;
						break;
					}
					else
					{
						check = 1;
					}
				}
				if(check)
				{continue;}
				string KEY = key;
				string VALUE = value;
		        script_list.add(KEY,VALUE);

				memset(key,0,128);
				memset(value,0,1024);
				memset(line,0,1280);
			}
		}
		else
		{
			throw "script config is not exist";//throw error
		}
			   
	}
	catch(const char* error_string)
	{
		perror(error_string);
		exit(-1);
	}
	return 0;
}