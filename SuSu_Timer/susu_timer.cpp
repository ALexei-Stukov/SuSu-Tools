#include "susu_timer.hpp"

namespace susu_tools{

void susu_timer::begin(){
    gettimeofday(&start, NULL);
}

void susu_timer::end(){
    gettimeofday(&stop, NULL);
}
timeval susu_timer::get_difference()
{
    timeval ret = {stop.tv_sec - start.tv_sec,stop.tv_usec - start.tv_usec};
    if(ret.tv_usec<0)
    {
        ret.tv_sec -= 1;
        ret.tv_usec += 1000*1000;
    }
    /*this timer can only accurate to ms*/
    /*in most of Linux system,the delta is 20-500 us*/
    return ret;
}

void susu_timer::delay(long sec,long us){
    timespec tx = {sec,us*1000};
    nanosleep(&tx,NULL);
}

}//namespace susu_tools
