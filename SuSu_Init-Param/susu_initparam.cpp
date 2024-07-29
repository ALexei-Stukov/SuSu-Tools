#include "susu_initparam.hpp"

namespace susu_tools{

susu_initparam* susu_initparam_handle = NULL;
susu_initparam* susu_initparam::get_Init_Param_instance()
{
    if(susu_initparam_handle == NULL)
    {
        init_mutex.lock();
        if(susu_initparam_handle == NULL)
        {
            susu_initparam_handle = new susu_initparam();
        }
        init_mutex.unlock();
    }
    return susu_initparam_handle;
}

::std::mutex susu_initparam::init_mutex;


susu_initparam::susu_initparam()
{
}
susu_initparam::~susu_initparam()
{
}

int susu_initparam::load_init_param(const char* str)  //加载参数
{
    try{
        //打开文件
        FILE* init_file = fopen(str,"r");
        char key[128] = {0};
        char value[1024] = {0};
        char line[1280] = {0};

        if(init_file != NULL)
        {
            memset(key,128,0);
            memset(value,1024,0);
            memset(line,1280,0);
            while(fscanf(init_file,"%s\n",line) != EOF)
            {
                int i;
                int check = 1;
                for(i=0;i<strlen(line);i++)
                {
                    if(line[i] == '=' && i <= 128)
                    {
                        strncpy(key,line,i);
                        strncpy(value,line+i+1,strlen(line)-i-1);
                        check = 0;
                        break;
                    }
                    else
                    {
                        check = 1;
                    }
                }
                if(check)
                {continue;}

		init_param.add<string>(key,value);
		/*
                if(int_param.find_key(key) != int_param_kv.end())
                {
                    int_param_kv[key] = atoi(value);
                }
                else if(init_param.find_key(key) != string_param_kv.end())
                {
                    string_param_kv[key] = value;
                }*/

                memset(key,0,128);
                memset(value,0,1024);
                memset(line,0,1280);
            }
            init_param_print();
        }
        else
        {
            throw "init config is not exist";//throw error
        }
               
    }
    catch(const char* error_string)
    {
        print_a_log_line(error_string);
        exit(-1);
    }
    return 0;
}

string susu_initparam::get_value(string key)
{
	if(init_param.find_key(key) != FAIL)
        {
		return *(init_param.get<string>(key));
        }
	else
	{
		return "ERROR";
	}
};

void susu_initparam::init_param_print()
{
	auto ins = init_param.get_data_store();
	
	for(auto it = ins.begin();it != ins.end();it++)
	{
		std::cout<<it->first
			<<"="
			<<*((string*)(it->second->data))
			<<std::endl;
	}
}

};
