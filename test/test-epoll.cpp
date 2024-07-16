#include "susu_epoll.hpp"
#include <iostream>

using namespace susu_tools;
using namespace std;

#include <typeinfo>

int mul(int a,int b)
{
	for(int j=0;j<a*b;j++)
	{
		cout<<j<<endl;
	}
	return 0;
}
int message(string input)
{
	cout<<input<<endl;
	return 0;
}
int test(int N)
{
	cout<<"A HA"<<endl;
	return 0;
}
/* 在模板层面上，epoll只接受带返回值，带参数的函数，否则无法通过编译。
 * 虽然这是我能力不足的体现，但恰好，epoll就是需要这种类型的函数。
 * epoll是用于监听文件描述符的，只要监听到了数据，就要做处理。处理函数最好是有入参，有返回值，这是理所应当的设计。
 *
 * */



int main()
{
        
	cout<<endl;
        cout<<"run by normal way-------------------"<<endl;
	mul(8,2);
	message("running by normal way");
	test(3);

	cout<<endl;
        cout<<"run by new way-------------------"<<endl;
	

	function<int(int,int)> MUL(mul);
	function<int(string)> MESSAGE(message);
	function<int(int)> TEST(test);
	
	susu_epoll epoll_object(10);
	
	epoll_object.epoll_process(MUL,8,2);

	epoll_object.epoll_process(MESSAGE,string("WDNMD"));
	
	string R_VALUE="The R_value test";
	epoll_object.epoll_process(MESSAGE,std::move(R_VALUE));
	
	epoll_object.epoll_process(TEST,3);
	

	cout<<endl;
        cout<<"a more easy way-------------------"<<endl;

	// Haven't found the syntax I want yet

	//listen a fd
	epoll_object.add_a_event(0);	//linsten the std input
	
        cout<<"try to input someting please-------------------"<<endl;
	for(int loop = 0;loop < 1;loop++)
	{
		if( 0 != epoll_object.get_epoll_result(-1) )
		{
			string temp;
			cin>>temp;
			cout<<"get a input from std input! the input is "<<temp<<endl;
		}
	}
	return 0;
}
