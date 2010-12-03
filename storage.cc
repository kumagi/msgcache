#include "storage.hpp"
#define DEBUG_MODE
#ifdef DEBUG_MODE
#define DEBUG_OUT(...) fprintf(stderr,__VA_ARGS__)
#define DEBUG(x) x
#define REACH(x) fprintf(stderr,"reach line:%d %s",__LINE__, x)
#define BLOCK(x) debugmode::block DEBUGBLOCK_(x)
#include <iostream>
namespace debugmode{
class block :boost::noncopyable{
	mutable std::string message;
public:
	block(const std::string& msg):message(msg){
		std::cerr << "start[" << message << "]";
	}
	~block(){
		std::cerr << "end[" << message << "]" << std::endl;
	}
};
}
#else
#define NDEBUG
#define DEBUG_OUT(...) 
#define DEBUG(...)
#define REACH(...)
#define BLOCK(...)
#endif//DEBUG_MODE

fast_buff* 
storage::set(const std::string& key, const std::string& value){
	REACH("set");
	{
		ref_index ri(index);
		index_t::iterator it = ri->find(key);
		
	}
}

fast_buff* 
storage::append(const std::string& key, const std::string& value){
	REACH("append");
}

fast_buff* 
storage::add(const std::string& key, const std::string& value){
	REACH("put");
}

fast_buff* 
storage::put(const std::string& key, const std::string& value){
	REACH("put");
}

fast_buff* 
storage::get(const std::string& key){
	REACH("get");
}
