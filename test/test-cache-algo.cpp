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

long long algorithm(long long N,int P)
{
	int LIMIT = 80145621;
	long long ret = N;
	
	/*  algorithm 1:
	 * 	 POW(N,P) % LIMIT
	 *
	 *  this algorithm is very fast,
	 *
	 *  the Key of SuSu Cache is std::string , In this algorithm , SuSu Cache cannot help you.
	 *
	int times = P - 1;
	while(times > 0)
	{
		if( times % 2 == 0 )
		{
			ret = (ret*ret) % LIMIT;
			times /= 2;
		}
		else
		{
			ret = (ret*N) % LIMIT;
			times -= 1;
		}
	}*/

	/* algorithm 2:
	 * 	A[n] = ( POW(A[n-1],2)-P ) % LIMIT
	 *
	 * In this algorithm,SuSu Cache is effective.
	 *
	 * This algorithm is very effective when the data size is small and the number of repetitions is high.
	 *
	 * */
	for(int loop = 0 ;loop < P;loop++)
	{
		ret = ((ret*ret)-P) % LIMIT;
	}
	return ret;
}

long long func_without_cache()
{
	srand(1024);	//set random seed
	long long ret = 0;
	for(int loop = 0;loop < 10000000;loop++)
	{
		int N = rand()%1000;
		int P = rand()%1000;
		ret += algorithm(N,P)% 123456789;
	}
	return ret;
}

susu_cache<long long> Cache;
long long func_with_cache()
{
	srand(1024);	//set random seed
	long long ret = 0;
	for(int loop = 0;loop < 10000000;loop++)
	{
		int N = rand()%1000;
		int P = rand()%1000;
		string key = to_string(N) + ":" + to_string(P);
		
		if(SUCCESS == Cache.find_key(key))
		{	
			ret += *(Cache.get(key))% 123456789;
		}
		else
		{
			long long temp = algorithm(N,P)% 123456789;
			ret += temp;
			Cache.add(key,temp);
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

