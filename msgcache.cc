#include <stdint.h>
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


class msgcache : public msgpack::rpc::dispatcher, public boost::noncopyable{
public:
	msgcache(){}
	void dispatch(msgpack::rpc::request req){
		// get the name of method
		const std::string& method = req.method().as<std::string>();
		// call the method
		if(!strcmp(method.c_str(),"set")){
			
		}else if(!strcmp(method.c_str(),"append")){
		}else if(!strcmp(method.c_str(),"get")){
			
		}else if(!strcmp(method.c_str(), "delete")){

		}else if(!strcmp(method.c_str(), "gets")){
		}else if(!strcmp(method.c_str(), "cas")){
		}else if(!strcmp(method.c_str(), "set_multi")){
		}else if(!strcmp(method.c_str(), "get_multi")){
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
	server.run(5);
}



