#ifndef SUSU_CACHE
#define SUSU_CACHE

#define SUCCESS 0
#define FAIL 1
#define OUT_OF_LIMIT 2
#define NULL_PTR 4
#define KEY_NOT_FOUND 8
#define KEY_HAD_EXIST 16

#include <list>
#include <iostream>
//#include <map>
#include <unordered_map>
#include <string>
//#include <mutex>
#include <queue>

using std::string;
using std::list;
//using std::map;
using std::unordered_map;
using std::pair;
//using std::mutex;
using std::queue;

namespace susu_tools{

class data_unit{
public:
	void* data;	// to store the real data 
	bool flag;	// to decide if this data can be read or not
};

class susu_cache{
public:
	int find_key(string& key);	// check a key is exist or not;
	int find_key(string&& key);	// rvalue-ref

	int add_kv(string& key,void* data);	// add a k-v to store
	int add_kv(string&& key,void* data);	// rvalue-ref

	int update(string& key,void* data);	// remove old data,insert new data
	int update(string&& key,void* data);	// rvalue-ref
		
	int remove(string& key);	// remove old data,insert new data
	int remove(string&& key);	//rvalue-ref

	//----------------------------------------------------------
	template<typename T>
	int add(string& key,T && object)	//	object is Rvalue-ref
	{
		T* input = new T(move(object));
		return add_kv(key,input);
	}
	
	template<typename T>
	int add(string& key,T * object)	//	pointer
	{
		T* input = new T(*object);
		return add_kv(key,input);
	}

	template<typename T>
	int add(string& key,T & object)	//	object is Lvalue-ref
	{
		T* input = new T(object);
		return add_kv(key,input);
	}

	template<typename T>
	int add(string&& key,T && object)	//rvalue-ref
	{
		T* input = new T(move(object));
		return add_kv(std::move(key),input);
	}
	
	template<typename T>
	int add(string&& key,T * object)	//rvalue-ref
	{
		T* input = new T(*object);
		return add_kv(std::move(key),input);
	}

	template<typename T>
	int add(string&& key,T & object)	//rvalue-ref
	{
		T* input = new T(object);
		return add_kv(std::move(key),input);
	}

	
	
	template<typename T>
	T* get(string& key)
	{
		if( find_key(key)==FAIL )	//check if the key is exist
		{
			return NULL;
		}
		else
		{
			return (T*)(data_store[key]->data);	//return a Cpoy Of Object
		}
	}

	template<typename T>
	T* get(string&& key)
	{
		if( find_key(std::move(key))==FAIL )	//check if the key is exist
		{
			return NULL;
		}
		else
		{
			return (T*)(data_store[key]->data);	//return a Cpoy Of Object
		}
	}	
	//----------------------------------------------------------
	
	long long get_data_store_size();
	
	long long get_limit();
	
	int set_limit(long long limit);

	unordered_map<string,data_unit*>& get_data_store();

private:
	unordered_map<string,data_unit*> data_store;
	long long limit = 1024*1024*1024;
};


}//namespace susu_tools
#endif

