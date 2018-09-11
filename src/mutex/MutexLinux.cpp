#include "Mutex.h"
#include <stdio.h>

#if LINUX
Mutex::Mutex()
{
	pthread_mutexattr_t attr;
	pthread_mutexattr_init(&attr);
	pthread_mutexattr_settype(&attr, PTHREAD_MUTEX_TIMED_NP);
	m_mutex = PTHREAD_MUTEX_INITIALIZER;
	
	int result = pthread_mutex_init(&m_mutex, &attr);
	printf("result:%d\n", result);
	assert(!result);

	pthread_mutexattr_destroy(&attr);
}

Mutex::~Mutex()
{
	int result = pthread_mutex_destroy(&m_mutex);
	assert(!result);
}

void Mutex::lock()
{
	int result = pthread_mutex_lock(&m_mutex);
	assert(!result);
}

bool Mutex::tryLock()
{
	int result = pthread_mutex_trylock(&m_mutex);

	if (result == 0)
		return true;
	if (result == -1)
		return false;

	return false;
}

void Mutex::unlock()
{
	int result = pthread_mutex_unlock(&m_mutex);
	assert(!result);
}
#endif