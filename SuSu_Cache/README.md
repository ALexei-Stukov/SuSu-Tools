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
using namespace susu_tools;

int main()
{
    susu_cache< Example  >  cache;    // the key type is string, the value type is xxx

    string key;

    Example value;
    
    //execute by left-value
    cache.add(key,value);
    cache.find(key);
    cache.update(key,value);
    cache.remove(key);
    
    //execute by right-value
    cache.add( move(key) , move(value) );
    cache.find( move(key) );
    cache.update( move(key) , move(value) );
    cache.remove( move(key) );

    //execute by pointer
    cache.add( &key , &value);
    cache.find( &key );
    cache.update( &key , &value);
    cache.remove( &key );
}

```
## The difference of executing type

**execute by left-value:** The object will build by Copy Constructor.

watch out,Maybe Copy Constructor will do some bad on your heap-area.

**execute by right-value:** The object will build by Move Constructor.

watch out,It's easy to make a nullptr when using the move().

**execute by left-value:** Almost equal to [execute by right-value].

## Something about design

In old design,the template was used like this:

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

It's easy to use,but sometimes it can also leading people to confusion.

So I fix the design like this:

```cpp

template<class T>
class susu_cache{
public:
	int add(string& key,T && object)	//	object is Rvalue-ref
	{
        //add a key-value into cache
	}
}
```
We must specify the type of key now.