#include "susu_thread_pool.hpp"


using namespace susu_tools;

#include <stdio.h>



int funny(int input)
{
	int n = input;
	for( int i = 0;i<10000000;i++)
	{
		n = (n*n+1) % 10000 + 1;
	}
	printf("the result is: %d\n",n);

	return n;
}

int main()
{
	susu_thread_pool::set_init_count(2);
	susu_thread_pool* tp = susu_thread_pool::get_susu_thread_pool();
	for(int i=0;i<10000;i++)
	{
		tp->add_task(funny,i);
	}

	getchar();
	getchar();
	getchar();
	return 0;
}
