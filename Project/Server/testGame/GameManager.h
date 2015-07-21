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
	void ChangeState(int);
	void StartGame();
	void setPort(HANDLE);
};

