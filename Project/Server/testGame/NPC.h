#pragma once
class NPC
{
private:
	Lock* key;
public:
	int state; //�׾��� ��ҳ� �����̳�..
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

