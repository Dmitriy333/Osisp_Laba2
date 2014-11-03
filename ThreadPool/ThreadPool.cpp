#include "stdafx.h"
#include "ThreadPool.h"
#include <stdlib.h>
#include <time.h>
#include <vector>
DWORD dwCounter = 0;
int numberOfThreads;
static int t = 1;
static HANDLE mutexLog;
static HANDLE hMutexx;
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
	CONST HANDLE hStdOut = GetStdHandle(STD_OUTPUT_HANDLE);
	/*for (int i = 0; i < 7; i++)
	{
		Task* myTask = new Task();
		this->AddTask((Task*)myTask);
	}*/
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
			threadQueue.push((CreateThread(NULL, 0, (LPTHREAD_START_ROUTINE)threadTask, (LPVOID)myData, NULL, &threadId)));
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
	WaitForSingleObject(hMutexx, INFINITE);
	myData = (FUNCDATASTRUCT*)HeapAlloc(GetProcessHeap(), HEAP_ZERO_MEMORY,
		sizeof(FUNCDATASTRUCT));
	myData->ID = nThreads++;
	myData->threadpool = this;
	threadQueue.push((CreateThread(NULL, 0, (LPTHREAD_START_ROUTINE)threadTask, (LPVOID)myData, NULL, &threadId)));
	string str = "Thread created, id = " + to_string(threadId) + "\n";
	Log(str);
	ReleaseMutex(hMutexx);
}
void ThreadPool::RemoveThread()
{
	WaitForSingleObject(hMutexx, INFINITE);
	if (!(threadQueue.empty()))
	{
		HANDLE threadId = threadQueue.front();
		threadQueue.pop();
		TerminateThread(threadId, 0);
		Log("Thread with id " + to_string(GetThreadId(threadId)) + " terminated\n");
		//Log("Thread was deleted.\n");
		ReleaseMutex(hMutexx);
	}
	else{
		ReleaseMutex(hMutexx);
	}
}

void ThreadPool::AddTask(int n)
{
	WaitForSingleObject(hMutexx, INFINITE);
	numberOfTask = 0;
	for (int i = 0; i < n; i++)
	{
		tasksQueue.push(new Task);
		Log("New task was added.\n");
	}
	ReleaseMutex(hMutexx);
}

ThreadPool::~ThreadPool()
{
	while (!(threadQueue.empty())){
		HANDLE threadId = threadQueue.front();
		threadQueue.pop();
		TerminateThread(threadId, 0);
		WaitForSingleObject(threadId, INFINITE);
		Log("Thread was deleted, " + to_string(GetThreadId(threadId)) + ".\n");
		CloseHandle(threadId);
		ReleaseMutex(hMutexx);
	}
	CloseHandle(hMutexx);
	CloseHandle(mutexLog);
	Log("Stopped. Press any key to exit.\n");
	_getch();
	ExitProcess(0);
	/*for (unsigned i = 0; i < nThreads; i++)
	{
		TerminateThread(threads[i], 0);
		WaitForSingleObject(threads[i], INFINITE);
		Log("Thread with id " + to_string(GetThreadId(threads[i])) + " terminated\n");
	}
	for (unsigned i = 0; i < nThreads; i++) {
		CloseHandle(threads[i]);
	}*/

	//delete[] threads;
	/*string str = "Counter = " + to_string(dwCounter);
	Log(str);*/
	
}
HANDLE ThreadPool::GethMutexx()
{
	return hMutexx;
}
void WINAPI ThreadPool::threadTask(LPVOID threadParams)
{
	Task* f;
	
	FUNCDATASTRUCT* arguments = (FUNCDATASTRUCT*)threadParams;
	ThreadPool* threadPool = arguments->threadpool;
	boolean awaitingLog = false;
	while (true)
	{
		WaitForSingleObject(hMutexx, INFINITE);
		if (!(threadPool->tasksQueue.empty()))
		{
			f = threadPool->tasksQueue.front();
			threadPool->tasksQueue.pop();
			t = 1 + rand() % 5;
			numberOfTask++;
			Log("Thread " + to_string(arguments->ID) + " started doing task " + to_string(numberOfTask) + ", and sleeps : " + to_string(t) + " seconds. Thread's id: " + to_string(GetCurrentThreadId()) + "\n");
			ReleaseMutex(hMutexx);
			f->Execute(t * 1000);
		}
		else {
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
