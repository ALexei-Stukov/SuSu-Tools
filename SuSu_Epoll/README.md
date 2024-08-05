[Introduction of SuSu-Tools](../README.md)

## Short describe about this tool

**susu_epoll** is a simple function-set.

This tool contains some common operationsto to help us using the epoll function.

This tool is hard to test,so I always add it into another tool,and test the another one.

## How to use this tool.

In most of times,you can use this tool like this:

```cpp
	susu_epoll* manager = new susu_epoll(1024);

	manager->add_an_event(1, EPOLLIN | EPOLLOUT );		//add a fd to epoll struct.
														//fd = 1
														//listem_param = EPOLLIN | EPOLLOUT

	while(true) //the loop of listening
	{
		if( get_epoll_result(int ms_count) != 0)
		{
			auto ins = manager->get_enents_array();		//get the array of EVENTS
			
			//process events one by one
			//........
		}
	}
```