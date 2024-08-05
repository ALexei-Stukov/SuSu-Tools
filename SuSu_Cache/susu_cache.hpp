#ifndef SUSU_CACHE
#define SUSU_CACHE

#define SUCCESS 0
#define FAIL 1
#define OUT_OF_LIMIT 2
#define NULL_PTR 4
#define KEY_NOT_FOUND 8
#define KEY_HAD_EXIST 16


#include <iostream>
#include <unordered_map>
#include <string>
#include <queue>

using std::string;
using std::unordered_map;
using std::is_lvalue_reference;
using std::forward;
using std::pair;
using std::move;

namespace susu_tools{

class data_unit{
public:
	void* data;	// to store the real data 
	bool flag;	// to decide if this data can be read or not
};


class susu_cache{
public:

	template<class T>
	int add(string & key,T && value);	//add a key-value into the cache 

	template<class T>
	T* get(string & key);				//get always return the copy of object
	template<class T>
	T* get(string && key);				//get always return the copy of object

	template<class T>
	int update(string & key,T && value);	//remove old k-v and insert new k-v
	template<class T>
	int update(string && key,T && value);	//remove old k-v and insert new k-v

    int find(string & key);			//check if the data_store have such a key
	int find(string && key);			//check if the data_store have such a key

    int remove(string & key);		//remove a key-value,this function will delete the value. 
	int remove(string && key);		//remove a key-value,this function will delete the value. 

	long long get_current_count();
	long long get_count_limit();
	int set_count_limit(long long limit);

	unordered_map<string,data_unit>& get_data_store();

private:
	template<class T>
	int add_kv(string && key,T & value);

	template<class T>
	int add_kv(string && key,T && value);

	unordered_map<string,data_unit> data_store;
	long long count_limit = 1024*1024*1024;	//the count limit of k-v
};

template<class T>
int susu_cache::add(string & key,T && value)
{
	return add_kv(move(key),forward<T>(value));	//perfect forward
}

template<class T>
int susu_cache::add_kv(string && key,T & value)
{
	if( get_current_count()+1 > get_count_limit())
	{
		return OUT_OF_LIMIT;
	}

	if( find(key) == true )
	{
		return KEY_HAD_EXIST;
	}

	T* data = new T();
	*data = value;
	//std::cout<<"the key is"<<key<<"the value is"<<data<<std::endl;
	data_unit temp = {(void *)(data),true};
	data_store.insert(pair<string,data_unit>(move(key),temp));
	return SUCCESS;
}

template<class T>
int susu_cache::add_kv(string&& key,T && value)
{
	if( get_current_count()+1 > get_count_limit())
	{
		return OUT_OF_LIMIT;
	}

	if( find(key) == true )
	{
		return KEY_HAD_EXIST;
	}

	T* data = new T();
	*data = move(value);
	//std::cout<<"the key is"<<key<<"the value is"<<data<<std::endl;
	data_unit temp = {(void *)(data),true};
	data_store.insert(pair<string,data_unit>(move(key),temp));
	return SUCCESS;
}

template<class T>
int susu_cache::update(string & key,T && value)
{
	if( find(key) == KEY_NOT_FOUND )
	{
		return KEY_NOT_FOUND;
	}
	else
	{
		data_store.erase( key );
		return add(key,forward<T>(value));
	}
}

template<class T>
int susu_cache::update(string && key,T && value)
{
	if( find(key) == KEY_NOT_FOUND )
	{
		return KEY_NOT_FOUND;
	}
	else
	{
		data_store.erase( key );
		return add(key,forward<T>(value));
	}
}

template<class T>
T* susu_cache::get(string & key)				//get always return the copy of object
{
	if( find(key) == KEY_NOT_FOUND )
	{
		return nullptr;
	}
	else
	{
		T* ret = new T();
		T* temp = (T*)(data_store[key].data);
		*ret = *temp;
		return ret;
	}
}

template<class T>
T* susu_cache::get(string && key)				//get always return the copy of object
{
	if( find(key) == KEY_NOT_FOUND )
	{
		return nullptr;
	}
	else
	{
		T* ret = new T();
		T* temp = (T*)(data_store[key].data);
		*ret = *temp;
		return ret;
	}
}


}//namespace susu_tools
#endif