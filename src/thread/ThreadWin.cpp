#include "Thread.h"

#if WIN
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

unsigned WINAPI wtfThreadEntryPoint(void* param)
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
			Sleep(1000);
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
	unsigned threadIdentifier = 0;
	ThreadIdentifier threadID = 0;
	
	HANDLE threadHandle = reinterpret_cast<HANDLE>(_beginthreadex(0, 0, wtfThreadEntryPoint, (void *)threadName, 0, &threadIdentifier));
	if (!threadHandle) {
		printf("Failed to create thread at entry point %p with data : %ld", this, errno);
		return 0;
	}

	threadID = static_cast<ThreadIdentifier>(threadIdentifier);
	storeThreadHandleByIdentifier(threadIdentifier, threadHandle);

	return threadID;
}

ThreadIdentifier Thread::currentThread()
{
	return static_cast<ThreadIdentifier>(GetCurrentThreadId());
}

int Thread::waitForThreadCompletion(ThreadIdentifier threadID)
{
	assert(threadID);

	HANDLE threadHandle = threadHandleForIdentifier(threadID);
	if (!threadHandle)
		printf("ThreadIdentifier %u did not correspond to an active thread when trying to quit", threadID);

	DWORD joinResult = WaitForSingleObject(threadHandle, INFINITE);
	if (joinResult == WAIT_FAILED)
		printf("ThreadIdentifier %u was found to be deadlocked trying to quit", threadID);

	CloseHandle(threadHandle);
	clearThreadHandleForIdentifier(threadID);

	return joinResult;
}

void Thread::detachThread(ThreadIdentifier threadID)
{
	assert(threadID);

	HANDLE threadHandle = threadHandleForIdentifier(threadID);
	if (threadHandle)
		CloseHandle(threadHandle);
	clearThreadHandleForIdentifier(threadID);
}

#endif //WIN