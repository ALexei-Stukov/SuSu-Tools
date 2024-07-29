#include "susu_task-queue.hpp"

using namespace susu_tools;

void Thread_Object::work_loop()
{
	::std::function<void()> task;
	for(;;)
	{
		//get and run a task. the result will be store in future queue,so some another object can get this result by future
		task = ::std::move(task_queue.front());
		task_queue.pop();
		task();
	}
}
