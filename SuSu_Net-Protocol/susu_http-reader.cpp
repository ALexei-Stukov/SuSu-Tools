#include "susu_http-reader.hpp"
#include <iostream>

using namespace susu_tools;
using std::cout;
using std::endl;

int susu_http_reader::analyse(int fd)
{
	head.remove_all();		//clean old data
	if(get_http_head(fd) == -1)
	{
		return -1;
	}
		
	while( get_a_http_line(fd) )	//不断读取http行
	{
		if( check_all_space(buffer) )	//遇到空行结束，空行即 "\r\n" 这种形式
		{
			break;
		}

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
		head.add(key,value);
	}
	auto ins = head.get_data_store();
	for(auto it = ins.begin();it != ins.end();it++)
	{
		string key = (*it).first;
		string value = *(head.get<string>(key));
		cout<<key<<" "<<value<<endl;
	}
	return 0;
}
/*//used by other function
int susu_http_reader::fd_write(const char * buffer,size_t n)
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
int susu_http_reader::fd_read(char * buffer,size_t n)
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
susu_cache& susu_http_reader::get_kv_store()
{
	return head;
}

int susu_http_reader::get_http_head(int fd)
{
	int numchars = get_a_http_line(fd);	//get the head of http request in buffer
		
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
		return -1;
	}


	//get the method,url,version from buffer
	char METHOD[line_length_limit];
	char URL[line_length_limit];
	char VERSION[line_length_limit];

   	sscanf(buffer,"%[^ ] %[^ ] %[^'\n']",METHOD,URL,VERSION);

	string Method(METHOD);
	string Url(URL);
	string Version(VERSION);

	//检查Version字符串内是否存在HTTP的字样，且不存在HTTPS的字样
	size_t pos_http = Version.find("HTTP");
	size_t pos_https = Version.find("HTTPS");
	if(pos_https != string::npos || pos_http == string::npos)
	{
		//包不合规，不作处理。
		return -1;
	}

	head.add("Method",Method);
	head.add("Url",Url);
	head.add("Version",Version);

	return 0;
}

int susu_http_reader::get_a_http_line(int fd)
{
	int length = 0;
	char ch = '\0';
	int numchars;
	while ( length < line_length_limit - 3 && ch != '\n'  )	//
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

   	return length;
}

bool susu_http_reader::check_all_space(char* str) //check if all the char is space
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
bool susu_http_reader::str_end_with(char* input,char* end)  //检查某个字符串A 是否以 某个字符串B结尾
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
