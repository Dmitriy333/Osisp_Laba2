#include "stdafx.h"
#include "Task.h"
#include "ThreadPool.h"
#include <stdlib.h>
#include <time.h>
Task::Task()
{
}


Task::~Task()
{
}
void Task::Execute(DWORD t)
{
	Sleep(t);
	//srand((unsigned)time(NULL));
	//Sleep(1 + (rand() % 2)*1000);
}
