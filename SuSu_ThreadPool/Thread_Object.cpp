#include "Thread_Object"

namespace susu_tools{
/*Thread_Object::Thread_Object(){
    //worker = new ::std::thread(work_loop,this);
    //worker = &temp;
}
Thread_Object::~Thread_Object(){

}*/


::std::thread::id Thread_Object::get_thread_id(){
   //return worker.get_id(); 
}

int Thread_Object::get_thread_task_count(){
    return task_queue.size();
}

int Thread_Object::get_thread_a_result(){
    int ret = task_result_queue.front().get();
    task_result_queue.pop();
    return 0;
    
}


void Thread_Object::work_loop()
{
    ::std::function<void()> task;
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
            return ;
        }

        // if sleep is true ,thread will have a sleep and then continue
        if(sleep == true)
        {
            ::std::this_thread::sleep_for(::std::chrono::milliseconds(sleep_time));
        }

        // if no task to run, thread will sleep 50 and try agagin
        if(task_queue.empty())
        {
            ::std::this_thread::sleep_for(::std::chrono::milliseconds(50));
            continue;
        }
        
        //get and run a task. the result will be store in future queue,so some another object can get this result by future
        task = ::std::move(task_queue.front());
        task_queue.pop();
        task();
    }
}

void Thread_Object::thread_sleep(int million)
{
    set_sleep_time(million);
    if(million == 0)
    {
        set_sleep_flag(false);
    }
    else
    {
        set_sleep_flag(true);
    }
}
void Thread_Object::thread_run()
{
    set_run_flag(true);
}
void Thread_Object::thread_pause()
{
    set_run_flag(false);
}
void Thread_Object::thread_exit()
{
    set_exit_flag(true);
}
}