#ifndef Mutex_h
#define Mutex_h
#include <assert.h>
#include "Locker.h"

#define WIN 0
#define LINUX 1
#if WIN
#include <process.h>
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