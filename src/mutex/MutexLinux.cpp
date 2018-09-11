#include "Mutex.h"

#if LINUX
Mutex::Mutex()
{
	pthread_mutexattr_t attr;
	pthread_mutexattr_init(&attr);
	pthread_mutexattr_settype(&attr, PTHREAD_MUTEX_NORMAL);

	int result = pthread_mutex_init(&m_mutex, &attr);
	ASSERT_UNUSED(result, !result);

	pthread_mutexattr_destroy(&attr);
}

Mutex::~Mutex()
{
	int result = pthread_mutex_destroy(&m_mutex);
	ASSERT_UNUSED(result, !result);
}

void Mutex::lock()
{
	int result = pthread_mutex_lock(&m_mutex);
	ASSERT_UNUSED(result, !result);
}

bool Mutex::tryLock()
{
	int result = pthread_mutex_trylock(&m_mutex);

	if (result == 0)
		return true;
	if (result == EBUSY)
		return false;

	ASSERT_NOT_REACHED();
	return false;
}

void Mutex::unlock()
{
	int result = pthread_mutex_unlock(&m_mutex);
	ASSERT_UNUSED(result, !result);
}
#endif