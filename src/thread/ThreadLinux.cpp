#include "Thread.h"

#if LINUX
Thread* Thread::_Instance = new Thread;
int tickets = 20;//tickets���ڱ����̲߳�����ע����һ��ȫ�ֱ��� 
static Mutex& threadMapMutex()
{
	static Mutex mutex;
	return mutex;
}

static map<DWORD, HANDLE>& threadMap()
{
	static map<DWORD, HANDLE>* gMap;
	if (!gMap)
		gMap = new map<DWORD, HANDLE>();
	return *gMap;
}

void Thread::storeThreadHandleByIdentifier(DWORD threadID, HANDLE threadHandle)
{
	MutexLocker locker(threadMapMutex());
	threadMap().insert(map<DWORD, HANDLE>::value_type(threadID, threadHandle));
}

HANDLE Thread::threadHandleForIdentifier(ThreadIdentifier id)
{
	MutexLocker locker(threadMapMutex());
	map<DWORD, HANDLE>::iterator key = threadMap().find(id);
	if (key != threadMap().end())
	{
		return key->second;
	}
	return NULL;
}

void Thread::clearThreadHandleForIdentifier(ThreadIdentifier id)
{
	MutexLocker locker(threadMapMutex());
	threadMap().erase(id);
}

static void* wtfThreadEntryPoint(void* param)
{
	MutexLocker locker(threadMapMutex());
	char *thread_name = (char *)param;
	printf("%s begin!\n", thread_name);
	//������������ת��Ϊ�ַ���  
	while (true)//�̵߳�ִ�б�����while���䣬Thread_Function�Ĵ��������ˣ��߳�Ҳ��������  
	{
		if (tickets > 0)
		{
			tickets--;//����ͬʱ����ticket����������׵ģ�  
			sleep(1);
			printf("thread_name:%s, tickets:%d\n", thread_name, tickets);
		}
		else {
			break;
		}
	}
	tickets = 10;
	printf("%s end!\n", thread_name);
	return 0;
}

ThreadIdentifier Thread::createThreadInternal(const char* threadName)
{
	static ThreadIdentifier threadID = 1;

	pthread_t threadHandle;
	if (pthread_create(&threadHandle, 0, wtfThreadEntryPoint, (void *)threadName)) {
		printf("Failed to create pthread at entry point %p with thread name: %s", wtfThreadEntryPoint, threadName);
		return 0;
	}

	storeThreadHandleByIdentifier(threadID, (void *)threadHandle);
	return threadID++;
}

ThreadIdentifier Thread::currentThread()
{
	//return static_cast<ThreadIdentifier>(GetCurrentThreadId());
	return NULL;
}

static pthread_t pthreadHandleForIdentifierWithLockAlreadyHeld(ThreadIdentifier id)
{
	return NULL;
}

int Thread::waitForThreadCompletion(ThreadIdentifier threadID)
{
	pthread_t pthreadHandle;
	assert(threadID);

	
	// We don't want to lock across the call to join, since that can block our thread and cause deadlock.
	MutexLocker locker(threadMapMutex());
	pthreadHandle = pthreadHandleForIdentifierWithLockAlreadyHeld(threadID);
	assert(!pthreadHandle);

	int joinResult = pthread_join(pthreadHandle, 0);

	if (joinResult == -1)
		printf("ThreadIdentifier %u was found to be deadlocked trying to quit", threadID);
	else if (joinResult)
		printf("ThreadIdentifier %u was unable to be joined.\n", threadID);
	clearThreadHandleForIdentifier(threadID);

	return joinResult;
}

void Thread::detachThread(ThreadIdentifier threadID)
{
	assert(threadID);

	MutexLocker locker(threadMapMutex());
	pthread_t pthreadHandle = pthreadHandleForIdentifierWithLockAlreadyHeld(threadID);
	assert(pthreadHandle);

	int detachResult = pthread_detach(pthreadHandle);
	if (detachResult)
		printf("ThreadIdentifier %u was unable to be detached\n", threadID);
	clearThreadHandleForIdentifier(threadID);
}
#endif //LINUX