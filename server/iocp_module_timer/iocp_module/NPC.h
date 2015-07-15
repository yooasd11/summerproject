#pragma once
class NPC
{
private:
	std::string Name;
	int x;
	int y;
	
public:
	int th;
	NPC();
	NPC(std::string, int, int, int);
	~NPC();
	void setNPC(std::string, int, int, int);
	void Move();
	std::string getNPCname();
	int getX();
	int getY();
};

