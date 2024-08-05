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
	cout<<"the time differenct is "<<timer.get_difference_ms()<<" ms"<<endl;
}
void print_cache_message(susu_cache& Cache)
{
	cout<<"the list.size() in Cache is:"<<Cache.get_current_count()<<endl;
}

long long algorithm(long long N,int P)
{
	int LIMIT = 80145621;
	long long ret = N;

	for(int loop = 0 ;loop < P;loop++)
	{
		ret = ((ret*ret)-P) % LIMIT;
	}
	return ret;
}


susu_cache Cache;
long long func_without_cache()
{
	srand(1024);	//set random seed
	long long ret = 0;
	for(int loop = 0;loop < 10000000;loop++)
	{
		int N = rand()%1000;
		int P = rand()%1000;

		string key = to_string(N*1000+P);
		
		long long temp = algorithm(N,P)% 123456789;
		ret += temp;
		ret %= 1189467205234;
	}
	return ret;
}

long long func_with_cache()
{
	srand(1024);	//set random seed
	long long ret = 0;
	for(int loop = 0;loop < 10000000;loop++)
	{
		int N = rand()%1000;
		int P = rand()%1000;
		string key = to_string(N*1000+P);
		string k = key;
		
		if(SUCCESS == Cache.find(std::move(key)))
		{	
			ret += *(Cache.get<long long>(std::move(k)))% 123456789;
		}
		else
		{
			long long temp = algorithm(N,P)% 123456789;
			ret += temp;
			ret %= 1189467205234;
			Cache.add(std::move(k),temp);
		}
	}
	return ret;
}

int main()
{
	cout<<"---------------------------------------"<<endl;
	cout<<"now begin to test SuSu_Cache use in algorithm"<<endl;
	susu_timer tools;

	cout<<"function without cache:"<<endl;
	tools.begin();
	long long ret = func_without_cache();
	tools.end();
	cout<<"the resut is "<<ret<<endl;
	print_time(tools);

	cout<<"function with cache:"<<endl;
	tools.begin();
	ret = func_with_cache();
	tools.end();
	cout<<"the resut is "<<ret<<endl;
	print_time(tools);
	
	return 0;
}

