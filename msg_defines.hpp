#ifndef MSG_DEFINES_HPP
#define MSG_DEFINES_HPP

#include <msgpack.hpp>
#include "msgpack_macro.h"
RPC_OPERATION2(set, std::string, key, std::string,value);
RPC_OPERATION2(add, std::string, key, std::string,value);
RPC_OPERATION1(get, std::string, key);
RPC_OPERATION2(append, std::string, key, std::string,value);
RPC_OPERATION3(gets, std::string, key, std::string, value, uint64_t, tag);
RPC_OPERATION1(del, std::string, key);
RPC_OPERATION4(cas, std::string, key, std::string, value, uint64_t, tag, uint64_t,newtag);


#endif
