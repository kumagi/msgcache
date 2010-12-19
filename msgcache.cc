#include <stdint.h>
#include <cclog/cclog.h>
#include <cclog/cclog_tty.h>
#include <msgpack/rpc/server.h>
#include <msgpack/rpc/client.h>

#include <string>
#include <vector>
#include <map>

// external lib
#include <boost/shared_ptr.hpp>
#include <boost/scoped_ptr.hpp>
#include <boost/program_options.hpp>
#include <boost/noncopyable.hpp>

#include <mp/wavy.h>
#include <mp/sync.h>

#include "interface.h"
#include "shared.hpp"
#include "msgpack_macro.h"

#include "boost/atomic.hpp"
#include "msg_defines.hpp"

class msgcache : public msgpack::rpc::dispatcher, public boost::noncopyable{
	boost::atomic_uint64_t m_counter;
public:
	msgcache():m_counter(1){}
	
	void dispatch(msgpack::rpc::request req){
		// get the name of method
		const std::string& method = req.method().as<std::string>();
		// call the method
		shared::storage_type& s = shared::instance().data;
		if(!strcmp(method.c_str(),"set")){
			const msg::set arg(req.params());
			req.result(s.set(arg.key, arg.value,
					m_counter.fetch_add(1,boost::memory_order_relaxed)));
		}else if(!strcmp(method.c_str(),"append")){
			const msg::append arg(req.params());
			req.result(s.append(arg.key, arg.value, 
					m_counter.fetch_add(1, boost::memory_order_relaxed)));
		}else if(!strcmp(method.c_str(),"add")){
			const msg::add arg(req.params());
			req.result(s.add(arg.key, arg.value,
					m_counter.fetch_add(1, boost::memory_order_relaxed)));
		}else if(!strcmp(method.c_str(),"get")){
			const msg::get arg(req.params());
			const boost::optional<const std::string> res(s.get(arg.key));
			if(res) { req.result<const std::string>(*res);}
			else { req.result(NULL); }
		}else if(!strcmp(method.c_str(), "delete")){
			const msg::del arg(req.params());
			req.result(s.del(arg.key));
		}else if(!strcmp(method.c_str(), "gets")){
			const msg::gets arg(req.params());
			const boost::optional<value_tag> res(s.gets(arg.key));
			if(res) { req.result<value_tag>(*res);}
			else{ req.result(NULL);}
		}else if(!strcmp(method.c_str(), "cas")){
			const msg::cas arg(req.params());
			req.result(s.cas(arg.key, arg.value, arg.tag,
					m_counter.fetch_add(1,boost::memory_order_relaxed)));
		}else if(!strcmp(method.c_str(), "set_multi")){
		}else if(!strcmp(method.c_str(), "get_multi")){
		}else{
			std::cerr << "invalid request" << std::endl;
		}
	}
};

struct cmdline: public boost::noncopyable{
	std::string interface;
	uint32_t threads;
	uint16_t port;
	uint64_t memory;
	uint32_t verbose;
	cmdline(const std::string& i, const uint64_t t, const uint16_t p, const uint64_t m, const uint32_t v)
		:interface(i),threads(t),port(p),memory(m),verbose(v){}
private:
	cmdline();
};

int main(int argc, char** argv){
	cclog::reset(new cclog_tty(cclog::TRACE, std::cout));
	boost::scoped_ptr<const cmdline> setting;
	{// settings
		boost::program_options::options_description opt("options");
		std::string interface;
		uint32_t threads;
		uint16_t port;
		uint64_t memory;
		uint32_t verbose;
		opt.add_options()
			("help,h", "view help")
			("interface,i",boost::program_options::value<std::string>
			 (&interface)->default_value("any"), "my interface")
			("interface,i",boost::program_options::value<uint32_t>
			 (&threads)->default_value(2), "number of threads")
			("port,p",boost::program_options::value<uint16_t>
			 (&port)->default_value(11311), "my port number")
			("address,a",boost::program_options::value<uint64_t>
			 (&memory)->default_value(1024*1024*1024), "max memory usage")
			("verbose,v",boost::program_options::value<uint32_t>
			 (&verbose)->default_value(0), "verbose level");
	
		boost::program_options::variables_map vm;
		store(parse_command_line(argc,argv,opt), vm);
		notify(vm);
		if(vm.count("help")){
			std::cerr << opt << std::endl;
			exit(0);
		}
		shared::instance().buff = reinterpret_cast<char*>(malloc(memory));
		setting.reset(new cmdline(interface,threads,port,memory,verbose));
	}
	
	msgpack::rpc::server server;
	msgcache host; // main callback

	server.serve(&host);
	if(setting->interface == "any"){
		server.listen("0.0.0.0", setting->port);
	}else{
		server.listen(get_myip(setting->interface).c_str(), setting->port);
	}
	server.run(2);
}



