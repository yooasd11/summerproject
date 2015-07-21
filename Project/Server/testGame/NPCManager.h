#pragma once
class NPCManager
{
private:
	Lock *key;
public:
	std::vector<NPC> npc;
	NPCManager();
	~NPCManager();
};

