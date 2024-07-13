#include "susu_cache.hpp"

namespace susu_tools{

long long susu_cache::get_data_store_size()
{
	return data_store.size();
}
long long susu_cache::get_limit()
{
        return limit;
}

int susu_cache::set_limit(long long limit)
{
        this->limit = limit;
        return SUCCESS;
}

unordered_map<string,data_unit*>& susu_cache::get_data_store()
{
        return data_store;
}

int susu_cache::remove(string& key)
{
	if( find_key(key)==FAIL )       //check if the key is exist
        {
		return KEY_NOT_FOUND;
        }
        else
        {
		auto it = data_store.find(key);
		//(*it).second->flag = false;
                //delete((*it).second->data);     //free the data
                data_store.erase(it);
                return SUCCESS;
        }
}

int susu_cache::remove(string&& key)
{
	if( find_key( key )==FAIL )       //check if the key is exist
        {
		return KEY_NOT_FOUND;
        }
        else
        {
		auto it = data_store.find( std::move(key) );
		//(*it).second->flag = false;
                //delete((*it).second->data);     //free the data
                data_store.erase(it);
                return SUCCESS;
        }
}

        int susu_cache::find_key(string& key)
        {
                if(data_store.find(key) != data_store.end())
                {
                        return SUCCESS;
                }
                else
                {
                        return FAIL;
                }
        }
        //rvalue
        int susu_cache::find_key(string&& key)
        {
                if(data_store.find(std::move(key)) != data_store.end())
                {
                        return SUCCESS;
                }
                else
                {
                        return FAIL;
                }
        }
	 // add a k-v to store
        int susu_cache::add_kv(string& key,void* data)
        {
                //add to data_unit
                data_unit* temp = new data_unit();
                temp->data = data;      //get the pointer
                temp->flag = true;
                data_store.insert(pair<string,data_unit*>(key,temp));

                return SUCCESS;
        }
        //rvalue
        int susu_cache::add_kv(string&& key,void* data)
        {
                //add to data_unit
                data_unit* temp = new data_unit();
                temp->data = data;
                temp->flag = true;
                data_store.insert(pair<string,data_unit*>(std::move(key),temp));

                return SUCCESS;
        }

        //remove old data,insert new data
        int susu_cache::update(string& key,void* data)
        {
                if( find_key(key)==FAIL )       //check if the key is exist
                {
                        return KEY_NOT_FOUND;
                }

                //remove old 
                remove(key);

                return add_kv(key,data);
        }
        //remove old data,insert new data
        int susu_cache::update(string&& key,void* data)
        {
                if( find_key(key)==FAIL )       //check if the key is exist
                {
                        return KEY_NOT_FOUND;
                }

                //remove old 
                remove(key);

                return add_kv(move(key),data);
        }
}//namespace susu_tools
