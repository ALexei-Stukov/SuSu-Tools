#include <iostream>
#include <unistd.h>

#include <sys/time.h>
#include <string>

#include "susu_cache.hpp"
#include "susu_timer.hpp"

#define BLOCK 1024*1024

using namespace susu_tools;
using namespace std;

void print_time(susu_timer timer)
{
	timeval ret = timer.get_difference();
	printf("the time difference is %ld sec %ld ms %ld us\n",ret.tv_sec,ret.tv_usec/1000,ret.tv_usec%1000);
}
void print_cache_message(susu_cache& Cache)
{
	cout<<"the data_store.size() in Cache is:"<<Cache.get_current_count()<<endl;
	cout<<endl;
}

int main()
{
	cout<<"---------------------------------------"<<endl;
	cout<<"now begin to test SuSu_Cache"<<endl;

	susu_timer tools;

	//------------------------------------------------------
	//Init

	susu_cache Cache;

	cout<<"the pointer of Cache is:"<<&Cache<<endl;
	cout<<endl;

	//------------------------------------------------------
	string str = "|SuSu_Cache test| key=";
	cout<<"try to insert "<<BLOCK<<" string object by "<<"add"<<endl;
	
	tools.begin();

	for(long long i=0;i<BLOCK;i++)
	{
		string id = to_string(i);
		//string value = str+id;
		//Cache.add(id,value);
		Cache.add(id,i);
	}

	tools.end();

	print_time(tools);
	print_cache_message(Cache);
	
	string temp = to_string(27);

	if(SUCCESS == Cache.find(temp))
	{	
		//cout<<"when the key=27,the value is:"<<*(Cache.get<string>(temp))<<endl;
		cout<<"when the key=27,the value is:"<<*(Cache.get<int>(temp))<<endl;
	}
	cout<<"after get():"<<endl;
	print_cache_message(Cache);

	temp = to_string(111);

	Cache.remove(temp);
	cout<<"after remove():"<<endl;
	print_cache_message(Cache);


	if(SUCCESS == Cache.find("111"))
	{	
		//cout<<"when the key=111the value is:"<<*(Cache.get<string>(temp))<<endl;
		cout<<"when the key=111,the value is:"<<*(Cache.get<int>("111"))<<endl;
	}
	else
	{
		cout<<"key=111 had been removed"<<endl;
	}
	cout<<"test end"<<endl;
	return 0;
}

