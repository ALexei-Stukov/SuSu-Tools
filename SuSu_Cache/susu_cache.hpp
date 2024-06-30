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

class data_store_unit{
public:
	void* data;	// to store the real data 
	bool flag;	// to decide if this data can be read or not
};

class data_meta_unit{
public:
	string name;		// to store the name of data_store_unit
	data_store_unit* location;	// to store the location of data_store_unit
};

/*mutex init;
static void* Singleton = NULL;*/

template<typename T>
class susu_cache{
public:
	//singleton
	/*static susu_cache<T>* get_susu_cache_singleton()
	{
		if(Singleton == NULL)
		{
			init.lock();
			if(Singleton == NULL)
			{
				Singleton = new susu_cache<T>();
			}
			init.unlock();
		}
		return (susu_cache<T>*)Singleton;
	}*/

	//check a key is exist or not;
	int find_key(string key)
	{
		if(meta_store.find(key) != meta_store.end())
		{
			return SUCCESS;
		}
		else
		{
			return FAIL;
		}
	}
	int add_kv(string key,T * data)
	{
		//add to data store
		data_store_unit* temp_data = new data_store_unit();
		temp_data->data = (void*)data;	//get the pointer
		temp_data->flag = true;
		data_store.insert(data_store.end(),temp_data);
	
		//add to meta store
		data_meta_unit* temp_meta = new data_meta_unit();
		temp_meta->name = key;
		temp_meta->location = temp_data;
		meta_store.insert(pair<string,data_meta_unit*>(key,temp_meta));

		return SUCCESS;
	}	

	//insert to list tail forever
	int store(string key,T * data)
	{
		if( find_key(key)!=FAIL )	//check if the key is exist
		{
			return KEY_HAD_EXIST;
		}
		else if( get_data_store_size() >= limit )	//check if data_store_unit is too much
		{
			free_data();	//try to free the unless data_store_unit at first
			if( get_data_store_size() >= limit )	//check again
			{
				return  OUT_OF_LIMIT;
			}
		}
		return add_kv(key,data);
	}
	
	//remove old data,insert new data
	int update(string key,T* data)
	{
		if( find_key(key)==FAIL )	//check if the key is exist
		{
			return KEY_NOT_FOUND;
		}

		//remove old 
		remove(key);
		
		return add_kv(key,data);
	}
	
        // R-value
	int add(string key,T && object)
	{
		T* input = new T(object);
		return store(key,input);
	}
	
	// pointer
	int add(string key,T * object)
	{
		T* input = new T(*object);
		return store(key,input);
	}

	// L-value
	int add(string key,T & object)
	{
		T* input = new T(object);
		return store(key,input);
	}

	// swap pointer into cache
	int add_swap(string key,T** object)
	{
		T* input = *object;
		*object = NULL;
		return store(key,input);
	}

	T* get(string key)
	{
		if( find_key(key)==FAIL )	//check if the key is exist
		{
			return NULL;
		}
		else
		{
			return (T*)(meta_store[key]->location->data);	//return a Cpoy Of Object
		}
	}

	T* get_swap(string key)
	{
		if( find_key(key)==FAIL )	//check if the key is exist
		{
			return NULL;
		}
		else
		{

			T* ret = (T*)(meta_store[key]->location->data);
			remove(key);
			return ret;	//swap the pointer and remove in cache
		}
	}

	void print()//print all the data pointer for debug
	{
		for(auto loop = data_store.begin();loop != data_store.end();loop++)
		{
			if( (*loop)->flag == true )
			{
				std::cout<<(*loop)->data<<"\n";
			}
		}
	}

	int remove(string key)
	{
		if( find_key(key)==FAIL )	//check if the key is exist
		{
			return KEY_NOT_FOUND;
		}
		else
		{
			auto it = meta_store.find(key);
			(*it).second->location->flag = false;
			meta_store.erase(it);
			return SUCCESS;
		}
	}

	int free_data()
	{
		//get_mutex();
		for(auto loop = data_store.begin();loop != data_store.end();) 
		{
			if( (*loop)->flag == false )
			{
				loop = data_store.erase(loop);
				free((*loop)->data);
			}
			else
			{
				loop++;
			}
       		}
		//release_mutex();

		return SUCCESS;
	}

	long long get_data_store_size()
	{
		return data_store.size();
	}

	long long get_meta_store_size()
	{
		return meta_store.size();
	}

	long long get_limit()
	{
		return limit;
	}

	int set_limit(long long limit)
	{
		this->limit = limit;
		return SUCCESS;
	}

	unordered_map<string,data_meta_unit*>& get_meta_store()
	{
		return meta_store;
	}
	/*
	int get_mutex()
	{
		rw_mutex.lock();
		return SUCCESS;
	}

	int release_mutex()
	{
		rw_mutex.unlock();
		return SUCCESS;
	}
	
	int MT_add(string key,T* object)
	{
		get_mutex();
		int ret = add(key,object);
		release_mutex();
		return ret;
	}

	int MT_add(string key,T& object)
	{
		get_mutex();
		int ret = add(key,object);
		release_mutex();
		return ret;
	}

	int MT_add_swap(string key,T** object)
	{
		get_mutex();
		int ret = add_swap(key,object);
		release_mutex();
		return ret;
	}
	
	T* MT_get_swap(string key)
	{
		if( find_key(key)==FAIL )	//check if the key is exist
		{
			return NULL;
		}
		else
		{
			get_mutex();
			T* ret = (T*)(meta_store[key]->location->data);
			remove(key);
			release_mutex();
			return ret;
		}
	}*/

private:
	list<data_store_unit*> data_store;
	//map<string,data_meta_unit*> meta_store;
	unordered_map<string,data_meta_unit*> meta_store;

	long long limit = 1024*1024*1024;
	//mutex rw_mutex;
};


}//namespace susu_tools
#endif

