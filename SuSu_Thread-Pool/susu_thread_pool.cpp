#include "susu_thread_pool.hpp"

#include <cmath>

#include <stdio.h>
using namespace susu_tools;

susu_thread_pool* susu_thread_pool_handle = NULL;
susu_thread_pool* susu_thread_pool::get_susu_thread_pool()
{
    if(susu_thread_pool_handle == NULL)
    {
        init_mutex.lock();
        if(susu_thread_pool_handle == NULL)
        {
            susu_thread_pool_handle = new susu_thread_pool();
        }
        init_mutex.unlock();
    }
    return susu_thread_pool_handle;
}

::std::mutex susu_thread_pool::init_mutex;

void susu_thread_pool::add_a_thread()
{
    susu_thread_worker* worker = new susu_thread_worker();    // build a new susu_thread_worker
        
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

int susu_thread_pool::init_count;
void susu_thread_pool::set_init_count(int n)
{
        int temp = n;
        if(temp > max_count)
        {
		temp = max_count;
	}

        if(temp < min_count)
        {
		temp = min_count;
	}

        init_count = temp;
}

susu_thread_pool::susu_thread_pool()
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
    //Dynamic_Expansion = ::std::thread([this](){this->dynamic_expansion_reduction();});
}

//compare function
bool task_count_cmp(int& a,int& b){
    return a < b;
};

//allocate is according to the count of Task in task queue
int susu_thread_pool::load_balance()
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

//if the workers[allocate_index] have 4 Task in Task Queue,try to get a new thread and a susu_thread_worker
void susu_thread_pool::dynamic_expansion_reduction()
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


bool susu_thread_pool::expansion_check()
{
       	//thread poll will init a new thread if this function return true
	if(dynamic_param == 0)
        {
		return false;
	}

        int sum_thread_task_count = 0;
        int sum_thread_count = 0;
        for(auto it = workers.begin();it != workers.end();it++)
        {
		sum_thread_task_count += (*it)->get_thread_task_count();
		sum_thread_count++;
        }

        //if sqrt(sum_thread_task_count)/dynamic_param > sum_thread_count, init a new thread

        int temp = (sum_thread_task_count >> 1)/dynamic_param - sum_thread_count;
        if(temp > 0)
        {
		return true;
	}
        else
        {
		return false;
	}
}

void susu_thread_pool::set_dynamic_param(int n)
{
        int temp = n;
        if(temp > max_count)
        {
		temp = max_count;
	}

        if(temp < min_count)
        {
		temp = min_count;
	}

        dynamic_param = temp;
}

int susu_thread_pool::get_dynamic_param()
{
	return dynamic_param;
}

void susu_thread_pool::set_thread_low_limit(int n)
{
        int temp = n;
        if(temp > max_count)
        {
		temp = max_count;
	}

        if(temp < min_count)
        {
		temp = min_count;
	}

        thread_low_limit = temp;
}

int susu_thread_pool::get_thread_low_limit()
{
	return thread_low_limit;
}

void susu_thread_pool::set_thread_up_limit(int n)
{
        int temp = n;
        if(temp > max_count)
        {
		temp = max_count;
	}

        if(temp < min_count)
        {
		temp = min_count;
	}

        thread_up_limit = temp;
}
int susu_thread_pool::get_thread_up_limit()
{
	return thread_up_limit;
}
