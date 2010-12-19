#ifndef STORAGE_HPP
#define STORAGE_HPP
#include <stdint.h>
#include <stdlib.h>
#include <string>
#include <boost/noncopyable.hpp>
#include <boost/unordered_map.hpp>
#include <msgpack.hpp>
#include <boost/optional.hpp>
#include <mp/sync.h>
#include "rwsync.hpp"
#include "optional_pack.hpp"

struct buff_view {
	// if size == 0  this object is NULL
	const uint32_t size;
	const void* const data;
	buff_view(uint32_t s,void* d)
		:size(s),data(d){}
};
struct buff_view_animated : public buff_view{
	// if size == 0  this object is NULL
	const uint32_t life;
	buff_view_animated(uint32_t s,void* d,uint32_t l)
		:buff_view(s,d),life(l){}
};

struct value_tag{
	std::string buff;
	uint64_t tag;
	explicit value_tag(const std::string& b, uint64_t t=0):buff(b),tag(t){}
	MSGPACK_DEFINE(buff,tag);
};

template <typename T>
struct optional{
	T t;
	bool flag;
	optional():flag(false){}
	explicit optional(const T& _t):t(t),flag(true){}
	
};


class mstorage{
private:
	typedef boost::unordered_map<std::string, value_tag> storage;
	typedef rwsync<storage> synced_storage;
	typedef synced_storage::read_ref storage_read_ref;
	typedef synced_storage::upgrade_ref storage_upgrade_ref;
	typedef synced_storage::write_ref storage_write_ref;
	synced_storage map;

public:
	mstorage(){}
	bool set(const std::string& key, const std::string& value, const uint64_t tag)
	{
		storage_upgrade_ref uplock(map);
		storage_write_ref wlock(uplock);
		wlock->insert(std::make_pair(key,value_tag(value,tag)));
		return true;
	}
	bool append(const std::string& key, const std::string& value, const uint64_t tag)
	{
		storage_upgrade_ref uplock(map);
		const storage::const_iterator it = uplock->find(key);
		if(it != uplock->end()){
			storage_write_ref wlock(uplock);
			wlock->erase(key);
			wlock->insert(std::make_pair(key,value_tag(value,tag)));
			return true;
		}else{
			return false;
		}
	}
	bool add(const std::string& key, const std::string& value, const uint64_t tag)
	{
		storage_upgrade_ref uplock(map);
		const storage::const_iterator it = uplock->find(key);
		if(it == uplock->end()){
			storage_write_ref wlock(uplock);
			wlock->insert(std::make_pair(key,value_tag(value,tag)));
			return true;
		}
		return false;
	}
	boost::optional<const std::string> get(const std::string& key)
	{
		const storage_read_ref rlock(map);
		const storage::const_iterator it = rlock->find(key);
		if(it != rlock->end()){
			return it->second.buff;
		}
		else return NULL;
	}
	boost::optional<const value_tag> gets(const std::string& key)
	{
		storage_read_ref rlock(map);
		const storage::const_iterator it = rlock->find(key);
		if(it != rlock->end()){
			return it->second;
		}
		else return NULL;
	}
	bool del(const std::string& key)
	{
		storage_upgrade_ref uplock(map);
		storage::const_iterator it = uplock->find(key);
		if(it != uplock->end()){
			storage_write_ref wlock(uplock);
			wlock->erase(key);
			return true;
		}
		return false;
	}
	
	bool cas(const std::string& key, const std::string& value, const uint64_t tag, const uint64_t newtag){
		storage_upgrade_ref uplock(map);
		storage::const_iterator it = uplock->find(key);
		if(it != uplock->end() && it->second.tag == tag){
			storage_write_ref wlock(uplock);
			wlock->erase(key);
			wlock->insert(std::make_pair(key, value_tag(value, newtag)));
			return true;
		}
		return false;
	}
};


class storage{
	// slab allocator for buffer
	storage(uint64_t size)
		:size_(size),buffer_(reinterpret_cast<char*>(malloc(size))){}
	bool set(const std::string& key, const std::string& value);
	bool append(const std::string& key, const std::string& value);
	bool add(const std::string& key, const std::string& value);
	boost::optional<const std::string> get(const std::string& key);
	boost::optional<const value_tag> gets(const std::string& key);
	bool del(const std::string& key);
	
private:
	// it must be replaced with fine concurrent hashtable
	typedef boost::unordered_map<std::string, buff_view> index_t;
	typedef mp::sync<index_t>::ref ref_index;
	mp::sync<boost::unordered_map<std::string, buff_view> > index;
	
	const uint64_t& size()const{return size_;}
	const uint64_t size_;
	char* const buffer_;
};

#endif
