#ifndef SHARED_HPP
#define SHARED_HPP

#include <boost/unordered_map.hpp>

#include <mp/sync.h>
#include "objlib.h"
#include "storage.hpp"


struct shared : public singleton<shared>{
	char* buff;
	typedef boost::unordered_map<std::string, fast_buff*> storage;
	typedef mp::sync<storage> synced_storage;
	typedef synced_storage::ref ref_storage;
	synced_storage map;
};


#endif
