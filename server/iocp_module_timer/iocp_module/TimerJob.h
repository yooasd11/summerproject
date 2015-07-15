#pragma once
class TimerJob
{
public:
	std::function<void()> func;
	DWORD exectime;
	int npc_th;
	TimerJob();
	~TimerJob();
};

