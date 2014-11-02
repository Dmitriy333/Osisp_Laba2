#pragma once
#include "stdafx.h"
#include <windows.h>
#include "stdlib.h"
#include <iostream>
#include <string>
#include <conio.h>
#include <queue>
#include <Wincrypt.h>
#include <atlstr.h>

class Task
{
public:
	Task();
	void Execute(DWORD time);
	~Task();
};