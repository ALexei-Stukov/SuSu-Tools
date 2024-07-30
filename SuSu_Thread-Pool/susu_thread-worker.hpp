#ifndef THREAD_OBJECT
#define THREAD_OBJECT

#include <vector>
#include <queue>
#include <memory>
#include <thread>
#include <mutex>
#include <future>
#include <functional>
#include <stdexcept>
#include <chrono>
#include <algorithm>

#include "susu_task-queue.hpp";

namespace susu_tools{
class susu_thread_worker{
public:
 
    int get_thread_task_count();
    
    FUTURE_RET get_thread_a_result();   //get a future from thread_task_queue.    
 
    void thread_run();	//change the run flag to true
    void thread_exit();	//change the exit flag to true
    void thread_pause();	//change the run flag to false

    //add a task to thread_task_queue
    template<class F, class... Args>
    int add_a_thread_task(F&& f, Args&&...args)
    {
	    return thread_task_queue.add_a_task(forward<F>(f),forward<Args>(args)...);
    }

    void work_loop(); // execute tasks one by one.
                      //
                      // flag: run exit sleep 
                      // These tags control the behavior of work loop
private:

    bool get_run_flag();
    void set_run_flag(bool flag);

    bool get_exit_flag();
    void set_exit_flag(bool flag);
    
    bool get_sleep_flag();
    void set_sleep_flag(bool flag);
    
    int get_sleep_millions();
    void set_sleep_millions(int millions);  
    void thread_sleep(int million); // if use shread_sleep(0), the thread will not sleep 

    bool run = true; // run flag
    bool exit = false; // stop flag
    bool sleep = false; // sleep flag
    int sleep_millions = 0; // if sleep flag is true, thread will sleep
    
    susu_task_queue thread_task_queue;
};

}//namespace susu_tools   
#endif
