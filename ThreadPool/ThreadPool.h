#pragma once

#include "windows.h"
#include <queue>
#include <iostream>
#include <string>
#include <queue>
#include "Task.h"

#include <conio.h>
using namespace std;
#define THREADS_NUMBER 15
#define ITERATIONS_NUMBER 100
#define PAUSE 10 /* ms */

class ThreadPool
{
private:
	queue<Task*> tasksQueue;
	void initialize();
	HANDLE *threads;
	DWORD nThreads;
	//HANDLE hMutex;
	//HANDLE mutexLog;
	HANDLE hStdOut;
public:
	ThreadPool(DWORD n);
	~ThreadPool();
	void AddTask(Task* NewTask);
	void AddThread();
	void RemoveThread();
	void Run();
	static void Log(string str);
	void Error(CONST HANDLE hStdOut, CONST LPCWSTR szMessage);
	static void WINAPI threadTask(LPVOID threadParams);
};