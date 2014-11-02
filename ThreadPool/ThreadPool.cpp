#include "stdafx.h"
#include "ThreadPool.h"
#include <stdlib.h>
#include <time.h>
DWORD dwCounter = 0;
int numberOfThreads;
HANDLE hMutexx;
HANDLE mutexLog;
HANDLE threadManageMutex;
static int t = 1;
std::vector<HANDLE> intVector;
typedef struct FUNCDATASTRUCT {
	ThreadPool* threadpool;
	int ID;
};
int numberOfTask = 0;

ThreadPool::ThreadPool(DWORD n)
{
	srand(time(NULL));
	nThreads = n;
	numberOfThreads = n;
	hMutexx = CreateMutex(NULL, FALSE, NULL);
	if (NULL == hMutexx) {
		Error(hStdOut, TEXT("Failed to create mutex for tasks.\r\n"));
	}
	mutexLog = CreateMutex(NULL, FALSE, NULL);
	if (NULL == mutexLog) {
		Error(hStdOut, TEXT("Failed to create mutex for logining.\r\n"));
	}
	threadManageMutex = CreateMutex(NULL, FALSE, NULL);
	CONST HANDLE hStdOut = GetStdHandle(STD_OUTPUT_HANDLE);
	for (int i = 0; i < 7; i++)
	{
		Task* myTask = new Task();
		this->AddTask((Task*)myTask);
	}
	initialize();
}

void ThreadPool::initialize()
{
	if (nThreads > THREADS_NUMBER){
		string str = "Number of threads is bigger than allowed";
		Log(str);
		_getch();
		ExitProcess(0);
	}
	else{
		threads = new HANDLE[nThreads];
		DWORD threadId;
		FUNCDATASTRUCT* myData;
		for (unsigned i = 0; i < nThreads; i++)
		{
			myData = (FUNCDATASTRUCT*)HeapAlloc(GetProcessHeap(), HEAP_ZERO_MEMORY,
				sizeof(FUNCDATASTRUCT));
			myData->ID = i;
			myData->threadpool = this;
			//threads[i] = CreateThread(NULL, 0, (LPTHREAD_START_ROUTINE)threadTask, (LPVOID)myData, NULL, &threadId);
			intVector.push_back(CreateThread(NULL, 0, (LPTHREAD_START_ROUTINE)threadTask, (LPVOID)myData, NULL, &threadId));
			if (NULL == threads[i]) {
				Error(hStdOut, TEXT("Failed to create thread.\r\n"));
			}
			else{
				string str = "Thread created, id = " + to_string(threadId) + "\n";
				Log(str);
			}
		}
		Log("\n");
	}
}
void ThreadPool::AddThread()
{
	DWORD threadId;
	FUNCDATASTRUCT* myData;
	WaitForSingleObject(threadManageMutex, INFINITE);
	myData = (FUNCDATASTRUCT*)HeapAlloc(GetProcessHeap(), HEAP_ZERO_MEMORY,
		sizeof(FUNCDATASTRUCT));
	myData->ID = nThreads++;
	myData->threadpool = this;
	intVector.push_back(CreateThread(NULL, 0, (LPTHREAD_START_ROUTINE)threadTask, (LPVOID)myData, NULL, &threadId));
	Log("New thread was added\n");
	ReleaseMutex(threadManageMutex);
}
void ThreadPool::RemoveThread()
{
	HANDLE threadId = intVector.front();
	WaitForSingleObject(threadId, INFINITE);
	TerminateThread(threadId, 0);
	intVector.erase(intVector.begin());
	Log("Thread was removed, id: " + to_string((DWORD)threadId) + "\n");
}

void ThreadPool::AddTask(Task* NewTask)
{
	WaitForSingleObject(hMutexx, INFINITE);
	tasksQueue.push(NewTask);
	Log("New task was added.\n");
	ReleaseMutex(hMutexx);

}

ThreadPool::~ThreadPool()
{
	for (unsigned i = 0; i < nThreads; i++)
	{
		TerminateThread(threads[i], 0);
		WaitForSingleObject(threads[i], INFINITE);
		Log("Thread with id " + to_string(GetThreadId(threads[i])) + " terminated\n");
	}
	for (unsigned i = 0; i < nThreads; i++) {
		CloseHandle(threads[i]);
	}
	delete[] threads;
	string str = "Counter = " + to_string(dwCounter);
	Log(str);
	CloseHandle(hMutexx);
	CloseHandle(mutexLog);
	_getch();
	ExitProcess(0);
}

void WINAPI ThreadPool::threadTask(LPVOID threadParams)
{
	Task* f;
	WaitForSingleObject(hMutexx, INFINITE);
	FUNCDATASTRUCT* arguments = (FUNCDATASTRUCT*)threadParams;
	ThreadPool* threadPool = arguments->threadpool;
	while (true)
	{
		if (!(threadPool->tasksQueue.empty()))
		{
			f = threadPool->tasksQueue.front();
			threadPool->tasksQueue.pop();
			ReleaseMutex(hMutexx);
			t = 1 + rand() % 5;
			numberOfTask++;
			Log("Thread " + to_string(arguments->ID) + " started doing task " + to_string(numberOfTask) + ", and sleeps : " + to_string(t) + " seconds. Thread's id: " + to_string(GetCurrentThreadId()) + "\n");
			f->Execute(t * 1000);
		}
		else{
			ReleaseMutex(hMutexx);
		}
	}
}

void ThreadPool::Log(string str)
{
	WaitForSingleObject(mutexLog, INFINITE);
	cout << str;
	ReleaseMutex(mutexLog);
}

void ThreadPool::Error(CONST HANDLE hStdOut, CONST LPCWSTR szMessage) {
	DWORD dwTemp;
	TCHAR szError[256];
	WriteConsole(hStdOut, szMessage, lstrlen(szMessage), &dwTemp, NULL);
	wsprintf(szError, TEXT("LastError = %d\r\n"), GetLastError());
	WriteConsole(hStdOut, szError, lstrlen(szError), &dwTemp, NULL);
	ExitProcess(0);
}
