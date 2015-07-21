#pragma once
class NPC
{
private:
	int x;
	int y;
	int LIFE;
	int th;
	Lock *Key;
public:
	NPC();
	NPC(int, int, int, int);
	~NPC();
	void Move();
	void Attack(std::map<SOCKET, USER>);
};

