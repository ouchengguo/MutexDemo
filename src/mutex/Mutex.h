#ifndef Mutex_h
#define Mutex_h
#include <process.h>
#include <assert.h>
#include "Locker.h"

#define WIN 1
#define LINUX 0
#if WIN
#include <windows.h>
#endif

#if LINUX
#include <pthread.h>
#endif

#if LINUX
	typedef pthread_mutex_t PlatformMutex;
#elif WIN
	struct PlatformMutex {
		CRITICAL_SECTION m_internalMutex;
		size_t m_recursionCount;
	};
#else
	typedef void* PlatformMutex;
#endif

class Mutex {
public:
	Mutex();
	~Mutex();

	void lock();
	bool tryLock();
	void unlock();

public:
	PlatformMutex& impl() { return m_mutex; }
private:
	PlatformMutex m_mutex;
};

typedef Locker<Mutex> MutexLocker;

#endif // !Mutex_h