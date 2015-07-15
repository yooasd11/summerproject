#pragma once
class NPCManager
{
private:
	IoData *ioInfo;
	int NPCcount;
public:
	NPC _NPC[10];
	NPCManager();
	~NPCManager();
	void StartNPC(HANDLE hPort);
	void tempFunction();
};

