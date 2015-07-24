#pragma once
class NPCManager
{
private:
	Lock *key;
public:
	int NpcCount;
	std::map<int, std::shared_ptr<NPC>> mappingNPC;
	void registNPC(std::shared_ptr<NPC>);
	std::shared_ptr<NPC> retNPC(int);
	void removeNPC(int);

	NPCManager();
	~NPCManager();
};

