#pragma once
class NPC
{
private:
	Lock* key;
public:
	enum state{
		alive = 1,
		dead,
	};
	enum state current;
	int nid;
	int hp;
	float x;
	float y;
	float direction;
	float velocity;

	NPC();
	NPC(int,int, float, float, float, float);
	~NPC();
	float getDegree(float, float, float, float);
	void Action();
};

