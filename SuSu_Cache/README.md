## Short describe about this k-v storeage

SuSu_Cache is a **K-V storeage unit**.

The value is **implemented by template**,so you also can consider it as **Object storage unit**.

If you want to use a **tiny or temporary cache**,you can try to use this tool.

## Why did I design this tools?

As we know,If we wan to use cache,the best choice is **redis** or **sqlite**.

But in most of times,I just want to speed up an algorithm by cache.That is a **"lite load scenarios"**.

Is necessary for us to run a redis or sqlite in a "lite load scenarios"? **I think is not necessary**.

So I design this tools for myself.

## How to use this tool?

### 1.singal thread - normal situation

in most situation,you just need to use these codes:
```cplusplus
#include "susu_cache.h"              //head file
    
using susu_tools::cache;        //just use SuSu_Cache
    
cache<class_name> store;        //build your_cache;
    
class_name object;              //construct your object
    
store.add( "key" , object );    //store a [Copy Of Object] into cache
store.add( "key" , &object );   //you also can provide a pointer

class_name ret = store.get( "key" ); //get a [Copy Of Object] from cache

```

Executing the **store.add()** function will not affect the object.

Because the cache will **construct a new object** (construct by copy constructor) and insert it into the cache. 

When you execute **store.get()**,the cache will also return a new object (construct by copy constructor).

If there are some problem about "DEEP COPY" , you should solve it by yourself.

***( In most of times,If your object needs to manipulate the heap (such as MALLOC FREE NEW DELETE) ,you should prepare a Copy Constructor for it. )***


After you execute **add_swap()**,the key and value will be inserted to cache successful,**But the pointer becomes NULL**,just like **std::move**.

In my views,you can follow these tips to check what function you need to use:

**add + get:**every object in cache can be use lot's of times.

**add + get_swap:**every object in cache can be use only once.

**add_swap:**replace "add" when you want to avoid copy constructor.

##How to complie
I provide a test example,You can refer it.
```
make test
```
or
```
g++ main.cpp -o example.bin
./example.bin
```
Before your coding,you should run the test exapmle at first.

##Project details
The SuSu_Cache is composed of list and map.

**map:** to store meta data.

**list:** to store real data.

When you remove a value by key,you just remove the key(meta data) from map.

The value(real data) still stored by list,**but you can't find it**,Because you had lost the meta data.

When you execute some function like **free_data()**,the value will be removed from list.

Maybe the data structure will change in the future, but the overall design will remain the same.
Because I think this design will help us to make some performance optimization
