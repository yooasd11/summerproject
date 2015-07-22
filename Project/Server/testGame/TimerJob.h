#pragma once
typedef struct{
	std::function<void()> func;
	enum state{
		UserMove = 1,
		UserAttact,
		NPCMove,
		NPCAttact,
	};
	enum state current;
	DWORD exectime;
	int th;
}TimerJob;