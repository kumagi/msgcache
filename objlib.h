#ifndef OBJLIB_H
#define OBJLIB_H
#include <boost/noncopyable.hpp>

// singleton
template<typename Derived>
class singleton : private boost::noncopyable {
public:
  static Derived& instance() { static Derived the_inst; return the_inst; }
protected:
  singleton(){}
  ~singleton(){}
};


#define MSGPACK_ENUM_DEFINE(X) namespace msgpack{\
	inline X& operator>> (object o, X& v){ \
	v = (X)type::detail::convert_integer<int>(o); return v;\
	}\
	template <typename Stream>\
	inline packer<Stream>& operator<< (packer<Stream>& o, const X& v)\
	{ o.pack_int(v); return o; }\
	}

#endif
