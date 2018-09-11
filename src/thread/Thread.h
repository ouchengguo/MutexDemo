#ifndef Thread_h
#define Thread_h
#include "Mutex.h"

#include <string>
#include <map>
#include <stdio.h>
#include <iostream>
using namespace std;

typedef unsigned int  ThreadIdentifier;
typedef unsigned long  DWORD;
typedef void* HANDLE;

class Thread
{
public:
	~Thread(){};

	// static member function for access static member variable
	static Thread* getSingleton()
	{
		if (NULL == _Instance){
			_Instance = new Thread;
		}
		return _Instance;
	}

	void storeThreadHandleByIdentifier(DWORD threadID, HANDLE threadHandle);
	HANDLE threadHandleForIdentifier(ThreadIdentifier id);
	void clearThreadHandleForIdentifier(ThreadIdentifier id);

	ThreadIdentifier createThreadInternal(const char* threadName);
	ThreadIdentifier currentThread();
	int waitForThreadCompletion(ThreadIdentifier threadID);
	void detachThread(ThreadIdentifier threadID);

private:
	Thread(){}; //singleton construct private
	static Thread* _Instance; // static member variable
};

#endif // !Thread_h

