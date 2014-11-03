// ThreadPool.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include "ThreadPool.h"
#include <tchar.h>
#include <iostream>
#include <conio.h>
#include "Thread.h"
char *text = new char[128];
int nTasks;
HANDLE mutex = CreateMutex(NULL, FALSE, NULL);
int _tmain(int argc, _TCHAR* argv[])
{

	int nThreads;
	char c;
	//cout << c << endl;
	cout << "Enter number of threads:" << endl;
	cin >> nThreads;
	if (nThreads < 1)
	{
		cout << "Should be at least 1" << endl;
		_getch();
	}
	else
	{
		cout << "To add task enter ''task''" << endl;
		cout << "To add thread enter ''threada''" << endl;
		cout << "To remove thread enter ''threadr''" << endl;
		ThreadPool* pool = new ThreadPool(nThreads);
		while (true){
			cin >> text;
			if (strcmp(text, "task") == 0){
				cout << "Input number of tasks: ''" << endl;
				cin >> nTasks;
				if (nTasks != 0){
					pool->AddTask(nTasks);
					//WaitForSingleObject(pool->GethMutexx(), INFINITE);
					/*for (unsigned i = 0; i < nTasks; i++){
						pool->AddTask(new Task());
					}*/
					//ReleaseMutex(pool->GethMutexx());
				}
				else if (nTasks == 0){
					delete pool;
					return 0;
				}
			}
			else if (strcmp(text, "threada") == 0){
				WaitForSingleObject(pool->GethMutexx(), INFINITE);
				pool->AddThread();
				ReleaseMutex(pool->GethMutexx());
			}
			else if (strcmp(text, "threadr") == 0){
				pool->RemoveThread();
			}
			else{
				cout << "Unknown command." << endl;
			}
		}
	}
	_getch();
	_getch();
	return 0;
}

/*#include <windows.h>

#define THREADS_NUMBER 10
#define ITERATIONS_NUMBER 100
#define PAUSE 100  ms

DWORD dwCounter = 0;

DWORD WINAPI ThreadProc(CONST LPVOID lpParam) {
CONST HANDLE hMutex = (CONST HANDLE)lpParam;
DWORD i;
for (i = 0; i < ITERATIONS_NUMBER; i++) {
WaitForSingleObject(hMutex, INFINITE);
dwCounter++;
ReleaseMutex(hMutex);
Sleep(PAUSE);
}
ExitThread(0);
}

VOID Error(CONST HANDLE hStdOut, CONST LPCWSTR szMessage) {
DWORD dwTemp;
TCHAR szError[256];
WriteConsole(hStdOut, szMessage, lstrlen(szMessage), &dwTemp, NULL);
wsprintf(szError, TEXT("LastError = %d\r\n"), GetLastError());
WriteConsole(hStdOut, szError, lstrlen(szError), &dwTemp, NULL);
ExitProcess(0);
}

INT main() {
TCHAR szMessage[256];
DWORD dwTemp, i;
HANDLE hThreads[THREADS_NUMBER];
CONST HANDLE hStdOut = GetStdHandle(STD_OUTPUT_HANDLE);
CONST HANDLE hMutex = CreateMutex(NULL, FALSE, NULL);
if (NULL == hMutex) {
Error(hStdOut, TEXT("Failed to create mutex.\r\n"));
}

for (i = 0; i < THREADS_NUMBER; i++) {
hThreads[i] = CreateThread(NULL, 0, &ThreadProc, hMutex, 0, NULL);
if (NULL == hThreads[i]) {
Error(hStdOut, TEXT("Failed to create thread.\r\n"));
}
}

WaitForMultipleObjects(THREADS_NUMBER, hThreads, TRUE, INFINITE);
wsprintf(szMessage, TEXT("Counter = %d\r\n"), dwCounter);
WriteConsole(hStdOut, szMessage, lstrlen(szMessage), &dwTemp, NULL);

for (i = 0; i < THREADS_NUMBER; i++) {
CloseHandle(hThreads[i]);
}
CloseHandle(hMutex);
_getch();
ExitProcess(0);
}*/

