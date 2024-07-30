#ifndef SUSU_THREAD_CONTROLER
#define SUSU_THREAD_CONTROLER

#include "susu_thread-worker.hpp"

#ifndef SUSU_THREAD_POOL_INIT_COUNT
    #define max_count 256
    #define min_count 1
#endif

namespace susu_tools{

class susu_thread_pool{
public:
    static susu_thread_pool* get_susu_thread_pool();
    static ::std::mutex init_mutex;
    static void set_init_count(int n);
    static int init_count;

    
    template<class F, class... Args>
    int add_task(F&& f, Args&&...args)
    {
    	int ret = 0;
    	Dynamic_Expansion_MUTEX.lock();
        	ret = workers[allocate_index]->add_a_thread_task(::std::forward<F>(f), ::std::forward<Args>(args)...);
        	load_balance();
    	Dynamic_Expansion_MUTEX.unlock();
    	return ret;
    }

    int load_balance();
    int allocate_index=0; // load_alance() will set the allocate_index

    void dynamic_expansion_reduction();
    int dynamic_param = 1;  //a param about the dynamic_expansion_reduction,if this param is 0,then Thread will not change the thread count;

    bool expansion_check();

    
    void set_dynamic_param(int n);
    int get_dynamic_param();

    void set_thread_low_limit(int n);
    int get_thread_low_limit();

    void set_thread_up_limit(int n);
    int get_thread_up_limit();

private:
    susu_thread_pool();
    ~susu_thread_pool();

    void add_a_thread();

    int thread_count = 0; // dynamic_expansion() will add or reduce the thread_count
    int thread_low_limit = 1;
    int thread_up_limit = 999;

    ::std::vector<susu_thread_worker*> workers;
    ::std::vector<::std::thread> threads;

    ::std::thread Load_Balance; //use to check thread
    ::std::mutex Load_Balance_MUTEX; //use to check thread;

    ::std::thread Dynamic_Expansion; //use to check thread
    ::std::mutex Dynamic_Expansion_MUTEX; //use to change thread counts;
};

}//namespace susu_tools
#endif
