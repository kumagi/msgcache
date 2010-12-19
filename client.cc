#include <stdint.h>
#include <cclog/cclog.h>
#include <cclog/cclog_tty.h>
#include <msgpack/rpc/client.h>

#include <string>
#include <vector>
#include <map>
#include <stdlib.h>

// external lib
#include <boost/shared_ptr.hpp>
#include <boost/scoped_ptr.hpp>
#include <boost/program_options.hpp>
#include <boost/noncopyable.hpp>
#include <boost/timer.hpp>

#include <mp/wavy.h>
#include <mp/sync.h>
#include <cclog/cclog.h>
#include <cclog/cclog_tty.h>

#include "interface.h"
#include "shared.hpp"
#include "msgpack_macro.h"

#include "boost/atomic.hpp"
#include <boost/lexical_cast.hpp>
#include "msg_defines.hpp"

void getter(const int cnt, msgpack::rpc::loop* lo){
	msgpack::rpc::client c("127.0.0.1", 11311, *lo);
	for(int i=0; i<cnt; i++){
		const uint32_t r = rand()%(i+1);
		boost::optional<std::string> t = c.call<std::string>
			("get", "k" +
			 boost::lexical_cast<std::string>(r))
			.get<boost::optional<std::string> >();
		if(t){ std::cout << "got " << *t << std::endl;}
		else { std::cout << "failed to get " << i;}
	}
}

struct cmdline: public boost::noncopyable{
	const std::string interface;
	const uint32_t threads;
	const uint16_t port;
	const uint64_t memory;
	const uint32_t verbose;
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

	
	msgpack::rpc::client c("127.0.0.1", 11311);
	std::list<msgpack::rpc::future > rets;
	for(int i=0; i<100000; i++){
		rets.push_back(c.call<std::string,std::string>
			("set", "k" + boost::lexical_cast<std::string>(i), 
				"v" + boost::lexical_cast<std::string>(i)));
	}

	boost::timer time;
	boost::thread_group tg;
	msgpack::rpc::loop lo;
	const int reqs = 10000000;
	const int threads = 8;
	for (int i = 0; i < threads; ++i) {
		tg.create_thread(boost::bind(&getter, reqs/threads, &lo)); // create_thread()でrun()を別スレッドで実行
	}
	tg.join_all();
	const int elapse = time.elapsed();
	printf("time: %d    %f qps \n", elapse, static_cast<double>(reqs)/elapse);
}
