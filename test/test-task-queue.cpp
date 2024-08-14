#include "susu_task-queue.hpp"
using namespace susu_tools;

#include <stdio.h>


int funny(int input)
{
	int n = input;
	for( int i = 0;i<10000000;i++)
	{
		n = (n*n + 1) % 100000 + 1;
	}
	printf("the result is: %d\n",n);
	return n;
}

int main()
{
	susu_task_queue t;

	for(int i = 0; i < 20;i++)
	{
		int add = t.add_a_task_future(funny,i);
		if( add != SUSU_OUT_OF_TASK_LIMIT || add != SUSU_OUT_OF_RESULT_LIMIT)
		{
			printf("insert successfully\n");
		}
		else
		{
			printf("insert failed\n");
		}
	}
	for(int i = 0; i < 20;i++)
	{
		t.execute_a_task();
	}
	for(int i = 0; i < 20;i++)
	{
		FUTURE_RET  ret = t.get_a_future_result(0);

		printf("get a future: %d,status: %d \n",ret.result,ret.status);
	}
	
	
	return 0;
}
