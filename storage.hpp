#ifndef STORAGE_HPP
#define STORAGE_HPP
#include <stdint.h>
#include <stdlib.h>
#include <string>
#include <boost/noncopyable.hpp>
#include <boost/unordered_map.hpp>
#include <mp/sync.h>


struct buff_view : public boost::noncopyable{
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

class storage{
	// slab allocator for buffer
	storage(uint64_t size)
		:size_(size),buffer_(reinterpret_cast<char*>(malloc(size)))
	{}
	buff_view* set(const std::string& key, const std::string& value);
	buff_view* append(const std::string& key, const std::string& value);
	buff_view* add(const std::string& key, const std::string& value);
	buff_view* put(const std::string& key, const std::string& value);
	buff_view* get(const std::string& key);

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
