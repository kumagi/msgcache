#ifndef RWSYNC_HPP
#define RWSYNC_HPP

#include <boost/thread.hpp>
#include <boost/noncopyable.hpp>

template <typename T>
class rwsync{
	T m_obj;
	typedef boost::shared_mutex smutex;
	smutex lock;
	friend class read_ref;
	friend class upgrade_ref;
	friend class write_ref;
public:
	class read_ref : public boost::noncopyable{
		boost::shared_lock<smutex> m_lock;
		const rwsync<T>* const m_ref;
	public:
		read_ref(rwsync& mutex):m_lock(mutex.lock),m_ref(&mutex){}
		const T& operator*() const { return m_ref->m_obj; }
		const T* operator->() const { return &operator*(); }
	};
	class write_ref;
	class upgrade_ref : public boost::noncopyable{
		friend class write_ref;
		boost::upgrade_lock<smutex> m_lock;
		rwsync<T>* const m_ref;
	public:
		upgrade_ref(rwsync& mutex):m_lock(mutex.lock),m_ref(&mutex){}
		const T& operator*() const { return m_ref->m_obj; }
		const T* operator->() const { return &operator*(); }
	};
	class write_ref : public boost::noncopyable{
		boost::upgrade_to_unique_lock<smutex> m_lock;
		rwsync<T>* const m_ref;
	public:
		write_ref(upgrade_ref& lock):m_lock(lock.m_lock),m_ref(lock.m_ref){}
		T& operator*() { return m_ref->m_obj; }
		T* operator->() { return &operator*(); }
		const T& operator*() const { return m_ref->m_obj; }
		const T* operator->() const { return &operator*(); }
	};

};
#endif
