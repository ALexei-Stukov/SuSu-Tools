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
    timeval get_difference();   //get the time distance  stop - start
	long long get_difference_ms();	//get the time distance stop - start,return by [ms].
        void delay(long sec,long us);    //delay a period of time
private:

    timeval start = {0,0};
    timeval stop = {0,0};
};

}//namespace susu_tools

#endif
