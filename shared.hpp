#ifndef SHARED_HPP
#define SHARED_HPP

#include <boost/unordered_map.hpp>
#include <mp/sync.h>
#include "objlib.h"
#include "rwsync.hpp"
#include "storage.hpp"

struct shared : public singleton<shared>{
	typedef  mstorage storage_type;
	storage_type data;
	
	
	char* buff;
};


#endif
