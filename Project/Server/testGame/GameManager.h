#pragma once
class GameManager
{
private:
	int State;
	HANDLE hPort;
	Lock* key;
public:
	int bulletCount;
	std::map<int, std::shared_ptr<bullet>> mappingBullet;
	void registBullet(std::shared_ptr<bullet>);
	std::shared_ptr<bullet> retBullet(int);
	void removeBullet(int);

	GameManager();
	~GameManager();
	void ChangeState(int);
	void StartGame();
	void setPort(HANDLE);
};

