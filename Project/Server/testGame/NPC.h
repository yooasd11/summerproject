#pragma once
class NPC
{
private:
	Lock* key;
public:
	int state; //죽었나 살았나 움직이나..
	int nid;
	int hp;
	float x;
	float y;
	float direction;
	float velocity;

	NPC();
	NPC(int, int, int, int);
	~NPC();
	void NPCmove();
};

