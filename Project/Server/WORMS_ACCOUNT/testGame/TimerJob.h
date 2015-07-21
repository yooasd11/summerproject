#pragma once
typedef struct{
	std::function<void()> func;
	DWORD type;
	DWORD exectime;
	int th;
}TimerJob;