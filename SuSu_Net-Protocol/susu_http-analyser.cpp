#include "susu_http-analyser.hpp"
#include <iostream>

using namespace susu_tools;
/*
* 关于该http组件的 硬性设计
* 
* 由于POST 和 GET 一般被 设计 用于 上传 和 下载 内容，但实际应用上，服务器要如何对待 POST 和 GET 请求，完全看生产实际，或者编码者的心情。
* 
* 也就是说，如果前端 执意 要把所有请求都用GET发送，那么服务端在解析GET请求时必须要能根据情况调用脚本，而不仅仅是返回文本或二进制数据。同样的情况也适用于POST。
*
* 因此，我个人在此作出规定，在本http组件内：
*
* 凡是GET请求，其body都会被读取并被忽略。所有参数必须在url内提供。获得静态文本文件或图片则必须用get。凡是需要调用脚本后才能获取的信息，一律用POST。
*
* 凡是POST请求，所有参数必须在body内提供，且请求内的url参数也会被忽略。且，url的指向必须是一个脚本名称，服务端会补充后缀为.lua或.php。
*
* 对于header内的大量参数，head-kv内的参数，除了少数常见参数之外，绝大部分参数都会被忽略。
*
* 在这个设计之下：
* 若想请求文件，如html,css,js,则必须用GET请求。
* 若想发送某些数据，或者请求的数据需要通过脚本来获取，比如查询数据库显然适合用脚本来做，则必须使用POST。
* 
* 该组件默认只支持Lua脚本，但经过简单的修改后就可以支持php脚本
*/


susu_http_analyser::susu_http_analyser(int fd)
{
	this->fd = fd;
}

int susu_http_analyser::get_fd()
{
	return fd;
}

/*//used by other function
int susu_http_analyser::fd_write(const char * buffer,size_t n)
{
	if(fd>0)
	{
		return write(fd,buffer,n);
	}
	else
	{
		return -1;
	}
}

//used by other function
int susu_http_analyser::fd_read(char * buffer,size_t n)
{
	if(fd>0)
	{

		return read(fd,buffer,n);
	}
	else
	{
		return -1;
	}
}*/

void susu_http_analyser::get_request_and_header()
{
	int numchars = get_a_line(); //get the head of http request in buffer
		
	//check the space count
		int space_count=0;
		for(int i=0;i<numchars;i++)
		{
			if(buffer[i] == ' ')
			{
					space_count++;
			}
		}

	//the first line of normal http request Must have 2 space.
		if(space_count != 2)
		{
			return;
		}


		//get the method,url,version from buffer
	//char[line_length_limit] METHOD;
	//char[line_length_limit] URL;
	//char[line_length_limit] VERSION;

   	sscanf(buffer,"%[^ ] %[^ ] %[^'\n']",method,url,version);
		
	//method = METHOD;
		//url = URL;
		//version = version;
	printf("%s\n %s\n %s\n",method,url,version);

		while( !check_all_space(buffer) )
		{
			numchars = get_a_line();

			string line = buffer;

			size_t pos = line.find(": "); // pos == 7
						  //
			if(pos == string::npos)	//忽略不带等号的行
			{
					continue;
			}

			//拆解其中的信息并插入到键值对表中
			string key,value;
			key = line.substr(0,pos);
			value = line.substr(pos+2,line.length());

			//http_head_kv.insert(pair<string,string>(key,value));
		}
}

int susu_http_analyser::get_a_line()
{
	int length = 0;
	char ch = '\0';
	int numchars;
	while ( length < line_length_limit-2 && ch != '\n'  )
	{
		numchars = recv(fd, &ch, 1, 0);
		if (numchars > 0)
		{
			buffer[length] = ch;
			length++;

			if (ch == '\r')  //check if the line is end 
			{
				numchars = recv(fd, &ch, 1, MSG_PEEK);	// use MSG_PEEK to check what is the next char in fd, this operation will not affect the data in fd.
								   // if the next char is '\n',then we read it ,and the line is end.
								   // if the next char not '\n',do nothing.
				
				if ((numchars > 0) && (ch == '\n'))
				{
					printf("Now I get this char");
					recv(fd, &ch, 1, 0);
					buffer[length] = ch;
					length++;
					break;
				}
			}
		}
		else
		{
			break;
		}
	}

	//make sure the line is end with "\r\n"
	if(buffer[length-2] != '\r' && buffer[length-1] != '\n')
	{
		buffer[length] = '\r';
		length++;
		buffer[length] = '\n';
		length++;
	}

	buffer[length] = '\0';

	printf("[%s]\n",buffer);
   	return length;
}


void susu_http_analyser::clear_head_message()
{
	method[0] = '\0';
	url[0] = '\0';
	version[0] = '\0';
}

bool susu_http_analyser::check_all_space(char* str) //check if all the char is space
{
	bool ret = true;
	for(int i=0;i<strlen(str);i++)
	{
		if( !isspace(str[i]) )
		{
			return false;
		}
	}
	return ret;
}

/*
bool susu_http_analyser::str_end_with(char* input,char* end)  //检查某个字符串A 是否以 某个字符串B结尾
{
	int i = strlen(input);
	int j = strlen(end);
	if(i > j)
	{
		return false;
	}
	for( ;i > 0 && j>0 ; i--,j-- )1
	{
		if(input[i] != end[j])
		{
			return false;
		}
	}
	return true;
}*/
