#include "susu_http.hpp"
#include <iostream>

namespace susu_tools{
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
void susu_http_object::get_request_and_header(int fd)
{
	int numchars = get_a_line(fd,buffer, sizeof(buffer)); //get the head of http request in buffer
    	
	//check the space count
    	int space_count=0;
    	for(int i=0;i<numchars;i++)
    	{
        	if(buffer[i] == ' ')
        	{
            		space_count++;
        	}
    	}

	//the first line of normal http request have 2 space.
    	if(space_count != 2)
    	{
        	return;
    	}


    	//get the method,url,version from buffer
	char[line_length_limit] METHOD;
	char[line_length_limit] URL;
	char[line_length_limit] VERSION;

   	sscanf(buffer,"%[^ ] %[^ ] %[^'\n']",METHOD,URL,VERSION);
    	method = METHOD;
    	url = URL;
    	version = version;

    	while(!check_all_space(buffer))
    	{
        	numchars = get_a_line(fd, buffer, sizeof(buffer));
        	string line = buffer;
        	size_t pos = line.find(": "); // pos == 7
        	if(pos == string::npos)    //忽略不带等号的行
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

int susu_http_object::get_a_line(int sock, char *buf, int size)
{
    int i = 0;
    char c = '\0';
    int n;
    while ((i < size - 1) && (c != '\n'))
    {
        n = recv(sock, &c, 1, 0);
        if (n > 0)
        {
            if (c == '\r')  //遇到'\r'时，尝试构造字符串结尾
            {
                n = recv(sock, &c, 1, MSG_PEEK);    //使用MSG_PEEK拷贝宏，先从sock中拷贝一下内容，看看是不是'\n'，如果是，则读取。如果不是，则直接忽略并默认在字符串后加上\n。
                if ((n > 0) && (c == '\n'))
                recv(sock, &c, 1, 0);
                else
                c = '\n';
            }
            buf[i] = c;
            i++;
        }
        else
        {break;}
    }

    buf[i] = '\0';
    return(i);
}


void susu_http_object::clear_head_body()
{
	method = "";
	url = "";
	version = "";
}

bool susu_http_object::check_all_space(char* str) //check if all the char is space
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
bool susu_http_object::str_end_with(char* input,char* end)  //检查某个字符串A 是否以 某个字符串B结尾
{
    int i = strlen(input);
    int j = strlen(end);
    if(i > j)
    {
        return false;
    }
    for( ;i > 0 && j>0 ; i--,j-- )
    {
        if(input[i] != end[j])
        {
            return false;
        }
    }
    return true;
}*/

}

//客户端的常见头部键值对，当然，也可以自定义。但最好不要自定义。
/*
Accept    指定客户端能够接收的内容类型    Accept: text/plain, text/html 
Accept-Charset    浏览器可以接受的字符编码集。    Accept-Charset: iso-8859-5 
Accept-Encoding    指定浏览器可以支持的web服务器返回内容压缩编码类型。    Accept-Encoding: compress, gzip 
Accept-Language    浏览器可接受的语言    Accept-Language: en,zh 
Accept-Ranges    可以请求网页实体的一个或者多个子范围字段    Accept-Ranges: bytes 
Authorization    HTTP授权的授权证书    Authorization: Basic QWxhZGRpbjpvcGVuIHNlc2FtZQ== 
Cache-Control    指定请求和响应遵循的缓存机制    Cache-Control: no-cache 
Connection    表示是否需要持久连接。（HTTP 1.1默认进行持久连接）    Connection: close 
Cookie    HTTP请求发送时，会把保存在该请求域名下的所有cookie值一起发送给web服务器。    Cookie: $Version=1; Skin=new; 
Content-Length    请求的内容长度    Content-Length: 348 
Content-Type    请求的与实体对应的MIME信息    Content-Type: application/x-www-form-urlencoded 
Date    请求发送的日期和时间    Date: Tue, 15 Nov 2010 08:12:31 GMT 
Expect    请求的特定的服务器行为    Expect: 100-continue 
From    发出请求的用户的Email    From: user@email.com 
Host    指定请求的服务器的域名和端口号    Host: www.zcmhi.com 
If-Match    只有请求内容与实体相匹配才有效    If-Match: “737060cd8c284d8af7ad3082f209582d” 
If-Modified-Since    如果请求的部分在指定时间之后被修改则请求成功，未被修改则返回304代码    If-Modified-Since: Sat, 29 Oct 2010 19:43:31 GMT 
If-None-Match    如果内容未改变返回304代码，参数为服务器先前发送的Etag，与服务器回应的Etag比较判断是否改变    If-None-Match: “737060cd8c284d8af7ad3082f209582d” 
If-Range    如果实体未改变，服务器发送客户端丢失的部分，否则发送整个实体。参数也为Etag    If-Range: “737060cd8c284d8af7ad3082f209582d” 
If-Unmodified-Since    只在实体在指定时间之后未被修改才请求成功    If-Unmodified-Since: Sat, 29 Oct 2010 19:43:31 GMT 
Max-Forwards    限制信息通过代理和网关传送的时间    Max-Forwards: 10 
Pragma    用来包含实现特定的指令    Pragma: no-cache 
Proxy-Authorization    连接到代理的授权证书    Proxy-Authorization: Basic QWxhZGRpbjpvcGVuIHNlc2FtZQ== 
Range    只请求实体的一部分，指定范围    Range: bytes=500-999 
Referer    先前网页的地址，当前请求网页紧随其后,即来路    Referer: http://www.zcmhi.com/archives/71.html 
TE    客户端愿意接受的传输编码，并通知服务器接受接受尾加头信息    TE: trailers,deflate;q=0.5 
Upgrade    向服务器指定某种传输协议以便服务器进行转换（如果支持）    Upgrade: HTTP/2.0, SHTTP/1.3, IRC/6.9, RTA/x11 
User-Agent    User-Agent的内容包含发出请求的用户信息    User-Agent: Mozilla/5.0 (Linux; X11) 
Via    通知中间网关或代理服务器地址，通信协议    Via: 1.0 fred, 1.1 nowhere.com (Apache/1.1) 
Warning    关于消息实体的警告信息    Warn: 199 Miscellaneous warning 
*/

//服务端的常见头部键值对，当然，也可以自定义。但最好不要自定义。
/*
Accept-Ranges    表明服务器是否支持指定范围请求及哪种类型的分段请求    Accept-Ranges: bytes 
Age    从原始服务器到代理缓存形成的估算时间（以秒计，非负）    Age: 12 
Allow    对某网络资源的有效的请求行为，不允许则返回405    Allow: GET, HEAD 
Cache-Control    告诉所有的缓存机制是否可以缓存及哪种类型    Cache-Control: no-cache 
Content-Encoding    web服务器支持的返回内容压缩编码类型。    Content-Encoding: gzip 
Content-Language    响应体的语言    Content-Language: en,zh 
Content-Length    响应体的长度    Content-Length: 348 
Content-Location    请求资源可替代的备用的另一地址    Content-Location: /index.htm 
Content-MD5    返回资源的MD5校验值    Content-MD5: Q2hlY2sgSW50ZWdyaXR5IQ== 
Content-Range    在整个返回体中本部分的字节位置    Content-Range: bytes 21010-47021/47022 
Content-Type    返回内容的MIME类型    Content-Type: text/html; charset=utf-8 
Date    原始服务器消息发出的时间    Date: Tue, 15 Nov 2010 08:12:31 GMT 
ETag    请求变量的实体标签的当前值    ETag: “737060cd8c284d8af7ad3082f209582d” 
Expires    响应过期的日期和时间    Expires: Thu, 01 Dec 2010 16:00:00 GMT 
Last-Modified    请求资源的最后修改时间    Last-Modified: Tue, 15 Nov 2010 12:45:26 GMT 
Location    用来重定向接收方到非请求URL的位置来完成请求或标识新的资源    Location: http://www.zcmhi.com/archives/94.html 
Pragma    包括实现特定的指令，它可应用到响应链上的任何接收方    Pragma: no-cache 
Proxy-Authenticate    它指出认证方案和可应用到代理的该URL上的参数    Proxy-Authenticate: Basic 
refresh    应用于重定向或一个新的资源被创造，在5秒之后重定向（由网景提出，被大部分浏览器支持）Refresh: 5; url=http://www.zcmhi.com/archives/94.html 
Retry-After    如果实体暂时不可取，通知客户端在指定时间之后再次尝试    Retry-After: 120 
Server    web服务器软件名称    Server: Apache/1.3.27 (Unix) (Red-Hat/Linux) 
Set-Cookie    设置Http Cookie    Set-Cookie: UserID=JohnDoe; Max-Age=3600; Version=1 
Trailer    指出头域在分块传输编码的尾部存在    Trailer: Max-Forwards 
Transfer-Encoding    文件传输编码    Transfer-Encoding:chunked 
Vary    告诉下游代理是使用缓存响应还是从原始服务器请求    Vary: * 
Via    告知代理客户端响应是通过哪里发送的    Via: 1.0 fred, 1.1 nowhere.com (Apache/1.1) 
Warning    警告实体可能存在的问题    Warning: 199 Miscellaneous warning 
WWW-Authenticate    表明客户端请求实体应该使用的授权方案    WWW-Authenticate: Basic 
*/
