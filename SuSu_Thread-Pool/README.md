[Introduction of SuSu-Tools](../README.md)

This tool can building threads smoothly.Every thread has a **work-loop**,we can execute some codes on work-loop.

For example:

```cpp
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
	susu_thread_pool::set_init_count(2);    //set the init param for thread pool
	
    susu_thread_pool* tp = susu_thread_pool::get_susu_thread_pool();    //build the thread pool,so we have 2 threads now.

    for(int i=0;i<10000;i++)
	{
        //Add a task to thread pool.
        //Thread pool will assign these tasks to all threads.
		tp->add_task(funny,i);
	}

    getchar();
    getchar();   
	return 0;
}

```

Thread pool can control the work-loop,But thread pool is not omnipotent.

stop the work-loop **right now**.   NO

let work-loop sleep 100ms **after this loop finished**.     YES

let work-loop stop 100ms **after this loop finished**.     YES

```cpp
void susu_thread_worker::work_loop()
{
	for(;;)
	{
		// if run is false ,thread will sleep entil the run is true
		if(run != true)
		{
			::std::this_thread::sleep_for(::std::chrono::milliseconds(1000));
			continue;
		}
		
		// if exit is true ,thread will exit
		if(exit == true)
		{
			return;
		}
		
		// if sleep is true ,thread will have a sleep and then continue
		if(sleep == true)
		{
			::std::this_thread::sleep_for(::std::chrono::milliseconds(sleep_millions));
		}
		
		// if no task to run, thread will sleep 50 and try agagin
		if(thread_task_queue.get_task_queue_size() == 0)
		{
			::std::this_thread::sleep_for(::std::chrono::milliseconds(50));
			continue;
		}
		
		//get and run a task. the result will be store in future queue,so some another object can get this result by future
		thread_task_queue.execute_a_task();
	}
}
```

This thread pool is a **Singleton**,it can create and release threads in dynamic.We can provide a param to control this behavior.

```cpp
    void dynamic_expansion_reduction(); //we have a special thread to process the dynamic change of thread count.

    int dynamic_param = 1;  //a param about the dynamic_expansion_reduction,if this param is 0,then Thread will not change the thread count;
                            //change this value so we can change the behavior of thread pool.
```

The Main Thread (thread with main function) can't (shouldn't) be managed by thread pool.

Tips：
Thread pool can cooperate with task queue.I use this plan in SuSu_Httpd (my own httpd).

The deatil is:**use task queue in work-loop of thread**.