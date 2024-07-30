#ifndef TASK_QUEUE
#define TASK_QUEUE


#include <queue>
#include <future>
#include <functional>
#include <chrono>

	using std::queue;
	using std::function;
	using std::forward;
	using std::future;
	using std::future_status;
	using std::result_of;
	using std::make_shared;
	using std::packaged_task;
	using std::bind;

namespace susu_tools{

//future is not exist
#define SUSU_FUTURE_NOT_EXIST 60000

//future is exist but not ready
#define SUSU_FUTURE_NOT_READY 60001

//future is ready
#define SUSU_FUTURE_READY 60002

//future is exist but not been computed.
//Because the function using lazy evaluation, so the result will be computed only when explicitly requested.
#define SUSU_FUTURE_DEFERRED 60003

//maybe the task is too many
#define SUSU_OUT_OF_TASK_LIMIT	40000

//maybe the future result is too many
#define SUSU_OUT_OF_RESULT_LIMIT 40001

struct FUTURE_RET{
	int result;
	int status;
};

class susu_task_queue{
public:
	susu_task_queue(int task_limit = 8192,int result_limit = 16384){
		this->task_limit = task_limit;
		this->result_limit = result_limit;
	};
	template<class F, class... Args>
	auto enqueue(F&& func, Args&&...args) -> future< typename result_of<F(Args...)>::type >
	{
		using return_type = typename result_of<F(Args...)>::type;	//check the return type of F(Args)
		
		// bind the function and params ，build a packaged_task. so that (*task)(); = f(args);
		auto task = make_shared< packaged_task<return_type()> >( bind( forward<F>(func),forward<Args>(args)...) );

    		// get this task's future，  
    		future<return_type> res = task->get_future();
    		//future<int> res = task->get_future();
  
    		// add thie task to task_queue
    		task_queue.emplace([task](){ (*task)(); });
		
		// add the future to resule queue
		result_queue.push(move(res));	//can't use copy construct,we must use move()

   		return res;
	}

	template<class F, class... Args>
	int add_a_task(F&& func, Args&&...args)
	{

		if(task_queue.size() > task_limit)
		{
			return SUSU_OUT_OF_TASK_LIMIT;
		}
		else if(result_queue.size() > result_limit) 
		{
			return SUSU_OUT_OF_RESULT_LIMIT;
		}
		else
		{
    		enqueue( forward<F>(func), forward<Args>(args)...);
    		return task_queue.size();	//return the current task count;
		}
	}

    	
	void execute_a_task()
    {
		if(get_task_queue_size() > 0 )
		{
			function<void()> task;
			//get and run a task. the result will be store in future queue,so some another object can get this result by future
			task = move(task_queue.front());
			task_queue.pop();
			task();
		}
    }

	// ms_count < 0 : wait for the result in block
	//
	// ms_count >= 0 : wait for a moment( The unit is milliseconds ).
	// 		if the time is end,try to read the future.		
	FUTURE_RET get_a_future_result(int ms_count)
	{
		//result queue is empty
		if(get_result_queue_size() == 0 )
		{
			FUTURE_RET ret = {0,SUSU_FUTURE_NOT_EXIST};
			return ret;
		}

	
		auto future = move(result_queue.front());
		
		if(ms_count < 0) //wait in block
		{
			future.wait();	//block on here
			int result = future.get();
			result_queue.pop();

			FUTURE_RET ret = {result,SUSU_FUTURE_NOT_READY};
			return ret;
		}
		else 
		{
			future_status status;
			std::chrono::milliseconds span(ms_count);	
			status = future.wait_for(span);

			switch(status)
			{	
				case future_status::deferred:
				{
					//int result = future.get();
					//result_queue.pop();
					FUTURE_RET ret = {0,SUSU_FUTURE_DEFERRED};
					return ret;
				}
            			case future_status::ready:
				{
					int result = future.get();
					result_queue.pop();
					FUTURE_RET ret = {result,SUSU_FUTURE_READY};
					return ret;
				}
				default:	// this case is future_status::timeout:
				{
					FUTURE_RET ret = {0,SUSU_FUTURE_NOT_READY};
					return ret;
				}
        	}
		}
    }

	int get_task_queue_size()
	{
		return task_queue.size();
	}

	int get_result_queue_size()
	{
		return result_queue.size();
	}


private:
    queue< function<void()> > task_queue;  //lambda function task

    queue< future<int> > result_queue; //future  task must be like this: int function(xxx,xxx,xxx......);

    int task_limit;	//the length of task_queue can't be endless
    
    int result_limit;	//the length of result_queue can't be endless
};

}//namespace susu_tools    
#endif
						//the interface to add a task to queue.
						//	task = the function need to be execute;
						//
						//this interface will call the enqueue(F&& func,Args&&...args)
						//
						//the return value of function will store in a queue too
