#include "Thread_Controler"
#include <cmath>

#include <stdio.h>



namespace susu_tools{
    
Thread_Controler* Thread_Controler_handle = NULL;
Thread_Controler* Thread_Controler::get_Thread_Controler()
{
    if(Thread_Controler_handle == NULL)
    {
        init_mutex.lock();
        if(Thread_Controler_handle == NULL)
        {
            Thread_Controler_handle = new Thread_Controler();
        }
        init_mutex.unlock();
    }
    return Thread_Controler_handle;
}

::std::mutex Thread_Controler::init_mutex;

void Thread_Controler::add_a_thread()
{
    Thread_Object* worker = new Thread_Object();    // build a new Thread_Object
        
    workers.emplace_back(worker);   // add to workers
    worker->thread_run();
        
        
    //add to threads by lambda
    threads.emplace_back(
        [worker]
        ()
        {
            worker->work_loop();
        }
    );
}

int Thread_Controler::init_count;
void Thread_Controler::set_init_count(int n)
{
        int temp = n;
        if(temp > max_count)
        {temp = max_count;}

        if(temp < min_count)
        {temp = min_count;}

        init_count = temp;
}

Thread_Controler::Thread_Controler()
{
    //Load_Balance
    //Load_Balance = ::std::thread([this](){this->load_balance();});

    //you must add thread at first
    int i;
    for(i=0;i<init_count;i++)
    {
        add_a_thread();
    }

    //printf("%d %d\n",workers.size(),threads.size());
    

    //Dynamic_Expansion
    Dynamic_Expansion = ::std::thread([this](){this->dynamic_expansion_reduction();});
}

//compare function
bool task_count_cmp(int& a,int& b){
    return a < b;
};

//allocate is according to the count of Task in task queue
int Thread_Controler::load_balance()
{
    /*while(true)
    {*/
        int sum = 0;
        ::std::vector<int> task_count;
        for(auto i = workers.begin();i != workers.end();i++)
        {
            task_count.push_back((*i)->get_thread_task_count());
            sum += *(task_count.end());
        }
        auto min_pos = ::std::min_element(task_count.begin(),task_count.end(),task_count_cmp);
        allocate_index = ::std::distance(task_count.begin(),min_pos);
        
        return sum;
        /*::std::this_thread::sleep_for(::std::chrono::milliseconds(10));*/
        
        /* debug message */
        /*for(auto i = workers.begin();i != workers.end();i++)
        {
            ::std::cout<<(*i)->get_thread_task_count()<<" ";
        }
        ::std::cout<<::std::endl;*/    
    /*}*/
}

/*
* if every Task cost a seconds
* [a] wait:0
* [a][a] wait: 1 a
* [a][a][a] wait: (1+2)a
* [a][a][a][a] wait: (1+2+3)a
* wait = (1+n)*n/2 a
* n=4, wait = 10a
*/

//if the workers[allocate_index] have 4 Task in Task Queue,try to get a new thread and a Thread_Object
void Thread_Controler::dynamic_expansion_reduction()
{
    while(true)
    {
        int task_sum = load_balance();
        if(expansion_check()==true)
        {
            if(thread_count<thread_up_limit)
            {
                Dynamic_Expansion_MUTEX.lock();
                    add_a_thread();
                    thread_count++;
                Dynamic_Expansion_MUTEX.unlock();
                ::std::this_thread::sleep_for(::std::chrono::milliseconds(100));//avoid to add or reduce thread frequently
            }
        }
        

        //try to reduce the last worker,if it's count is 0
        else if( (*(workers.rbegin()))->get_thread_task_count() == 0)
        {
            if(thread_count>thread_low_limit)
            {
                ::std::this_thread::sleep_for(::std::chrono::milliseconds(500));//avoid to add or reduce thread frequently
                Dynamic_Expansion_MUTEX.lock();
                    if( (*(workers.rbegin()))->get_thread_task_count() == 0)
                    {
                        workers.pop_back();
                        thread_count--;
                    }
                Dynamic_Expansion_MUTEX.unlock();
            }
        }
        
        //this code will print the count in task_queue,maybe zhe thread is running a task,so the real count is task_queue.size()+1  .but that is not important. we don't need to know if is +1 or not.
        /*
        ::std::cout<<"the task count is:\n";
        for(auto i = workers.begin();i != workers.end();i++)
        {
            ::std::cout<<(*i)->get_thread_task_count()<<" ";
        }
        ::std::cout<<::std::endl;
        */

        ::std::this_thread::sleep_for(::std::chrono::milliseconds(200));
    }
}
}