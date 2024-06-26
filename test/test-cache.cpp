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
void print_cache_message(susu_cache<string>& Cache)
{
	cout<<"the map.size() in Cache is:"<<Cache.get_meta_store_size()<<endl;
	cout<<"the list.size() in Cache is:"<<Cache.get_data_store_size()<<endl;
	cout<<endl;
}

int main()
{
	cout<<"---------------------------------------"<<endl;
	cout<<"now begin to test SuSu_Cache"<<endl;

	susu_timer tools;

	//------------------------------------------------------
	//Init

	susu_cache<string> Cache;
	
	//Cache.set_limit(100);  you can try to use this code

	cout<<"the pointer of Cache is:"<<&Cache<<endl;
	cout<<endl;

	//------------------------------------------------------
	//add   get  remove
	string str = "SuSu_Cache test value=";
	cout<<"try to insert "<<BLOCK<<" string object by "<<"add"<<endl;
	tools.begin();

	for(long long i=0;i<BLOCK;i++)
	{
		string id = to_string(i);
		string value = str+id;
		Cache.add(id,value);
	}

	tools.end();

	print_time(tools);
	print_cache_message(Cache);

	if(SUCCESS == Cache.find_key(to_string(9827)))
	{	
		cout<<"when the key=9827,the value is:"<<*(Cache.get(to_string(9827)))<<endl;
	}
	cout<<"after get():"<<endl;
	print_cache_message(Cache);


	Cache.remove("111");
	cout<<"after remove():"<<endl;
	print_cache_message(Cache);

	Cache.free_data();
	cout<<"after free_data():"<<endl;
	print_cache_message(Cache);
	//------------------------------------------------------
	//add_swap update get_swap 
	
	cout<<"try to insert "<<BLOCK<<" string object by "<<"add_swap"<<endl;
	tools.begin();

	for(int i=BLOCK;i<2*BLOCK;i++)
	{
		string key = to_string(i);
		string* value = new string(str+key);
		Cache.add_swap(key,&value);
	}

	tools.end();
	print_time(tools);
	print_cache_message(Cache);

	cout<<"update the 9827"<<endl;
	Cache.update("9827",new string("WDNMD"));

	if(SUCCESS == Cache.find_key(to_string(9827)))
	{
		cout<<"when the key=9827,the value is:"<<*(Cache.get_swap(to_string(9827)))<<endl;
	}	

	cout<<"after get_swap():"<<endl;
	print_cache_message(Cache);
	if( Cache.find_key("9827") != SUCCESS)
	{
		cout<<"9827 is empty now"<<endl;
	}
	return 0;
}

