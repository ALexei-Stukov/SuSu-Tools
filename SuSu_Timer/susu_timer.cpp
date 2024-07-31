#include "susu_timer.hpp"

namespace susu_tools{

void susu_timer::begin()
{
    gettimeofday(&begin_timeval, NULL);
}

void susu_timer::end()
{
    gettimeofday(&end_timeval, NULL);
}

timeval susu_timer::get_begin()
{
    return begin_timeval;    
}
timeval susu_timer::get_end()
{
    return end_timeval;    
}

timeval susu_timer::get_difference()
{
    timeval ret = {end_timeval.tv_sec - begin_timeval.tv_sec,end_timeval.tv_usec - begin_timeval.tv_usec};
    if(ret.tv_usec<0)
    {
        ret.tv_sec -= 1;
        ret.tv_usec += 1000*1000;
    }
    /*this timer can only accurate to ms*/
    /*in most of Linux system,the delta is 20-500 us*/
    return ret;
}

long long susu_timer::get_difference_ms()
{
    timeval ret = {end_timeval.tv_sec - begin_timeval.tv_sec,end_timeval.tv_usec - begin_timeval.tv_usec};
    if(ret.tv_usec<0)
    {
        ret.tv_sec -= 1;
        ret.tv_usec += 1000*1000;
    }

    /*this timer can only accurate to ms*/
    /*in most of Linux system,the delta is 20-500 us*/
    long long ret_ms = ret.tv_sec*1000 + ret.tv_usec/1000;
    return ret_ms;
}

void susu_timer::delay(long sec,long us){
    timespec tx = {sec,us*1000};
    nanosleep(&tx,NULL);
}

}//namespace susu_tools
