#ifndef OPTIONAL_PACK_HPP
#define OPTIONAL_PACK_HPP


#include <msgpack/object.hpp>
#include <boost/optional.hpp>

namespace msgpack {

template <typename T>
inline boost::optional<T>& operator>> (object o, boost::optional<T>& v)
{
	if(o.type == type::NIL) { v=boost::optional<T>(); return v;}
	else {
		T value;
		o >> value;
		v = value;
		return v;
	}

}

template <typename Stream, typename T>
inline packer<Stream>& operator<< (packer<Stream>& o, const boost::optional<T>& v)
{
	if(!v){o << type::nil(); return o;}
	else { o << *v; return o;}
}

}  // namespace msgpack

#endif
