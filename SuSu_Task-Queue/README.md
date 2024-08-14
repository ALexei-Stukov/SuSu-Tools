## Short describe about this task queue

[Introduction of SuSu-Tools](../README.md)

SuSu_Task-Queue is a **very useful tool**,expecially in multi-thread environment.

The tool is easy to use,because it is built by templates.

## What is task?

Task is a function which had converted to closure.That means "task = C++ Lambda"

Task = context + function.

We can "store" a task by templory,and run this task whenever you want.**As long as the context is correct, the function will always return the correct result.**

## Every function can be a task?

It depends on whether you want to use **asynchronous** features.

If you don't care about the return value of task,every function can be a task.In this situation,you don't need to use **asynchronous** features.

But if you want to store the return value of task,you had to use **asynchronous** features.We can't get the return value before the function had been executed done,so the return value must be a **future value**.

```cpp
    #include <future>   //the value
``` 

Every return value will be store in a result queue,the result queue is:

```cpp
    queue< future<int> > result_queue; //future  task must be like this: int function(xxx,xxx,xxx......);
``` 

So the type of return value must be **int**.

Ofcourse,you can change the type of return type like this:

```cpp
    queue< future< std::string > > result_queue; //change the type
``` 

Then the retury type can be std::string.

## How to use this tool (use asynchronous features)?

just like this:

```cpp
#include "susu_task-queue.hpp"
using namespace susu_tools;

#include <stdio.h>

//prepare a function like   int function(xxx,xxx,xxx....)
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
	susu_task_queue t;  // build a task queue

	for(int i = 0; i < 20;i++)
	{
		int add = t.add_a_task_future(funny,i); //add a future task.the return value will be store in result task
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
		t.execute_a_task(); //execute the tesk
	}
	for(int i = 0; i < 20;i++)
	{
		FUTURE_RET  ret = t.get_a_future_result(100); // wait for 100ms and try to get the return value.

		printf("get a future: %d,status: %d \n",ret.result,ret.status);
	}
	
	
	return 0;
}

```