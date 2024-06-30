#include "susu_epoll.hpp"
#include <iostream>

using namespace susu_tools;
using namespace std;

#include <typeinfo>

int game(int a,int b)
{
	for(int j=0;j<a*b;j++)
	{
		cout<<j<<endl;
	}
	return 0;
}

int main()
{
	function<int(int,int)> f(game);
	susu_epoll epoll_object(10);
        
	cout<<endl;
        cout<<"run by normal way-------------------"<<endl;
	f(8,2);

	cout<<endl;
        cout<<"run by new way-------------------"<<endl;
	epoll_object.epoll_process<function<int(int,int)>>(f,8,2);
	

	cout<<endl;
        cout<<"a more easy way-------------------"<<endl;

	// Haven't found the syntax I want yet

	return 0;
}
