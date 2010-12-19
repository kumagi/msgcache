#ifndef MSGPACK_MACRO_H
#define MSGPACK_MACRO_H

#define RPC_OPERATION1(NAME, type1, arg1)																\
	namespace msg{ struct NAME{ type1 arg1;																\
		NAME(const type1& arg1_)																						\
			:arg1(arg1_){}																										\
		NAME(const msgpack::object& obj){																		\
			obj.via.array.ptr[0].convert(&arg1);															\
		}																																		\
		MSGPACK_DEFINE(arg1);																								\
	};																																		\
	std::ostream& operator<<(std::ostream& ost, const NAME& obj){\
		ost << "  " #arg1 ":" << obj.arg1;\
		return ost;								 \
	}\
	}																																			\
	namespace logic{																											\
	template<typename request, typename server> void NAME(request* obj, server* sv); \
	}

#define RPC_OPERATION2(NAME, type1, arg1, type2, arg2)									\
	namespace msg{ struct NAME{ type1 arg1; type2 arg2;										\
		NAME(const type1& arg1##_, const type2& arg2##_)										\
			:arg1(arg1##_),arg2(arg2##_){}																		\
		NAME(const msgpack::object& obj){																		\
			obj.via.array.ptr[0].convert(&arg1);															\
			obj.via.array.ptr[1].convert(&arg2);															\
		}																																		\
		MSGPACK_DEFINE(arg1,arg2);																					\
	};																																		\
	std::ostream& operator<<(std::ostream& ost, const NAME& obj){	\
		ost << "  " #arg1 ":" << obj.arg1 << "  " #arg2 ":" << obj.arg2;\
		return ost;								 \
	}																																			\
	}																																			\
	namespace logic{																											\
	template<typename request,typename server> void NAME(request* obj, server* sv); \
	}

#define RPC_OPERATION3(NAME, type1, arg1, type2, arg2, type3, arg3)			\
	namespace msg{ struct NAME{ type1 arg1; type2 arg2; type3 arg3;				\
		NAME(const type1& arg1##_, const type2& arg2##_, const type3& arg3##_) \
			:arg1(arg1##_),arg2(arg2##_),arg3(arg3##_){}											\
		NAME(const msgpack::object& obj){																		\
			obj.via.array.ptr[0].convert(&arg1);															\
			obj.via.array.ptr[1].convert(&arg2);															\
			obj.via.array.ptr[2].convert(&arg3);															\
		}																																		\
		MSGPACK_DEFINE(arg1,arg2,arg3);																			\
	};																																		\
	std::ostream& operator<<(std::ostream& ost, const NAME& obj){	\
		ost << "  " #arg1 ":" << obj.arg1 << "  " #arg2 ":" << obj.arg2 << "  " #arg3 ":" << obj.arg3; \
		return ost;								 \
	}																																			\
	}																																			\
	namespace logic{																											\
	template<typename request,typename server> void NAME(request* obj, server* sv); \
	}

#define RPC_OPERATION4(NAME, type1, arg1, type2, arg2, type3, arg3,type4,arg4) \
	namespace msg{ struct NAME{ type1 arg1; type2 arg2; type3 arg3; type4 arg4;\
		NAME(const type1& arg1##_, const type2& arg2##_, const type3& arg3##_, const type4& arg4##_) \
			:arg1(arg1##_),arg2(arg2##_),arg3(arg3##_),arg4(arg4##_){}\
		NAME(const msgpack::object& obj){\
			obj.via.array.ptr[0].convert(&arg1);\
			obj.via.array.ptr[1].convert(&arg2);\
			obj.via.array.ptr[2].convert(&arg3);\
			obj.via.array.ptr[3].convert(&arg4);\
		}\
		MSGPACK_DEFINE(arg1,arg2,arg3,arg4);\
	};\
	std::ostream& operator<<(std::ostream& ost, const NAME& obj){	\
		ost << "  " #arg1 ":" << obj.arg1 << "  " #arg2 ":" << obj.arg2 << "  " #arg3 ":" << obj.arg3 << "  " #arg4 ":" << obj.arg4; \
		return ost;								 \
	}																																			\
	}																							\
	namespace logic{\
	template<typename request, typename server> void NAME(request* obj, server* sv); \
	}

#define RPC_OPERATION5(NAME, type1, arg1, type2, arg2, type3, arg3,type4, arg4, type5, arg5) \
	namespace msg{ struct NAME{ type1 arg1; type2 arg2; type3 arg3; type4 arg4; type5 arg5; \
		NAME(const type1& arg1##_, const type2& arg2##_, const type3& arg3##_, const type4& arg4##_, const type5& arg5##_) \
			:arg1(arg1##_),arg2(arg2##_),arg3(arg3##_),arg4(arg4##_),arg5(arg5##_){} \
		NAME(const msgpack::object& obj){\
			obj.via.array.ptr[0].convert(&arg1);\
			obj.via.array.ptr[1].convert(&arg2);\
			obj.via.array.ptr[2].convert(&arg3);\
			obj.via.array.ptr[3].convert(&arg4);\
			obj.via.array.ptr[4].convert(&arg5);\
		}\
		MSGPACK_DEFINE(arg1,arg2,arg3,arg4,arg5);\
	};\
	std::ostream& operator<<(std::ostream& ost, const NAME& obj){	\
		ost << "  " #arg1 ":" << obj.arg1 << "  " #arg2 ":" << obj.arg2 << "  " #arg3 ":" << obj.arg3 << "  " #arg4 ":" << obj.arg4  << "  " #arg5 ":" << obj.arg5; \
		return ost;								 \
	}\
	}																							\
	namespace logic{\
	template<typename request,typename server> void NAME(request* obj, server* sv); \
	}

#endif






