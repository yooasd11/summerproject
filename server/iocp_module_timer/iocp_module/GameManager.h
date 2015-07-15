#pragma once
class GameManager
{
private:
	int State;
	HANDLE hPort;
public:
	NPCManager *manageNPC;
	GameManager();
	~GameManager();
	int ChangeState(int);
	void StartGame();
	void setPort(HANDLE);
};

