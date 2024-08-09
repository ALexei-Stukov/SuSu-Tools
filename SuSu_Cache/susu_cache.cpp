#include "susu_cache.hpp"

using namespace susu_tools;

long long susu_cache::get_current_count()
{
	return data_store.size();
}

long long susu_cache::get_count_limit()
{
        return count_limit;
}

int susu_cache::set_count_limit(long long limit)
{
        this->count_limit = limit;
        return SUCCESS;
}

unordered_map<string,data_unit>& susu_cache::get_data_store()
{
        return data_store;
}

int susu_cache::find(string & key)
{
	auto it = data_store.find(key);
	if (it == data_store.end())
	{
		return KEY_NOT_FOUND;
	}
	else
	{
		return SUCCESS;
	}
}
int susu_cache::find(string && key)
{
	auto it = data_store.find(key);
	if (it == data_store.end())
	{
		return KEY_NOT_FOUND;
	}
	else
	{
		return SUCCESS;
	}
}

int susu_cache::remove(string & key)
{
	if( find(key) == KEY_NOT_FOUND )
	{
		return KEY_NOT_FOUND;
	}
	else
	{
		data_store.erase( key );
		return SUCCESS;
	}
}

int susu_cache::remove(string && key)
{
	if( find(key) == KEY_NOT_FOUND )
	{
		return KEY_NOT_FOUND;
	}
	else
	{
		data_store.erase( key );
		return SUCCESS;
	}
}


int susu_cache::remove_all()
{
	/*
	auto it = data_store.begin();
	while(data_store.size() > 0)
	{
		data_store.erase(it);
	}*/
	data_store.clear();
	return SUCCESS;
}