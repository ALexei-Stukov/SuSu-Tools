#ifndef SUSU_INIT_PARAM
#define SUSU_INIT_PARAM

#include <queue>
#include <mutex>

#include <fstream>

#include<ctime>
#include<stdio.h>

#include <unordered_map>
using std::unordered_map;
using std::string;
using std::pair;
#include <iostream>
#include <string.h>

#include "susu_cache.hpp"

namespace susu_tools{

class susu_initparam{
public:
    static susu_initparam* get_Init_Param_instance();
    static ::std::mutex init_mutex;
    
    int load_init_param(const char* str);  //load param from a file.
    void init_param_print();    //print on stdout at once

    string get_value(string key);	//get value from cache

    void print_a_log_line(const char* str)
    {
        perror(str);   //print something to stderr
    };

private:
    susu_cache init_param;
    susu_initparam();
    ~susu_initparam();
};

}//namespace susu_tools
#endif
