#include "susu_thread-worker.hpp"

using namespace susu_tools;

int susu_thread_worker::get_thread_task_count(){
	return thread_task_queue.get_task_queue_size();
}

FUTURE_RET susu_thread_worker::get_thread_a_result(){
	//try to get the result in 3 times.
	//return when the state is READY
	FUTURE_RET ret;
	for(int times = 0;times < 3;times++)
	{
		ret = thread_task_queue.get_a_future_result(300);	//wait for 200ms
		if( ret.status == SUSU_FUTURE_READY)
		{
			return ret;
		}
		else
		{
			continue;
		}
	}
	return ret;
}


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

void susu_thread_worker::thread_sleep(int million)
{
	set_sleep_millions(million);
	if(million == 0)
	{
		set_sleep_flag(false);
	}
	else
	{
		set_sleep_flag(true);
	}
}

void susu_thread_worker::thread_run()
{
	set_run_flag(true);
}
void susu_thread_worker::thread_pause()
{
	set_run_flag(false);
}
void susu_thread_worker::thread_exit()
{
	set_exit_flag(true);
}

bool susu_thread_worker::get_run_flag()
{
	return run;
}

void susu_thread_worker::set_run_flag(bool flag)
{
	run = flag;
}

bool susu_thread_worker::get_exit_flag()
{
	return exit;
}
void susu_thread_worker::set_exit_flag(bool flag)
{
	exit = flag;
}

bool susu_thread_worker::get_sleep_flag()
{
	return sleep;
}
void susu_thread_worker::set_sleep_flag(bool flag)
{
	sleep = flag;
}

int susu_thread_worker::get_sleep_millions()
{
	return sleep_millions;
}
void susu_thread_worker::set_sleep_millions(int millions)
{
	sleep_millions = millions;
}
