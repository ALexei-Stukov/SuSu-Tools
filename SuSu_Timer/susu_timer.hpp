#ifndef SUSU_TIMER
#define SUSU_TIMER

#include <sys/time.h>   //get detail time, ms
#include <time.h>   //get a delay, us
#include <cstddef>

namespace susu_tools{
class susu_timer{
public:
    void begin();    //get the begin time
    void end();  //get the stop time
    
    timeval get_begin();    //get the timeval of begin
    timeval get_end();    //get the timeval of end
    
    timeval get_difference();   //get the time distance  stop - begin
	long long get_difference_ms();	//get the time distance stop - begin,return by [ms].
    
    void delay(long sec,long us);    //delay a period of time

private:

    timeval begin_timeval = {0,0};
    timeval end_timeval = {0,0};
};

}//namespace susu_tools

#endif
