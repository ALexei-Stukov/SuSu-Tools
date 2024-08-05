## Short describe about this k-v storeage

[Introduction of SuSu-Tools](../README.md)

SuSu_Cache is a **K-V storeage unit**.

The value is **implemented by template**,so you also can consider it as **Object storage unit**.

If you want to use a **tiny or temporary cache**,you can try to use this tool.

## Key points 
**Performance is not very high.**

**Useful in lite load scenarios.**

**Make by template.**

**Not thread-safe.**

## Why did I design this tools?

As we know,If we wan to use cache,the best choice is **redis** or **sqlite**.

But in most of times,I just want to speed up an algorithm by cache.That is a **"lite load scenarios"**.

Is necessary for us to run a redis or sqlite in a "lite load scenarios"? **I think is not necessary**.

So I design this tools for myself.

## How to use this tool?

just like this:

```cpp

#include "susu_cache.hpp"
#include <iostream>

using namespace std;
using namespace susu_tools;

int main()
{
    susu_cache cache;

    string key = "111";
    cache.add(key,5);
    
    if(SUCCESS == cache.find("111"))
    {	
        cout<<"when the key=111,the value is:"<<*(cache.get<int>("111"))<<endl;
    }
    
    return 0;
}

```
## The KEY POINT of functions:

**constructor:** We don't need to specify the type of value in constructor.Because I use templates for functions.

```cpp
class susu_cache{
public:

	template<class T>
	int add(string& key,T && object)	//	object is Rvalue-ref
	{
                //add a key-value into cache
	}
}
```
This may not be the best design, but it is very convenient.

## The Tpye of KEY and VALUE
These are all the important codes:

```cpp
	template<class T>
	int add(string & key,T && value);	//add a key-value into the cache 
	template<class T>
	int add(string && key,T && value);	//add a key-value into the cache 

	template<class T>
	T* get(string & key);				//get always return the copy of object or nullptr
	template<class T>
	T* get(string && key);				//get always return the copy of object or nullptr

	template<class T>
	int update(string & key,T && value);	//remove old k-v and insert new k-v
	template<class T>
	int update(string && key,T && value);	//remove old k-v and insert new k-v
    
    int find(string & key);			//check if the data_store have such a key
    int find(string && key);		//check if the data_store have such a key

    int remove(string & key);		//remove a key-value,this function will delete the value. 
    int remove(string && key);		//remove a key-value,this function will delete the value.
``` 

For **key** and **value**, we can use left-values ​​and right-values.

The difference between key and value is:

The **value** is made by template,So it's easy to process left-values and right-values in 1 function.

But **key** must be string,it can't process left-values and right-values in 1 function,So I had to build 2 functions to process left-values and right-values.

## The details of functions:

**add:** This function will return:

```cpp
    OUT_OF_LIMIT;       //when the count of data_store is out of limit.add failed.
	
    KEY_HAD_EXIST;      //the key had exist.add failed.
	
    SUCCESS;     //add success.
```

**find:** This function will return:

```cpp

    KEY_NOT_FOUND;      //no such key in data_store
	
    SUCCESS;     //find the key success.
```

**get:** This function will return:

```cpp

    nullptr;      //no such key in data_store,return nullptr
	
    T* The_pointer_of_new_object;     //return a copy of object by pointer.
```

**remove:** This function will return:

```cpp
    KEY_NOT_FOUND;      //no such key in data_store,can't remove anything.
	
    SUCCESS;     //remove successfully.
```

**update:** This function will return:

```cpp
    KEY_NOT_FOUND;      //no such key in data_store,can't update anything.
	
    SUCCESS;     //update successfully.
```