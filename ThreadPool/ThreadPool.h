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
	queue<HANDLE> threadQueue;
	void initialize();
	HANDLE *threads;
	DWORD nThreads;
	HANDLE hStdOut;
public:
	HANDLE GethMutexx();
	ThreadPool(DWORD n);
	~ThreadPool();
	//void AddTask(Task* NewTask);
	void AddTask(int n);
	void AddThread();
	void RemoveThread();
	void Run();
	static void Log(string str);
	void Error(CONST HANDLE hStdOut, CONST LPCWSTR szMessage);
	static void WINAPI threadTask(LPVOID threadParams);
};