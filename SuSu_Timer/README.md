[susu-tools](../README.md)

SuSu-Timeris a tiny and useful timer.It is a **common component** in SuSu-Tools

you can use it just like this:

```cplusplus
#include "susu_timer.hpp"
#include <cstdio>

using namespace susu_tools;
int main()
{
    susu_timer time_tool;
    
    time_tool.begin();      // Set start time
    time_tool.delay(3,500); //sleep (3s + 500us)
    time_tool.end();        //Set end time
    
    timeval ret = time_tool.get_difference();   //difference = end - start
    
    //show difference
    printf("the time difference is %ld sec %ld ms %ld us\n",ret.tv_sec,ret.tv_usec/1000,ret.tv_usec%1000);
    
    return 0;
}

```
